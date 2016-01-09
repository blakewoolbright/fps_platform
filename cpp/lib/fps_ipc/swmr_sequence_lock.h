#ifndef FPS__IPC__SWMR__SEQUENCE_LOCK__H
#define FPS__IPC__SWMR__SEQUENCE_LOCK__H

#include "fps_ipc/spinlock.h"
#include <cstdint>
#include <atomic>

namespace fps {
namespace ipc {
namespace swmr {

  //----------------------------------------------------------------------------
  // Synchronization lock for lossy single writer/multi-reader designs.
  //----------------------------------------------------------------------------
  class SequenceLock
  {
  private :
    std::atomic<uint64_t> state_ ;
  
  public :
    //--------------------------------------------------------------------------
    inline
    SequenceLock() 
      : state_( 0 )
    {}
    
    //--------------------------------------------------------------------------
    inline 
    void
    write_begin() 
    {
      state_.fetch_add( 1, std::memory_order_acquire ) ;
    }

    //--------------------------------------------------------------------------
    inline 
    void
    write_end() 
    {
      state_.fetch_add( 1, std::memory_order_release ) ;
    }

    //--------------------------------------------------------------------------
    inline 
    uint64_t 
    read_begin() const
    {
      for( ;; ) 
      { uint64_t state = state_.load( std::memory_order_acquire ) ;
        if( (state & 1) == 0 )
        { return state ;
        }
      }
    }

    //--------------------------------------------------------------------------
    inline 
    bool 
    read_end( uint64_t begin_state ) const
    {
      std::atomic_thread_fence( std::memory_order_acquire ) ;
      uint64_t state = state_.load( std::memory_order_relaxed ) ;
      return state == begin_state ;
    }
  } ;

}}}

#endif

