#ifndef FPS__EXAMPLES__SWMR_QUEUE_COMMON__H
#define FPS__EXAMPLES__SWMR_QUEUE_COMMON__H

#include <stdint.h>
#include <string>
#include <cstring>

namespace fps {
namespace examples {
namespace ipc      {
namespace swmr     {

  //---------------------------------------------------------------------------------------
  static const uint32_t Capacity     = 1024 * 100 ;
  static const char     Queue_Name[] = "fps.swmr_shm_queue.example" ;
  static const uint32_t Reader_CPU   = 0 ;
  static const uint32_t Writer_CPU   = Reader_CPU + 1 ;

  //---------------------------------------------------------------------------------------
  struct Message 
  {
    uint64_t seq_  ;
    uint64_t w_ts_ ;
    uint64_t r_ts_ ;

    Message() 
      : seq_ ( 0 ) 
      , w_ts_( 0 )
    {}

    Message( uint64_t sequence, uint64_t timestamp ) 
      : seq_ ( 0 ) 
      , w_ts_( 0 )
    {}

    Message( const Message & rhs ) 
      : seq_ ( rhs.seq_ ) 
      , w_ts_( rhs.w_ts_ ) 
    {}

    Message & 
    operator=( const Message & rhs ) 
    { seq_  = rhs.seq_ ;
      w_ts_ = rhs.w_ts_ ;
      return *this ;
    }

    inline 
    void 
    on_write( uint64_t sequence, uint64_t timestamp ) 
    { 
      seq_  = sequence ;
      w_ts_ = timestamp ;
    }

    inline 
    void 
    on_read( uint64_t timestamp )
    { 
      r_ts_ = timestamp ;
    }

    inline uint64_t sequence() const { return seq_ ; }
    inline uint64_t write_ts() const { return w_ts_ ; }
    inline uint64_t read_ts () const { return r_ts_ ; }
  } ;

  //---------------------------------------------------------------------------------------
  /*
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

    Message & 
    operator=( const Message & rhs ) 
    { std::memcpy( content_, rhs.content_, sizeof( content_ ) ) ;
      return *this ;
    }

    inline 
    void 
    set( uint64_t value ) 
    { 
      std::sprintf( content_, "\n%.60lu", value ) ;
    }

    inline 
    uint64_t 
    get() const 
    {
      uint32_t rv = std::strtoul( content_, NULL, 10 ) ;
      return rv ;
    }
  } ;
  */
}}}}

#endif
