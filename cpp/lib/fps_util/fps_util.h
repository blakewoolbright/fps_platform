#ifndef FPS__UTIL__H
#define FPS__UTIL__H

namespace fps  {
namespace util {

  //----------------------------------------------------------------------------------
  // Enable/Disable synchronization of glibc & stdc++ based io streams
  // Returns : The previous synchronization state.
  // Note    : Must be called before the program performs it's first i/o operation.
  // Note    : Always affects stdout, stdin, and stderr.
  //----------------------------------------------------------------------------------
  bool set_stdio_synchronization( bool yes_no ) ;

  //----------------------------------------------------------------------------------
  // Likely/Unlikely macros 
  //----------------------------------------------------------------------------------
  #ifndef fps_likely 
  #  define fps_likely(x) __builtin_expect(!!(x), 1)
  #endif 

  #ifndef fps_unlikely 
  #  define fps_unlikely(x)   __builtin_expect(!!(x), 0)
  #endif

}}

#endif

