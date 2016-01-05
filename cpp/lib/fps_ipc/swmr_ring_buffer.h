#ifndef FPS__IPC__SWMR_RING_BUFFER__H
#define FPS__IPC__SWMR_RING_BUFFER__H

#include "fps_ipc/spinlock.h"

#include "fps_ipc/sequenced_spinlock.h"
#include "fps_ipc/swmr_spinlock.h"

namespace fps {
namespace ipc {
namespace swmr {

  //--------------------------------------------------------------------------
  template<typename T>
  class Slot 
  {
  private :
    ipc::swmr::SpinLock lock_ ;
    T                   data_ ;

  public : 
    inline Slot() : lock_(), data_() {}
    
  } ;

}}}

#endif
