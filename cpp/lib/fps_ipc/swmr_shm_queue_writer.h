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
    typedef swmr::detail::RingBuffer<T, T_Capacity> impl_t ;
    static const uint32_t Capacity = T_Capacity ;
    
    //------------------------------------------------------------------------
    ipc::SharedMemory shm_ ;
    ipc::MappedMemory shm_map_ ;
    int32_t           error_ ;
    impl_t          * impl_  ;
    
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
    void close() ;
    
    //------------------------------------------------------------------------
    bool write( const T & dest ) ;

    //------------------------------------------------------------------------
    inline 
    uint32_t 
    write_index() const 
    { return ( impl_ != NULL ) ? impl_->write_index() : 0 ; 
    }

    //------------------------------------------------------------------------
    inline bool     is_open()     const { return impl_ != NULL ; }
    inline int32_t  last_error()  const { return error_ ; }

    //------------------------------------------------------------------------
    // Note: Debug Only
    //------------------------------------------------------------------------
    inline const ipc::SharedMemory & shared_memory() const { return shm_ ; }
    inline const ipc::MappedMemory & mapped_memory() const { return shm_map_ ; }
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
    close() ;
  }

  //------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  void 
  ShmQueueWriter<T,T_Capacity>::
  close()
  {
    if( shm_.is_open() ) 
      shm_.close() ;
    
    if( shm_map_.is_open() ) 
      shm_map_.close() ;

    error_ = 0 ;
    impl_  = NULL ;
  }

  //------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  bool
  ShmQueueWriter<T,T_Capacity>::
  open( const std::string & shm_q_name ) 
  {
    close() ;
    int32_t access_flags = ipc::access::Read_Write 
                         | ipc::access::Create 
                         | ipc::access::Exclusive 
                         ;

    if( !shm_.open( shm_q_name, access_flags ) )
    { error_ = shm_.last_error() ;
      return false ;
    }

    if( !shm_.resize<impl_t>() ) 
    { error_ = shm_.last_error() ;
      shm_.close() ;
      return false ;
    }
  
    if( !shm_map_.open( shm_, access_flags ) ) 
    { error_ = shm_map_.last_error() ;
      shm_.close() ;
      return false ;
    }
  
    // TODO: Should I just close the ipc::SharedMemory instance now?
    impl_ = shm_map_.construct<impl_t>() ;
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
  ShmQueueWriter<T,T_Capacity>::
  write( const T & src ) 
  {
    if( fps_unlikely( NULL == impl_ ) ) 
      return false ; 

    impl_->write( src ) ;
    return true ;
  }

}}}

#endif
