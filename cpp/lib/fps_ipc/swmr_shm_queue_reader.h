#ifndef FPS__IPC__SWMR_SHM_QUEUE_READER__H
#define FPS__IPC__SWMR_SHM_QUEUE_READER__H

#include "fps_ipc/swmr_ring_buffer.h"
#include "fps_fs/path.h"
#include "fps_ipc/shared_memory.h"
#include "fps_ipc/mapped_memory.h"
#include "fps_util/fps_util.h" // For fps_likely/unlikely

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
    mutable uint32_t  r_idx_ ;
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
    void close() ;
    
    //------------------------------------------------------------------------
    bool read( T & dest ) const ;

    //------------------------------------------------------------------------
    inline bool    is_open()    const { return impl_ != NULL && shm_map_.is_open() ; }
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
  { close() ;
  }

  //------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  void 
  ShmQueueReader<T,T_Capacity>::
  close()
  {
    if( shm_.is_open() ) 
      shm_.close() ;
    
    if( shm_map_.is_open() ) 
      shm_map_.close() ;

    error_ = 0 ;
    impl_  = NULL ;
    r_idx_ = 0 ;
  }

  //------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  bool
  ShmQueueReader<T,T_Capacity>::
  open( const std::string & shm_q_name ) 
  {
    close() ;

    if( !shm_.open( shm_q_name, ipc::access::Read_Only ) ) 
    { error_ = shm_.last_error() ;
      return false ;
    }
  
    if( !shm_map_.open( shm_, ipc::access::Read_Only ) ) 
    { error_ = errno ;
      shm_.close() ;
      return false ;
    }
  
    // TODO: Should I just close the ipc::SharedMemory instance now?
    impl_ = shm_map_.cast<impl_t>() ;
    if( impl_ == NULL ) 
    { shm_map_.close() ;
      shm_.close() ;  
      return false ;
    }
  
    return true ;
  }
  
  //------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  bool
  ShmQueueReader<T,T_Capacity>::
  read( T & dest ) const
  {
    if( fps_unlikely( NULL == impl_ ) ) 
      return false ;
  
    if( !impl_->read( r_idx_, dest ) ) 
      return false ;

    r_idx_ = impl_->advance( r_idx_ ) ;
    return true ;
  }
}}}

#endif
