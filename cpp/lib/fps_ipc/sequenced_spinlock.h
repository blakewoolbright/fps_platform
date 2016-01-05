#ifndef FPS__IPC__SEQUENCED_SPINLOCK__H
#define FPS__IPC__SEQUENCED_SPINLOCK__H

#include "fps_ipc/spinlock.h"
#include <cstdint>

namespace fps {
namespace ipc {

  //----------------------------------------------------------------------------
  // Synchronization lock for lossy single writer/multi-reader designs.
  //----------------------------------------------------------------------------
  class SequencedSpinLock
  {
  private :
    SpinLock              lock_  ;
    std::atomic<uint64_t> state_ ;
  
  public :
    //--------------------------------------------------------------------------
    inline
    SequencedSpinLock() 
      : lock_ () 
      , state_( 0 )
    {}
    
    //--------------------------------------------------------------------------
    inline 
    void
    write_begin() 
    {
      lock_.lock() ;
      state_.fetch_add( 1, std::memory_order_acquire ) ;
    }

    //--------------------------------------------------------------------------
    inline 
    void
    write_end() 
    {
      lock_.lock() ;
      state_.fetch_add( 1, std::memory_order_acquire ) ;
    }

    //--------------------------------------------------------------------------
    inline 
    uint64_t 
    read_begin() 
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
    read_end( uint64_t begin_state ) 
    {
      std::atomic_thread_fence( std::memory_order_acquire ) ;
      uint64_t state = state_.load( std::memory_order_relaxed ) ;
      return state == begin_state ;
    }
  } ;

}}

#endif
