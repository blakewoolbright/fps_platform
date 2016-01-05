#ifndef FPS__IPC__IPC_UTIL__H
#define FPS__IPC__IPC_UTIL__H

#include <ctime>
#include <sched.h>

namespace fps {
namespace ipc {

  //--------------------------------------------------------------------------------
  namespace access 
  {
    static const uint32_t Read_Only  = 0 ;
    static const uint32_t Read_Write = 1 ;
    static const uint32_t Create     = 2 ;
    static const uint32_t Exclusive  = 4 ;
  } ;

  //--------------------------------------------------------------------------------
  inline
  void
  progressive_yield( uint32_t counter )
  {
    // 1) If the yield counter is close to zero, optimistically retry immediately.
    if( counter < 4 ) 
    {
    }
    // 2) Once we've passed the optimistic retry threshold, start executing an
    //    x86 "pause" instruction before returning.
    else if( weight < 16 ) 
    {
      __asm__( "pause;" ) ;
    }
    // 3) After passing the "pause" threshold, start yielding our timeslice 
    //    before returning.  Additionally, execute this logic any time the
    //    value of counter is odd. 
    else if( weight < 32 || (weight & 1) )
    {
      ::sched_yield() ;
    }
    // 4) After passing the sched_yield() threshold, sleep for one microsecond
    //    every time the counter value is numerically even.
    else
    {
      // g++ -Wextra warns on {} or {0}
      ::timespec timeout ;
      timeout.tv_sec  = 0 ;
      timeout.tv_nsec = 1000 ;

      ::nanosleep( &timeout, 0 ) ;
    }
  }

  namespace detail 
  {
    //--------------------------------------------------------------------------------
    /*
    template<typename T, uint64_t T_Size>
    union PadType 
    {
      static const uint32_t Total_Size = sizeof(T) + (T_Size-(sizeof(T) % T_Size)) ;
      typedef T value_t ;

      value_t value_ ;
      char    pad_[ Total_Size ] ;
    } ;
    */

    //--------------------------------------------------------------------------------
    template<typename T>
    struct 
    alignas( 64 )
    Align64
    {
    public :
      typedef T value_t ;

    private :
      value_t value_ ;

    public :
      inline 
      const value_t & 
      get() const 
      { return value_ ; 
      }

      inline
      void 
      put( const value_t & v ) 
      { value_ = v ;
      }
    } ;
    
    //--------------------------------------------------------------------------------
    template<typename T>
    struct 
    alignas( 32 )
    Align32
    {
    public :
      typedef T value_t ;

    private :
      value_t value_ ;

    public :
      inline 
      const value_t & 
      get() const 
      { return value_ ; 
      }

      inline
      void 
      put( const value_t & v ) 
      { value_ = v ;
      }
    } ;
  }
}}

#endif
