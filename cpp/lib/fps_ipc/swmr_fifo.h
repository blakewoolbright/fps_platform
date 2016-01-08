#ifndef FPS__IPC__SWMR_FIFO__H
#define FPS__IPC__SWMR_FIFO__H

#include "fps_ipc/swmr_ring_buffer.h"

namespace fps  {
namespace ipc  {
namespace swmr {

  //--------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  struct Queue
  {
  private :
    //------------------------------------------------------------------------
    typedef swmr::RingBuffer<T, T_Capacity> rb_t ;
    rb_t * rb_ ;

    
    
    
  } ;

}}}

#endif
