#ifndef FPS__IPC__SPINLOCK__H
#define FPS__IPC__SPINLOCK__H

#include <atomic>

namespace fps {
namespace ipc {

  //----------------------------------------------------------------------------
  class SpinLock 
  {
  private :
    std::atomic_flag lock_ ;

  public:
    //--------------------------------------------------------------------------
    inline
    SpinLock() 
      : lock_( ATOMIC_FLAG_INIT ) 
    {}

    //--------------------------------------------------------------------------
    inline
    void 
    lock() 
    {
      while( lock_.test_and_set( std::memory_order_acquire ) ) { ; }
    }

    //--------------------------------------------------------------------------
    inline
    void 
    unlock()     
    {
      lock_.clear( std::memory_order_release ) ;
    }
  };

}}

#endif
