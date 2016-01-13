#ifndef FPS__FS__DETAIL__BASIC_FILE__H
#define FPS__FS__DETAIL__BASIC_FILE__H

#include "fps_fs/detail/file_iface.h"

namespace fps    { 
namespace fs     {
namespace detail {
 
  //--------------------------------------------------------------------------------------
  class BasicFile 
    : public IFile
  {
  private :
    FILE * fp_  ;

  public :
    //------------------------------------------------------------------------------------
    inline 
    BasicFile() 
      : fp_( NULL ) 
    {}

    //------------------------------------------------------------------------------------
    inline 
    explicit 
    BasicFile( const char * fname, const char * mode="r" ) 
      : fp_( NULL ) 
    {
      open( fname, mode ) ;
    }

    //------------------------------------------------------------------------------------
    inline 
    explicit 
    BasicFile( FILE * fp, const char * mode="r" ) 
      : fp_( fp )
    {
      set_blocking_io( true ) ;
    }

    //------------------------------------------------------------------------------------
    inline virtual ~BasicFile() { close() ; }

    //------------------------------------------------------------------------------------
    inline
    bool 
    set_blocking_io( bool yes_no ) 
    {
      if( !fp_ || (fp_==::stdout) || (fp_==::stderr) ) 
        return false ;

      int32_t fcntl_flags = ::fcntl( ::fileno( fp_ ), F_GETFL ) ;
      if( yes_no ) 
        fcntl_flags &= ~O_NONBLOCK ;
      else 
        fcntl_flags |= O_NONBLOCK ;

      ::fcntl( ::fileno( fp_ ), F_SETFL, fcntl_flags ) ;
      return true ;
    }

    //------------------------------------------------------------------------------------
    inline virtual bool is_open() const { return fp_ != NULL ; }

    //------------------------------------------------------------------------------------
    inline virtual bool get_error_flag() const { return fp_ && (0 != ::ferror( fp_ )) ; }
    inline virtual void clr_error_flag()       { ::clearerr( fp_ ) ; }

    //------------------------------------------------------------------------------------
    inline 
    virtual 
    int64_t 
    seek( int64_t delta, int64_t from ) 
    { 
      return ::fseek( fp_, delta, from ) ;
    }

    //------------------------------------------------------------------------------------
    inline virtual int64_t tell() const { return ::ftell( fp_ ) ;  }

    //------------------------------------------------------------------------------------
    inline 
    virtual 
    bool 
    eof() const 
    { 
      return fp_ 
             ? static_cast<bool>( ::feof( const_cast<FILE*>(fp_) ) ) 
             : true 
             ; 
    } 

    //------------------------------------------------------------------------------------
    inline 
    virtual 
    bool 
    open( const char * fname, const char * mode )
    { 
      fp_ = ::fopen( fname, mode ) ;

      if( fp_ == NULL ) 
        return false ;

      set_blocking_io( true ) ; 
      return true ;
    }

    //------------------------------------------------------------------------------------
    inline virtual 
    bool 
    close() 
    {
      if( !fp_ || fp_==stdin || fp_==stdout || fp_==stderr ) 
        return true ;

      if( !::fclose( fp_ ) ) 
        fp_ = NULL ; 

      return ( fp_ == NULL ) ;
    }

    //------------------------------------------------------------------------------------
    inline virtual bool flush() { return !static_cast<bool>( ::fflush( fp_ ) ) ; }
    
    //------------------------------------------------------------------------------------
    inline virtual 
    int32_t 
    read( char * dest, uint32_t len ) 
    { 
      return ::fread( reinterpret_cast<void*>(dest), sizeof(char), len, fp_ ) ;

      /*
      if( !fp_ )
      { 
        errno = EBADF ;
        return false ;
      }
      int32_t rv = ::read( ::fileno( fp_ ), dest, len ) ;
      return ( rv < 0 && (errno == EAGAIN || errno == EWOULDBLOCK) ) 
             ? 0 
             : rv 
             ;
      */
    }

    //------------------------------------------------------------------------------------
    inline 
    virtual 
    int32_t 
    write( const char * buf, uint32_t len ) 
    { 
      return ::fwrite( buf, sizeof(char), len, fp_ ) ;
      /*
      if( fp_ == NULL ) 
      { 
        = EBADF ;
        return false ;
      }
      
      int32_t rv = ::write( ::fileno( fp_ ), buf, len ) ;
      return ( rv < 0 && (errno == EAGAIN || errno == EWOULDBLOCK) ) 
             ? 0 
             : rv 
             ;
      */
    }
  } ;

}}}

#endif
