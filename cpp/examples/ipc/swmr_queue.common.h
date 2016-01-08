#ifndef FPS__EXAMPLES__SWMR_QUEUE_COMMON__H
#define FPS__EXAMPLES__SWMR_QUEUE_COMMON__H

#include <stdint.h>
#include <string>

namespace fps {
namespace examples {
namespace ipc      {
namespace swmr     {

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
  static const uint32_t Capacity     = 1024 ;
  static const char     Queue_Name[] = "fps.swmr_queue.example" ;
}}}}

#endif
