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
  shm_path.rm() ;

  ipc::SharedMemory shm ;
  if( !shm.open( shm_path.leaf(), ipc::access::Create | ipc::access::Read_Write ) ) 
  { std::cout << "Error :: Failed to open shared memory segment w/ name '" 
              << shm_path.str() << "', capacity: " << rb_t::Capacity << std::endl 
              << std::endl ;
    return 1 ;
  }
  
  if( !shm.resize<rb_t>() ) 
  { std::cout << "Error :: Failed to resize shm segment to " << sizeof( rb_t ) << " bytes" << std::endl 
              << std::endl ;
    return 1 ;
  }

  ipc::MappedMemory shm_map ; 
  if( !shm_map.open( shm, ipc::access::Read_Write ) ) 
  { std::cout << "Error :: Failed to create read-write memory map on shared memory segment" << std::endl 
              << std::endl ;
    return 1 ;
  }

  std::cout << "[ fps::examples::ipc::swmr_queue - writer ]" << std::endl   
            << "|--[ rb_t::Capacity : " << rb_t::Capacity << " ]" << std::endl 
            << "|--[ shm.size()     : " << shm.size() << " ]" << std::endl 
            << "|--[ MMap.size()    : " << shm_map.size() << " ]" << std::endl 
            << "|--[ MMap.is_open() : " << (shm_map.is_open()?"true":"false") << " ]" << std::endl 
            << "|--[ sizeof( rb_t ) : " << sizeof( rb_t ) << " ]" << std::endl 
            << "|" << std::endl ;
  
  rb_t * swmr_rb = shm_map.construct<rb_t>() ;

  if( !swmr_rb ) 
  { std::cout << "Error :: Failed to construct swmr_queue in mapped shm segment (errno: " << errno << ")" << std::endl ;
    return 1 ;
  }
  
  std::cout << "|--[ rb_t::Capacity         : " << rb_t::Capacity << " ]" << std::endl 
            << "|--[ swmr_rb->write_index() : " << swmr_rb->write_index() << " ]" << std::endl 
            << "|" << std::endl ;

  msg_t msg ;
  for( uint32_t value = 1 ; value < rb_t::Capacity ; ++value ) 
  { msg.set( value ) ;
    swmr_rb->write( msg ) ;
  }

  ::usleep( 1000 ) ;

  std::cout << std::endl ;
  
  return 0 ;
}

