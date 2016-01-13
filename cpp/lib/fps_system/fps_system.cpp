#include <unistd.h>
#include "fps_system/fps_system.h"
#include "fps_string/format.h"

namespace fps {
namespace system {
namespace cpu {

  //----------------------------------------------------------------------
  int64_t 
  l1_cacheline_size() 
  {
    return ::sysconf( _SC_LEVEL1_DCACHE_LINESIZE ) ;
  }

  //----------------------------------------------------------------------
  int64_t 
  l1_cache_size() 
  {
    return ::sysconf( _SC_LEVEL1_DCACHE_SIZE ) ;
  }

}}}
