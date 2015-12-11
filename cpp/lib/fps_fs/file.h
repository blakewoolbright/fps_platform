#ifndef FPS__FS__FILE__H
#define FPS__FS__FILE__H

#include "fps_fs/detail/file_iface.h"
#include "fps_util/byte_queue.h"
#include "fps_util/extractors.h"
#include <string>

namespace fps {
namespace fs  {

  //--------------------------------------------------------------------------------------
  class Path ;

  //--------------------------------------------------------------------------------------
  class File
  {
  private :
    detail::IFile * impl_ ;
    std::string     name_ ;
    std::string     mode_ ;

  public :
    //------------------------------------------------------------------------------------
    File() ;
    File( const std::string & fname, const std::string & mode ) ;
    File( const Path        & fpath, const std::string & mode ) ;
    explicit File( FILE * fp ) ;
    ~File() ;

    //------------------------------------------------------------------------------------
    inline const std::string & name() const { return name_ ; }
    inline const std::string & mode() const { return mode_ ; }

    //------------------------------------------------------------------------------------
    inline bool is_open() const { return impl_ ? impl_->is_open() : false ; }
    inline bool eof()     const { return impl_ ? impl_->eof()     : true ; }

    //------------------------------------------------------------------------------------
    bool open( const std::string & fname, const std::string & fmode ) ;
    bool close() ;
    bool flush() ;

    //------------------------------------------------------------------------------------
    inline
    int32_t
    read( char * rx_dest, uint32_t rx_bytes )
    { 
      return ( impl_ ) ? impl_->read( rx_dest, rx_bytes ) : -1 ;
    }

    //------------------------------------------------------------------------------------
    inline
    int32_t
    write( const char * wx_src, uint32_t wx_bytes )
    { 
      return ( impl_ ) ? impl_->write( wx_src, wx_bytes ) : -1 ;
    }

    //------------------------------------------------------------------------------------
    inline 
    int32_t 
    write( const char * wx_src ) 
    { 
      return ( wx_src != NULL ) ? write( wx_src, std::strlen( wx_src ) ) : -1 ;  
    }

    //------------------------------------------------------------------------------------
    inline 
    int32_t 
    write( const std::string & wx_str ) 
    { 
      return ( !wx_str.empty() ) ? write( wx_str.c_str(), wx_str.length() ) : -1 ;  
    }

    //------------------------------------------------------------------------------------
    // Return false if the system level file error flag is set
    //------------------------------------------------------------------------------------
    inline bool get_error_flag() const { return impl_ ? !(impl_->get_error_flag()) : true ; }

    //------------------------------------------------------------------------------------
    inline
    void
    clr_error_flag()
    { 
      if( impl_ ) impl_->clr_error_flag() ;
    }

    //------------------------------------------------------------------------------------
    // Return the value of the system ::fseek() function (stdio.h)
    //
    // The 'from_position' argument can be positive or negative depending on whether
    // you want to seek forward or backward.  You may also use the pre-defined seek
    // targets :
    //   SEEK_SET ( file start )
    //   SEEK_CUR ( current position )
    //   SEEK_END ( file end )
    //------------------------------------------------------------------------------------
    inline
    bool
    seek( int64_t delta, int64_t from=SEEK_CUR )
    { 
      return ( impl_ )
             ? (impl_->seek( delta, from ) >= 0)
             : false
             ;
    }

    //------------------------------------------------------------------------------------
    // Return the value of the system ::ftell() function (stdio.h)
    //------------------------------------------------------------------------------------
    inline int64_t tell() const { return impl_ ? impl_->tell() : -1 ; }
  } ;

}}

#endif

