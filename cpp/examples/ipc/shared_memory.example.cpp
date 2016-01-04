#include "fps_ipc/shared_memory.h"
#include "fps_ipc/mapped_memory.h"

using namespace fps ;

//---------------------------------------------------------------------------------------
int 
main( int argc, char * argv[] ) 
{
  ipc::SharedMemory shm ; 
  shm.open( "fps_ipc.shared_memory.example", ipc::access::Read_Write | ipc::access::Create ) ;

  return 0 ;
}
