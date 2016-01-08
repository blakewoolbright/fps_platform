#ifndef FPS__IPC__SWMR_SHM_QUEUE_WRITER__H
#define FPS__IPC__SWMR_SHM_QUEUE_WRITER__H

#include "fps_ipc/swmr_ring_buffer.h"
#include "fps_fs/path.h"
#include "fps_ipc/shared_memory.h"
#include "fps_ipc/mapped_memory.h"

namespace fps  {
namespace ipc  {
namespace swmr {

  //--------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  struct ShmQueueWriter
  {
  private :
    //------------------------------------------------------------------------
    typedef swmr::RingBuffer<T, T_Capacity> impl_t ;
    static const uint32_t Capacity = T_Capacity ;
    
    //------------------------------------------------------------------------
    ipc::SharedMemory shm_ ;
    ipc::MappedMemory shm_map_ ;
    int32_t           error_ ;
    const impl_t    * impl_  ;
    
  public :
    //------------------------------------------------------------------------
    ShmQueueWriter() ;
    
    //------------------------------------------------------------------------
    ~ShmQueueWriter() ;

    //------------------------------------------------------------------------
    // Open the indicated shm queue for reading.  
    // Return true on success, false on failure and sets internal error_ 
    // member to the associated system errno value (if possible).
    //------------------------------------------------------------------------
    bool open( const std::string & shm_q_name ) ;

    //------------------------------------------------------------------------
    bool close() ;
    
    //------------------------------------------------------------------------
    bool write( const T & dest ) ;

    //------------------------------------------------------------------------
    inline uint32_t write_index() const 
    { return ( impl_ != NULL ) ? impl_->write_index() : 0 ; 
    }

    //------------------------------------------------------------------------
    inline bool     is_open()     const { return impl_ != NULL ; }
    inline int32_t  last_error()  const { return error_ ; }
  } ;

  //------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  ShmQueueWriter<T,T_Capacity>::
  ShmQueueWriter()  
    : error_( 0 ) 
    , impl_ ( NULL ) 
  { 
  }
    
  //------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  ShmQueueWriter<T,T_Capacity>::
  ~ShmQueueWriter()  
  {
  }

}}}

#endif
