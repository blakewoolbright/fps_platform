#include "fps_fs/path.h"

#include <fcntl.h>
#include <vector>

namespace fps {
namespace fs  {

  //--------------------------------------------------------------------------------------------------
  Path::Path() 
    : path_()
    , leaf_idx_( 0 ) 
    , valid_   ( false )
    , error_id_( EBADF ) 
  { 
    std::memset( &stat_, 0, sizeof( stat_ ) ) ; 
  }

  //--------------------------------------------------------------------------------------------------
  Path::Path( const std::string & path_str )
    : leaf_idx_( 0 )
    , valid_   ( false )
    , error_id_( 0 )
  {
    assign( path_str ) ;
  }

  //--------------------------------------------------------------------------------------------------
  Path::Path( const std::string & dir_name, const std::string & base_name )
    : leaf_idx_( 0 )
    , valid_   ( false )
    , error_id_( 0 )
  {
    assign( join( dir_name, base_name ) ) ;
  }

  //--------------------------------------------------------------------------------------------------
  Path::Path( const Path & dir_name, const std::string & base_name )
    : leaf_idx_( 0 )
    , valid_   ( false )
    , error_id_( 0 )
  {
    assign( join( dir_name.str(), base_name ) ) ;
  }

  //--------------------------------------------------------------------------------------------------
  Path::Path( const char * path_str )
    : leaf_idx_( 0 )
    , valid_   ( false )
    , error_id_( 0 )
  {
    assign( std::string( path_str ) ) ;
  }

  //--------------------------------------------------------------------------------------------------
  Path::Path( const Path & rhs )
    : path_    ( rhs.path_ )
    , leaf_idx_( rhs.leaf_idx_ )
    , valid_   ( rhs.valid_ )
    , error_id_( rhs.error_id_ )
  {
    std::memcpy( &stat_, &(rhs.stat_), sizeof( stat_ ) ) ;
  } 

  //--------------------------------------------------------------------------------------------------
  uint32_t 
  Path::get_leaf_idx( const std::string & in_path ) 
  { 
    if( in_path.empty() ) 
      return 0 ;
    
    std::string path = string::stripped( in_path ) ;
  
    //
    // Skip trailing '/' character(s) before searching for delimiter between parent and child
    //
    while( string::ends_with( path, '/' ) ) 
      path.pop_back() ;

    int32_t find_rv = path.rfind( "/" ) ; 
    return ( find_rv < 0 ) ? 0 : find_rv + 1 ;
  }

  //--------------------------------------------------------------------------------------------------
  bool 
  Path::sync() const 
  {
    valid_ = !static_cast<bool>( ::lstat( path_.c_str(), &stat_ ) ) ;

    if( valid_ )  
      return true ;

    error_id_ = errno ;
    return false ;
  }


  //------------------------------------------------------------------------------------------------
  bool 
  Path::assign( const std::string & new_path ) 
  {
    error_id_ = 0 ;
    path_     = new_path ;
    string::strip( path_ ) ;    

    leaf_idx_ = get_leaf_idx( path_ ) ;
    if( leaf_idx_ <= 0 )
    { 
      if( !string::starts_with( path_, "." ) )
      { path_ = join( ".", path_ ) ;
        leaf_idx_ = get_leaf_idx( path_ ) ;
      }
    }

    std::memset( &stat_, 0, sizeof( stat_ ) ) ;
    return sync() ;
  }
  
  //------------------------------------------------------------------------------------------------
  Path
  Path::absolute() const
  {
    if( exists() ) 
    {
      std::string rp_str ; 
      rp_str.resize( PATH_MAX ) ;

      const char * rp_rv = ::realpath( path_.c_str(), const_cast<char *>( rp_str.c_str() ) ) ;

      if( rp_rv ) 
      { 
        rp_str.resize( std::strlen( rp_rv ) ) ;
        return Path( rp_str ) ;
      }
    }
    
    return Path() ;
  }

