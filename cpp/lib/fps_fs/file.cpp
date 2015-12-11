#include "fps_fs/file.h"
#include "fps_fs/detail/basic_file.h"
#include "fps_fs/detail/gzip_file.h"
#include "fps_fs/path.h"
#include "fps_string/fps_string.h"

namespace fps  { 
namespace fs   {

  //------------------------------------------------------------------------------------
  File::File() 
    : impl_( NULL ) 
  {}

  //------------------------------------------------------------------------------------
  File::File( const std::string & fname, const std::string & fmode ) 
    : impl_ ( NULL )
  { 
    open( fname, fmode ) ;
  } 

  //------------------------------------------------------------------------------------
  File::File( const Path & fpath, const std::string & fmode ) 
    : impl_ ( NULL )
  { 
    open( fpath.str(), fmode ) ;
  } 

  //------------------------------------------------------------------------------------
  File::File( FILE * fp )
    : impl_ ( new detail::BasicFile( fp ) )
  {}

  //------------------------------------------------------------------------------------
  File::~File()
  { 
    flush() ;
    close() ;
  }

  //------------------------------------------------------------------------------------
  bool 
  File::open( const std::string & fname, const std::string & fmode )
  {
    name_ = string::stripped( fname ) ; 
    mode_ = string::lower( fmode ) ;

    if( name_.empty() )
      return false ;

    if( impl_ )
      close() ;

    std::string lc_name = string::lower( name_ ) ;
    if( string::ends_with( lc_name, ".gz" ) ) 
      impl_ = new detail::GZipFile() ;
    else 
      impl_ = new detail::BasicFile() ;

    return impl_->open( name_.c_str(), mode_.c_str() ) ; 
  }

  //------------------------------------------------------------------------------------
  bool 
  File::close() 
  { 
    if( impl_ != NULL && impl_->close() ) 
    { delete impl_ ;
      impl_ = NULL ;
    }
    return (impl_ == NULL) ;
  }

  //------------------------------------------------------------------------------------
  bool File::flush()
  {
    return ( impl_ && impl_->is_open() ) 
           ? impl_->flush() 
           : true 
           ;
  }

}}

