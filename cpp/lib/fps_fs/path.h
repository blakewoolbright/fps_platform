#ifndef FPS__FS__PATH__H
#define FPS__FS__PATH__H

#include "fps_string/fps_string.h"

#include <ftw.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <climits>
#include <unistd.h>
#include <pwd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace fps {
namespace fs  {

  //--------------------------------------------------------------------------------------------------
  class Path
  {
  private :
    std::string           path_ ;
    uint32_t              leaf_idx_ ;
    mutable bool          valid_ ;
    mutable int32_t       error_id_ ;
    mutable struct ::stat stat_  ;

  private :

    //------------------------------------------------------------------------------------------------
    // Helper function to find the index of the '/' character that delimits the leaf name from it's 
    // parent's name.
    //------------------------------------------------------------------------------------------------
    static uint32_t get_leaf_idx( const std::string & path ) ;
    // inline uint32_t    leaf_idx() const { return leaf_idx_ ; }

    //------------------------------------------------------------------------------------------------
    // Helper function for use w/ rmtree
    //------------------------------------------------------------------------------------------------
    static 
    int32_t 
    rm_tree_helper( const char          * path_ptr
                  , const struct ::stat * stat_ptr
                  , int                   ftw_flag 
                  , ::FTW               *
                  ) ;

  public :
    //------------------------------------------------------------------------------------------------
    Path() ;
    explicit Path( const std::string & path_str ) ;
    explicit Path( const char * path_str ) ;
    Path         ( const std::string & dir_name, const std::string & base_name ) ;
    Path         ( const Path & dir_name,        const std::string & base_name ) ;

    //------------------------------------------------------------------------------------------------
    Path( const Path & rhs ) ;

    //------------------------------------------------------------------------------------------------
    inline const std::string & str()        const { return path_ ; }         
    inline const char        * c_str()      const { return path_.c_str() ; } 
    inline int32_t             error_id()   const { return error_id_ ; }   //TODO: Rename last_error()
    inline int32_t             last_error() const { return error_id_ ; }   

    //------------------------------------------------------------------------------------------------
    inline std::string leaf()   const ; // The target file/directory/device name
    inline std::string parent() const ; // The parent directory of the leaf()

    //------------------------------------------------------------------------------------------------
    Path absolute() const ;

    //------------------------------------------------------------------------------------------------
    // TODO: operator=, operator==, operator!= 
    //------------------------------------------------------------------------------------------------
    Path & operator=( const Path & rhs ) ;

    //------------------------------------------------------------------------------------------------
    // Return misc. attributes for current filesystem node
    //------------------------------------------------------------------------------------------------
    inline bool        exists()           const ; 
    inline bool        is_file()          const ;
    inline bool        is_dir()           const ;
    inline bool        is_char_device()   const ;
    inline bool        is_block_device()  const ;
    inline bool        is_pipe()          const ;
    inline bool        is_symlink()       const ; 
    inline bool        is_socket()        const ;
    inline int64_t     size()             const ; 
    inline uint64_t    ts_accessed()      const ; 
    inline uint64_t    ts_modified()      const ; 
    inline int32_t     uid()              const ; 
    inline int32_t     gid()              const ; 

    //------------------------------------------------------------------------------------------------
    inline bool valid()       const { return valid_ && !path_.empty() ; }
    inline bool empty()       const { return path_.empty() ; }               
    inline bool is_absolute() const { return !empty() && path_[0] == '/' ; } 
    inline bool is_relative() const { return !empty() && path_[0] != '/' ; } 

    //------------------------------------------------------------------------------------------------
    bool        sync()  const ;
    std::string owner() const ; 

    //------------------------------------------------------------------------------------------------
    bool assign( const std::string & new_path ) ; 

    //------------------------------------------------------------------------------------------------
    bool touch()  const ; 
    bool mkdirs() const ; 
    bool rmtree() const ; 
    bool rm()     const ; 

    //------------------------------------------------------------------------------------------------
    static 
    inline 
    std::pair<std::string, std::string> 
    split( const std::string & str ) ;

    //------------------------------------------------------------------------------------------------
    static 
    inline 
    std::string 
    join ( const std::string & parent_name, const std::string & leaf_name ) ; 
  } ;

