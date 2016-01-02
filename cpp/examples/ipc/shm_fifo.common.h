#ifndef FPS__EXAMPLES__SHM_FIFO_COMMON__H
#define FPS__EXAMPLES__SHM_FIFO_COMMON__H

#include <stdint.h>
#include <string>

namespace fps {
namespace examples {
namespace shm_fifo {

  //---------------------------------------------------------------------------------------
  struct Message
  {
    char content_[ 64 ] ; 

    Message() 
    {
      set( 0 ) ;
    }

    Message( const Message & rhs ) 
    { std::memcpy( content_, rhs.content_, sizeof( content_ ) ) ;
    }

    inline 
    void 
    set( uint64_t value ) 
    { ::sprintf( content_, "\n%.63lu", value ) ;
    }

    inline 
    uint64_t 
    get() const 
    {
      uint32_t rv = ::strtoul( content_, NULL, 10 ) ;
      return rv ;
    }
  } ;

  //---------------------------------------------------------------------------------------
  static const uint32_t Capacity    = 1024 ;
  static const char     Name[]      = "fps.shm_fifo_example.data" ;
}}}

#endif
