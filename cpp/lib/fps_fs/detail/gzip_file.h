#ifndef FPS__FS__DETAIL__GZIP_FILE__H
#define FPS__FS__DETAIL__GZIP_FILE__H

#include "fps_fs/detail/file_iface.h"
#include <zlib.h>

namespace fps    { 
namespace fs     {
namespace detail {
 
  //--------------------------------------------------------------------------------------
  class GZipFile 
    : public IFile
  {
  private :
    ::gzFile fp_  ;

  public :
    //------------------------------------------------------------------------------------
    inline 
    GZipFile() 
      : fp_( NULL ) 
    {}

    //------------------------------------------------------------------------------------
    inline 
    explicit 
    GZipFile( const char * fname, const char * mode="r" ) 
      : fp_( NULL ) 
    {
      open( fname, mode ) ;
    }

    //------------------------------------------------------------------------------------
    inline 
    virtual
    ~GZipFile() 
    { close() ;
    }

    //------------------------------------------------------------------------------------
    inline
    bool 
    set_nonblocking() 
    {
      return false ;
    }

    //------------------------------------------------------------------------------------
    inline virtual bool is_open() const { return fp_ != NULL ; }

    //------------------------------------------------------------------------------------
    inline 
    virtual 
    bool 
    get_error_flag() const 
    { 
      if( !is_open() ) 
        return false ;

      int32_t error_code = Z_OK ;
      return ::gzerror( fp_, &error_code ) != NULL ;
    }

    //------------------------------------------------------------------------------------
    inline 
    virtual 
    void 
    clr_error_flag() 
    { 
      if( fp_ ) 
        ::gzclearerr( fp_ ) ; 
    }

    //------------------------------------------------------------------------------------
    inline virtual int64_t tell() const { return fp_ ? ::gztell( fp_ ) : 0 ; }

    //------------------------------------------------------------------------------------
    inline 
    virtual 
    int64_t 
    seek( int64_t delta, int64_t from ) 
    { 
      return fp_ ? ::gzseek( fp_, delta, from ) : 0 ;
    }

    //------------------------------------------------------------------------------------
    inline virtual 
    bool 
    open( const char * fname, const char * mode )
    { 
      fp_ = ::gzopen( fname, mode ) ;
      return ( fp_ != NULL ) ;
    }

    //------------------------------------------------------------------------------------
    inline virtual 
    bool 
    close() 
    {
      if( fp_ == NULL )
        return true ;

      if( ::gzclose( fp_ )==Z_OK ) 
        fp_ = NULL ; 

      return fp_ == NULL ;
    }

    //------------------------------------------------------------------------------------
    inline 
    virtual 
    bool 
    flush() 
    { 
      return fp_ ? ::gzflush( fp_, Z_SYNC_FLUSH ) == Z_OK : false ; 
    } 
    
    //------------------------------------------------------------------------------------
    inline virtual 
    int32_t 
    read( char * dest, uint32_t len ) 
    { 
      return fp_ ? ::gzread( fp_, reinterpret_cast<void*>(dest), len ) : -2 ; 
    }

    //------------------------------------------------------------------------------------
    inline virtual 
    int32_t 
    write( const char * buf, uint32_t len ) 
    { 
      return fp_ ? ::gzwrite( fp_, buf, len ) : -2 ; 
    }

    //------------------------------------------------------------------------------------
    inline virtual 
    bool 
    eof() const 
    { 
      return ::gzeof( fp_ ) != 0 ;
    } 
  } ;

}}}

#endif
