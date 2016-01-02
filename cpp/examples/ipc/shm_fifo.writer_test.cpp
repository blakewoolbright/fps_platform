#include "fps_ipc/fps_ipc.h"
#include "shm_fifo.common.h"

using namespace fps ;

//---------------------------------------------------------------------------------------
int 
main( int argc, char * argv[] ) 
{
  typedef examples::shm_fifo::Message msg_t ;
  typedef ipc::RingBuffer_Fixed<msg_t, examples::shm_fifo::Capacity> fifo_t ;

  std::cout << "[ fps::ipc::RingBuffer_Fixed | Writer Example ]" << std::endl 
            << "|--[ Capacity : " << examples::shm_fifo::Capacity << " ]" << std::endl 
            << "|--[ Shm File : " << examples::shm_fifo::Name     << " ]" << std::endl 
            << "|" << std::endl ;

  fs::Path shm_path( "/dev/shm", examples::shm_fifo::Name ) ;
  if( shm_path.exists() ) 
    shm_path.rm() ;
  
  ipc::ShmRegion shm ;
  fifo_t       * fifo_ptr = shm.open<fifo_t>( examples::shm_fifo::Name ) ;
  if( !fifo_ptr )
  { std::cout << "|--[ Error    : errno=" << shm.last_error() << " ]" << std::endl 
              << "|" << std::endl ;
    return 1 ;
  }
  std::cout << "|--[ open() : success ]" << std::endl ;

  msg_t msg ;
  for( uint32_t idx = 0 ; idx < fifo_t::Capacity ; ++idx ) 
  { msg.set( idx + 1 ) ;
    fifo_ptr->push( msg ) ;
  }  

  return 0 ;
}
