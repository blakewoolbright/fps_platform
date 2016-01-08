#include "fps_ipc/swmr_ring_buffer.h"
#include "fps_ipc/shared_memory.h"
#include "fps_ipc/mapped_memory.h"

#include "swmr_shm_queue.common.h"

using namespace fps ;

//---------------------------------------------------------------------------------------
int 
main( int argc, char * argv[] ) 
{
  typedef examples::ipc::swmr::Message msg_t ;
  typedef ipc::swmr::RingBuffer<msg_t, examples::ipc::swmr::Capacity> rb_t ;

  fs::Path shm_path( "/dev/shm", examples::ipc::swmr::Queue_Name ) ;
  if( !shm_path.exists() ) 
  { std::cout << "Error :: Failed to find shm segment @ '" << shm_path.str() << "'" << std::endl 
              << std::endl ;
    return 1 ;
  }

  ipc::SharedMemory shm ;
  if( !shm.open( shm_path.leaf(), ipc::access::Read_Only ) ) 
  { std::cout << "Error :: Failed to open shared memory segment w/ name '" 
              << shm_path.str() << "', capacity: " << rb_t::Capacity << std::endl 
              << std::endl ;
    return 1 ;
  }
  
  ipc::MappedMemory shm_map ; 
  if( !shm_map.open( shm, ipc::access::Read_Only ) ) 
  { std::cout << "Error :: Failed to create read-only memory map on shared memory segment" << std::endl 
              << std::endl ;
    return 1 ;
  }

  std::cout << "[ fps::examples::ipc::swmr_queue - reader ]" << std::endl   
            << "|--[ rb_t::Capacity : " << rb_t::Capacity << " ]" << std::endl 
            << "|--[ shm.size()     : " << shm.size() << " ]" << std::endl 
            << "|--[ MMap.size()    : " << shm_map.size() << " ]" << std::endl 
            << "|--[ MMap.is_open() : " << (shm_map.is_open()?"true":"false") << " ]" << std::endl 
            << "|--[ sizeof( rb_t ) : " << sizeof( rb_t ) << " ]" << std::endl 
            << "|" << std::endl ;
  
  const rb_t * swmr_rb = shm_map.cast<rb_t>() ;

  if( !swmr_rb ) 
  { std::cout << "Error :: Failed to construct swmr_queue in mapped shm segment (errno: " << errno << ")" << std::endl ;
    return 1 ;
  }
  
  std::cout << "|--[ rb_t::Capacity         : " << rb_t::Capacity << " ]" << std::endl 
            << "|--[ swmr_rb->write_index() : " << swmr_rb->write_index() << " ]" << std::endl 
            << "|" << std::endl ;

  msg_t msg ;
  uint32_t idx = 0 ;
  for( ; idx < rb_t::Capacity ; ++idx ) 
  { 
    if( !swmr_rb->read( idx, msg ) ) 
      break ;
  
    std::cout << "|--[ " << idx << " : " << msg.get() << " ]" << std::endl ;
  }
  std::cout << "|" << std::endl 
            << "|--[ Reads : " << idx + 1 << " ]" << std::endl 
            << "|" << std::endl ;

  std::cout << std::endl ;
  
  return 0 ;
}

