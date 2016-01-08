#ifndef FPS__IPC__SPINLOCK__H
#define FPS__IPC__SPINLOCK__H

#include <atomic>
#include "fps_ipc/ipc_util.h"

namespace fps {
namespace ipc {

  //----------------------------------------------------------------------------
  class SpinLock_Atomic
  {
  private :
    std::atomic_flag lock_ ;

  public:
    //--------------------------------------------------------------------------
    inline
    SpinLock_Atomic() 
      : lock_( ATOMIC_FLAG_INIT ) 
    {}

    //--------------------------------------------------------------------------
    inline
    void 
    lock() 
    {
      for( uint32_t idx = 0 
         ; lock_.test_and_set( std::memory_order_acquire ) 
         ; ++idx 
         ) 
      { ipc::progressive_yield( idx ) ; 
      }
    }

    //--------------------------------------------------------------------------
    inline
    void 
    unlock()     
    {
      lock_.clear( std::memory_order_release ) ;
    }
  } ;

  //----------------------------------------------------------------------------
  class SpinLock_Intrinsic 
  {
  public:
    uint32_t state_ ;

  public:
    //----------------------------------------------------------------------------
    inline SpinLock_Intrinsic() : state_( 0 ) {}

    //----------------------------------------------------------------------------
    inline
    bool 
    try_lock()
    {
      uint32_t old_value = ::__sync_lock_test_and_set( &state_, 1 );
      return ( 0 == old_value ) ;
    }

    //----------------------------------------------------------------------------
    inline
    void 
    lock()
    {
      for( uint32_t counter = 0; !try_lock(); ++counter ) 
      { ipc::progressive_yield( counter ) ;
      }
    }

    //----------------------------------------------------------------------------
    inline
    void 
    unlock()
    {
      ::__sync_lock_release( &state_ ) ;
    }
  } ;

  typedef SpinLock_Intrinsic SpinLock ;

}}

#endif
