#ifndef FPS__SYSTEM__SYSTEM__H
#define FPS__SYSTEM__SYSTEM__H

#include <cstdint>
#include <thread>
#include <sched.h>
#include <unistd.h>
#include <cstring>
#include "fps_system/cpu_affinity_mask.h"

namespace fps {
namespace system {

  /* TODO:

  current user
  current group
  installed ram
  X installed cpu cores 
  network interfaces
  */

  //--------------------------------------------------------------------------
  namespace cpu 
  {
    //--------------------------------------------------------------------------
    int64_t l1_cache_size() ;  

    //--------------------------------------------------------------------------
    int64_t l1_cacheline_size() ;  

    //------------------------------------------------------------------------
    inline
    uint32_t
    core_count() 
    { 
      return std::thread::hardware_concurrency() ;
      // return ::sysconf( _SC_NPROCESSORS_ONLN ) ;
    }

    //------------------------------------------------------------------------
    inline 
    bool
    set_affinity( const AffinityMask & mask ) 
    {
      int32_t rv = ::pthread_setaffinity_np( ::pthread_self()
                                           , sizeof( cpu_set_t )
                                           , &(mask.cpu_set())
                                           ) ;
      return ( rv == 0 ) ;
    }

    //------------------------------------------------------------------------
    inline
    AffinityMask 
    get_affinity() 
    {
      AffinityMask rv ;
      if( 0 != ::pthread_getaffinity_np( ::pthread_self()
                                       , sizeof( cpu_set_t ) 
                                       , &(rv.cpu_set())
                                       ) 
        ) 
      { rv.reset() ; 
      }

      return rv ;
    }
  }
}}

#endif
