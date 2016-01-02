#include "fps_ipc/fps_ipc.h"
#include "shm_fifo.common.h"

using namespace fps ;

//---------------------------------------------------------------------------------------
int 
main( int argc, char * argv[] ) 
{
  std::cout << "[ fps::ipc::RingBuffer_Fixed | Reader Example ]" << std::endl 
            << "|--[ Capacity : " << examples::shm_fifo::Capacity << " ]" << std::endl 
            << "|--[ Shm File : " << examples::shm_fifo::Name     << " ]" << std::endl 
            << std::endl ;

  return 0 ;
}
