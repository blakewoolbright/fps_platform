#ifndef FPS__TIME__TIMER__H
#define FPS__TIME__TIMER__H

#include "fps_time/clock.h"
#include "fps_time/constants.h"
#include <unistd.h>

namespace fps  {
namespace time {

  //-----------------------------------------------------------------------------------------------
  //
  // Timer 
  // Simple stopwatch like timer implementation. 
  // 
  // Example : (Print "hello" every 2 seconds for 10 seconds)
  //
  // time::Timer tm_print ;
  // time::Timer tm_stop  ;
  // tm_print.set_seconds( 2  ) ;
  // tm_stop.set_seconds ( 10 ) ;
  // tm_print.start() ;
  // tm_stop.start() ;
  // while( !tm_stop.expired() ) 
  // { 
  //    if( tm_print.expired() ) 
  //    { std::cout << "hello" << std::endl ;
  //      tm_print.restart() ;
  //    }
  //    ::usleep( 100 ) ;
  // } 
  //
  //
  //-----------------------------------------------------------------------------------------------
  class Timer 
  {
    uint64_t start_  ;  // Epoch nanosecond time that the timer was started.
    uint64_t expiry_ ;  // Expiry time in delta nanoseconds

  public :
    //---------------------------------------------------------------------------------------------
    Timer()
      : start_ ( 0 )
      , expiry_( 0 )
    {}

    //---------------------------------------------------------------------------------------------
    // Construct a timer with a pre-defined expiry interval. 
    //---------------------------------------------------------------------------------------------
    explicit 
    inline 
    Timer( uint64_t nanos )
      : start_ ( 0 )
      , expiry_( nanos )
    {}

    //---------------------------------------------------------------------------------------------
    // Set number of nanos, micros, millis, seconds, minutes before timer expires
    //  
    // TODO: Add additional set_* implementations
    //---------------------------------------------------------------------------------------------
    inline void set        ( uint64_t n_count ) { expiry_ = n_count ; }
    inline void set_seconds( uint64_t s_count ) { set( s_count * time::Nanos_Per_Second ) ; } 

    //---------------------------------------------------------------------------------------------
    inline bool running() const { return start_ != 0 ; }
    inline bool expired() const { return running() && ((Clock::now() - start_) > expiry_ ) ; }

    //---------------------------------------------------------------------------------------------
    inline void stop   () { start_ = 0 ; }
    inline void start  () { start_ = Clock::now() ;  } 
    inline void restart() { start() ; } 
  } ;

}}

#endif

