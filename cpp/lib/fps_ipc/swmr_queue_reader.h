#ifndef FPS__IPC__SWMR_SHM_QUEUE_READER__H
#define FPS__IPC__SWMR_SHM_QUEUE_READER__H

#include "fps_ipc/swmr_ring_buffer.h"
#include "fps_fs/path.h"
#include "fps_ipc/shared_memory.h"
#include "fps_ipc/mapped_memory.h"

namespace fps  {
namespace ipc  {
namespace swmr {

  //--------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  struct ShmQueueReader
  {
  private :
    //------------------------------------------------------------------------
    typedef swmr::RingBuffer<T, T_Capacity> impl_t ;
    static const uint32_t Capacity = T_Capacity ;
    
    //------------------------------------------------------------------------
    ipc::SharedMemory shm_ ;
    ipc::MappedMemory shm_map_ ;
    int32_t           error_ ;
    uint32_t          r_idx_ ;
    const impl_t    * impl_  ;
    
  public :
    //------------------------------------------------------------------------
    ShmQueueReader() ;
    
    //------------------------------------------------------------------------
    ~ShmQueueReader() ;

    //------------------------------------------------------------------------
    // Open the indicated shm queue for reading.  
    // Return true on success, false on failure and sets internal error_ 
    // member to the associated system errno value (if possible).
    //------------------------------------------------------------------------
    bool open( const std::string & shm_q_name ) ;

    //------------------------------------------------------------------------
    bool close() ;
    
    //------------------------------------------------------------------------
    bool read( T & dest ) ;

    //------------------------------------------------------------------------
    inline bool    is_open()    const { return impl_ != NULL ; }
    inline int32_t last_error() const { return error_ ; }
  } ;

  //------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  ShmQueueReader<T,T_Capacity>::
  ShmQueueReader()  
    : error_( 0 ) 
    , r_idx_( 0 ) 
    , impl_ ( NULL ) 
  { 
  }
    
  //------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  ShmQueueReader<T,T_Capacity>::
  ~ShmQueueReader()  
  {
  }

}}}

#endif
