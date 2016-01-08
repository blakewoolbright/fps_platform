#include "fps_ipc/fps_ipc.h"
#include "shm_fifo.common.h"

using namespace fps ;

//---------------------------------------------------------------------------------------
int 
main( int argc, char * argv[] ) 
{
  typedef examples::shm_fifo::Message msg_t ;
  typedef ipc::RingBuffer_Fixed<msg_t, examples::shm_fifo::Capacity> fifo_t ;

  std::cout << "[ fps::ipc::RingBuffer_Fixed | Reader Example ]" << std::endl 
            << "|--[ Capacity : " << examples::shm_fifo::Capacity << " ]" << std::endl 
            << "|--[ Shm File : " << examples::shm_fifo::Name     << " ]" << std::endl 
            << std::endl ;

  fs::Path shm_path( "/dev/shm", examples::shm_fifo::Name ) ;
  if( !shm_path.exists() ) 
  { std::cout << "|--[ Error  : Shm region '" << shm_path.str() << "' not found ]" << std::endl 
              << "|" << std::endl ;
    return 1 ;
  }

  ipc::ShmRegion shm ;
  fifo_t       * fifo_ptr = shm.open<fifo_t>( examples::shm_fifo::Name, ipc::Read_Write ) ;
  if( !fifo_ptr )
  { std::cout << "|--[ Error  : open() : (errno=" << shm.last_error() << ") ]" << std::endl 
              << "|" << std::endl ;
    return 1 ;
  }
  std::cout << "|--[ R_Idx  : " << fifo_ptr->read_index()  << " ]" << std::endl 
            << "|--[ W_Idx  : " << fifo_ptr->write_index() << " ]" << std::endl ;

  msg_t msg ;
  while( fifo_ptr->pop( msg ) ) 
  {
    std::cout << "|--[ pop()  : " << msg.get() << " ]" << std::endl ;
  }

  return 0 ;
}
#if 0

  ipc::SharedMemory shm ;
  shm.open( "/dev/shm/fps.test_segment.spsc_q", ipc::Read_Write ) ;
    
  ipc::MappedMemory shm_control = shm_segment.mmap<ipc::ShmFifoControl>() ;
  ipc::MappedMemory shm_content = shm_segment.mmap<ipc::ShmFifoQueue>() ;
  
  ipc::MappedMemory<ipc::ShmFifoControl> fifo_ctl_block( 
  ipc::MappedMemory<ipc::ShmFifoContent> fifo_cnt_block( ;

  shm_fifo_meta.



#endif