  //--------------------------------------------------------------------------------------------------
  std::string 
  Path::leaf() const 
  {
    return !path_.empty() 
           ? std::string( (path_.c_str() + leaf_idx_), path_.length() - leaf_idx_ ) 
           : std::string() 
           ;
  }

  //--------------------------------------------------------------------------------------------------
  std::string 
  Path::parent() const 
  {
    return (path_.empty() || leaf_idx_ <= 0 )
           ? std::string() 
           : std::string( path_.c_str(), leaf_idx_-1 ) 
           ;
  }

  //--------------------------------------------------------------------------------------------------
  bool 
  Path::exists() const 
  {
    return ( !valid_ && !sync() ) ? false : true ;
  }
  
  //--------------------------------------------------------------------------------------------------
  bool 
  Path::is_file() const
  {
    return ( !valid_ && !sync() ) ? false : S_ISREG( stat_.st_mode ) ;
  }
    
  //--------------------------------------------------------------------------------------------------
  bool 
  Path::is_dir() const
  {
    return ( !valid_ && !sync() ) ? false : S_ISDIR( stat_.st_mode ) ;
  }

  //--------------------------------------------------------------------------------------------------
  bool 
  Path::is_char_device() const
  {
    return ( !valid_ && !sync() ) ? false : S_ISCHR( stat_.st_mode ) ;
  }

  //--------------------------------------------------------------------------------------------------
  bool Path::is_block_device() const
  {
    return ( !valid_ && !sync() ) ? false : S_ISBLK( stat_.st_mode ) ;
  }

  //--------------------------------------------------------------------------------------------------
  bool 
  Path::is_pipe() const
  {
    return ( !valid_ && !sync() ) ? false : S_ISFIFO( stat_.st_mode ) ;
  }

  //--------------------------------------------------------------------------------------------------
  bool 
  Path::is_symlink() const
  {
    return ( !valid_ && !sync() ) ? false : S_ISLNK( stat_.st_mode ) ;
  }

  //--------------------------------------------------------------------------------------------------
  bool 
  Path::is_socket() const 
  {
    return ( !valid_ && !sync() ) ? false : S_ISSOCK( stat_.st_mode ) ;
  }

  //--------------------------------------------------------------------------------------------------
  int64_t 
  Path::size() const 
  {
    return ( !valid_ && !sync()) ? -1 : stat_.st_size ;
  }

  //--------------------------------------------------------------------------------------------------
  int32_t 
  Path::uid() const 
  {
    return ( !valid_ && !sync()) ? -1 : stat_.st_uid ;
  }

  //--------------------------------------------------------------------------------------------------
  int32_t 
  Path::gid() const 
  {
    return (!valid_ && !sync()) ? -1 : stat_.st_gid ;
  }

  //--------------------------------------------------------------------------------------------------
  uint64_t
  Path::ts_accessed() const 
  { 
    return ( !valid_ && !sync() ) ? 0 : static_cast<uint64_t>( stat_.st_atime ) ;
  }

  //--------------------------------------------------------------------------------------------------
  uint64_t
  Path::ts_modified() const 
  {
    return ( !valid_ && !sync() ) ? 0 : static_cast<uint64_t>( stat_.st_mtime ) ;
  }

  //------------------------------------------------------------------------------------------------
  std::pair<std::string, std::string> 
  Path::split( const std::string & str ) 
  {
    if( str.empty() ) 
      return std::pair<std::string, std::string>() ; 

    int32_t leaf_idx = get_leaf_idx( str ) ;

    typedef std::pair<std::string,std::string> rv_t ;
    return ( leaf_idx <= 0 ) 
           ? rv_t( ".", str )
           : rv_t( std::string( str.c_str(), str.c_str() + (leaf_idx-1) )
                 , std::string( str.c_str() + leaf_idx, str.c_str() + (str.length() - leaf_idx) ) 
                 )
           ;
  }

  //------------------------------------------------------------------------------------------------
  std::string 
  Path::join( const std::string & parent_name, const std::string & leaf_name ) 
  {
    if( string::ends_with( parent_name, '/' ) )
      return parent_name + leaf_name ;

    std::string rv = parent_name ; 
    rv.push_back( '/' )  ;
    rv += leaf_name ;
      
    return rv ;
  }
}}

#endif

