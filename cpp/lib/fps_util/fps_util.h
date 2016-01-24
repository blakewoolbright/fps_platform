#ifndef FPS__UTIL__H
#define FPS__UTIL__H

#include "fps_util/macros.h"
//#include "fps_util/comparators.h"
//#include "fps_util/algorithms.h"
//#include "fps_util/set.h"
#include "fps_util/intrinsics.h"

namespace fps  {
namespace util {

  //----------------------------------------------------------------------------------
  // Enable/Disable synchronization of glibc & stdc++ based io streams
  // Returns : The previous synchronization state.
  // Note    : Must be called before the program performs it's first i/o operation.
  // Note    : Always affects stdout, stdin, and stderr.
  //----------------------------------------------------------------------------------
  bool set_stdio_synchronization( bool yes_no ) ;

}}

#endif

