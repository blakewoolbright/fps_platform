#ifndef FPS__FS__DETAIL__FILE_IFACE__H
#define FPS__FS__DETAIL__FILE_IFACE__H

#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <fcntl.h>

namespace fps    {
namespace fs     {
namespace detail {

  //--------------------------------------------------------------------------------------
  class IFile 
  {
  public :
    inline IFile() {}
    inline virtual ~IFile() {}

    virtual bool    open( const char *, const char * ) = 0 ;
    virtual bool    close()                            = 0 ;
    virtual bool    flush()                            = 0 ;
    virtual bool    eof() const                        = 0 ;
    virtual bool    is_open() const                    = 0 ;
    virtual bool    get_error_flag() const             = 0 ;
    virtual void    clr_error_flag()                   = 0 ;
    virtual int64_t tell() const                       = 0 ;
    virtual int64_t seek( int64_t , int64_t )          = 0 ;
    virtual int32_t read ( char *, uint32_t )          = 0 ; 
    virtual int32_t write( const char *, uint32_t )    = 0 ; 
  } ;

}}}

#endif

