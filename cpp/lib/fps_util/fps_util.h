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

}}

#endif

