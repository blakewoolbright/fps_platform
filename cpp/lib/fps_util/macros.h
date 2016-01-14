#ifndef FPS__UTIL__MACROS__H
#define FPS__UTIL__MACROS__H

  //----------------------------------------------------------------------------------
  // Branch prediction hint - expect conditional to evaluate true. 
  //----------------------------------------------------------------------------------
  #ifndef fps_likely 
  #  define fps_likely(x) __builtin_expect(!!(x), 1)
  #endif 

  //----------------------------------------------------------------------------------
  // Branch prediction hint - expect conditional to evaluate false. 
  //----------------------------------------------------------------------------------
  #ifndef fps_unlikely 
  #  define fps_unlikely(x)   __builtin_expect(!!(x), 0)
  #endif

#endif
