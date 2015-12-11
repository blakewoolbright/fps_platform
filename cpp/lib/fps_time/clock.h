#ifndef FPS__TIME__CLOCK__H
#define FPS__TIME__CLOCK__H

#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include "fps_time/constants.h"

namespace fps  {
namespace time {

  //------------------------------------------------------------------------------------------
  struct Clock
  {
    //----------------------------------------------------------------------------------------
    static inline uint64_t now() 
    { 
      struct ::timespec ts ;
      return ( 0 == ::clock_gettime( CLOCK_REALTIME, &ts ) ) 
             ? ((ts.tv_sec * Nanos_Per_Second) + ts.tv_nsec)
             : 0 
             ;
    }
  } ;

}}

#endif

