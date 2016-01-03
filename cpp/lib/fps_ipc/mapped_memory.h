#ifndef FPS__IPC__MAPPED_MEMORY__H
#define FPS__IPC__MAPPED_MEMORY__H

#include <errno.h>
#include <unistd.h>
#include <stdint.h>

namespace fps {
namespace ipc { 

  //-------------------------------------------------------------------------------------
  struct MappedMemory
  {
  private :
    //-----------------------------------------------------------------------------------
    void       * begin_ ;
    std::size_t  size_  ;
    uint32_t     error_ ;

  public :
    //-----------------------------------------------------------------------------------
    inline
    MappedMemory() 
      : begin_( NULL ) 
      , size_ ( 0 )
      , error_( EOK ) 
    {}
      
    //-----------------------------------------------------------------------------------
    inline
    MappedMemory( void * begin, std::size_t size ) 
      : begin_( begin )
      , size_ ( size )
      , error_( ( begin_ == NULL || size_ == 0 ) ? EINVAL : EOK )
    {}

    //-----------------------------------------------------------------------------------
    inline 
    void
    clear() 
    { begin_ = NULL ;
      size_  = 0 ;
      errno_ = EOK ;
    }

    //-----------------------------------------------------------------------------------
    inline bool         empty()      const { return begin_ == NULL || size_ == 0 ; }
    inline const void * begin()      const { return begin_ ; }
    inline std::size_t  size()       const { return size_ ; }
    inline uint32_t     last_error() const { return error_ ; }
  } ;

}}

#endif
