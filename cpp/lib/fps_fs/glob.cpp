#include "fps_fs/glob.h"

namespace fps {
namespace fs  {

  //------------------------------------------------------------------------------------------------
  Glob::Glob() 
    : flags_   ( Default_Flags )
    , error_id_( 0 )
  {
    std::memset( &glob_, 0, sizeof( glob_t ) ) ;
  } 

  //------------------------------------------------------------------------------------------------
  Glob::Glob( const std::string & expr )
    : flags_   ( Default_Flags ) 
    , error_id_( 0 )
  {
    std::memset( &glob_, 0, sizeof( glob_t ) ) ;
    assign( expr, flags_ ) ; 
  }

  //------------------------------------------------------------------------------------------------
  Glob::Glob( uint32_t flags ) 
    : flags_   ( flags ) 
    , error_id_( 0 ) 
  {
    std::memset( &glob_, 0, sizeof( glob_t ) ) ;
  }

  //------------------------------------------------------------------------------------------------
  Glob::Glob( const std::string & expr, uint32_t flags ) 
    : flags_   ( flags ) 
    , error_id_( 0 )
  {
    std::memset( &glob_, 0, sizeof( glob_t ) ) ;
    assign( expr, flags_ ) ; 
  }

  //------------------------------------------------------------------------------------------------
  Glob::~Glob()
  { 
    if( !empty() ) 
      ::globfree( &glob_ ) ;
  }

  //------------------------------------------------------------------------------------------------
  bool 
  Glob::assign( const std::string & new_expr, uint32_t new_flags ) 
  {
    flags_ = new_flags ;
    expr_  = new_expr ;

    if( Path( expr_ ).is_dir() ) 
      expr_ = Path::join( expr_, "*" ) ;
    
    std::memset( &glob_, 0, sizeof( glob_t ) ) ;
    return sync() ;
  }

  //------------------------------------------------------------------------------------------------
  bool 
  Glob::sync() 
  {
    if( glob_.gl_pathc > 0 ) 
    { 
      ::globfree( &glob_ ) ;
      std::memset( &glob_, 0, sizeof( glob_t ) ) ;
    }
    
    if( 0 == (error_id_ = ::glob( expr_.c_str(), flags_, NULL, &glob_ )) ) 
      return true ;
                        
    std::memset( &glob_, 0, sizeof( glob_t ) ) ;
    return false ;
  }
}}