  //------------------------------------------------------------------------------------------------
  bool 
  Path::mkdirs() const 
  {
    error_id_ = 0 ;

    if( empty() ) 
      return false ;

    auto parts = string::split<std::vector<std::string>>( path_, "/" ) ;

    // If path string not terminated with '/' character, don't create a directory for it.
    if( !string::ends_with( path_, '/' ) ) 
      parts.pop_back() ;

    std::string next_dir ; 
    if( is_absolute() ) 
      next_dir += '/' ;

    for( auto itr=parts.begin() ; itr != parts.end() ; ++itr ) 
    { 
      next_dir += *itr ;
      next_dir += '/' ;

      int mkdir_rv = ::mkdir( next_dir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO ) ;
      if( mkdir_rv && errno != EEXIST )
      { 
        error_id_ = errno ;
        return false ;
      }
    }
    
    sync() ;
    return true ;
  }

  //------------------------------------------------------------------------------------------------
  bool 
  Path::touch() const 
  {
    error_id_ = 0 ;
  
    // 1) Ensure parent directories exist
    if( !mkdirs() ) 
      return false ;

    // 2) If this Path instance is a directory, we're done.
    if( string::ends_with( path_, '/' ) )
      return true ;
  
    // 3) Create the desired file 
    FILE * fp = ::fopen( path_.c_str(), "ab" ) ;
    if( fp == NULL ) 
    { 
      error_id_ = errno ;
      return false ;
    }
    ::fclose( fp ) ; 

    // 4) Synchronize Path instance w/ filesystem
    sync() ;

    return true ;
  }

  //------------------------------------------------------------------------------------------------
  bool 
  Path::rm() const 
  {
    error_id_ = 0 ;

    bool rv = true ;
    if( is_dir() ) 
    { 
      if( ::rmdir( path_.c_str() ) ) 
      { error_id_ = errno ;
        rv = false ;
      }
    }
    else
    {
      if( ::unlink( path_.c_str() ) ) 
      { error_id_ = errno ;
        rv = false ;
      }
    }

    sync() ;
    return rv ;
  }

  //------------------------------------------------------------------------------------------------
  int32_t 
  Path::rm_tree_helper( const char          * path_ptr
                      , const struct ::stat * stat_ptr 
                      , int                   ftw_flag 
                      , ::FTW               * 
                      )
  {
    if( ftw_flag == FTW_DP ) 
    { 
      if( 0 == ::rmdir( path_ptr ) ) 
        return 0 ;
    }
    else 
    { 
      if( 0 == ::unlink( path_ptr ) ) 
        return 0 ;
    }
  
    return errno ;
  } 

  //------------------------------------------------------------------------------------------------
  bool 
  Path::rmtree() const 
  {
    error_id_ = 0 ;

    if( !exists() ) 
      return true ;

    if( is_file() ) 
      return rm() ;

    error_id_ = ::nftw( path_.c_str(), Path::rm_tree_helper, 32, FTW_PHYS|FTW_DEPTH ) ;
    sync() ;

    return ( error_id_ == 0 ) ;
  }

  //--------------------------------------------------------------------------------------------------
  std::string 
  Path::owner() const 
  {
    if( !valid_ && !sync() ) 
      return std::string() ;

    // TODO: Can the query to find out max db rec size be defaulted so we can stack-alloc the buffer?
    int32_t PW_Max_Size = ::sysconf( _SC_GETPW_R_SIZE_MAX ) ; 
    if( PW_Max_Size < 0 ) 
      PW_Max_Size = 16384 ;

    // Reserve space for return value before allocating the buffer below.
    std::string rv ;
    rv.reserve( 256 ) ;

    struct ::passwd   db_rec ;
    struct ::passwd * db_rec_ptr = NULL ;
    char            * db_rec_buf = new char[ PW_Max_Size ] ;

    int32_t result = ::getpwuid_r( stat_.st_uid
                                 , &db_rec
                                 , db_rec_buf
                                 , sizeof(db_rec_buf)
                                 , &db_rec_ptr 
                                 ) ;


    if( !result && db_rec_ptr ) 
      rv = db_rec.pw_name ;

    delete [] db_rec_buf ;
    return rv ;
  }

}}



