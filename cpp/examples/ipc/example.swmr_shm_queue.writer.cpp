#include "fps_ipc/swmr_shm_queue.h"
#include "fps_util/signal.h"
#include "fps_time/timer.h"
#include "swmr_shm_queue.common.h"
#include <iostream>

using namespace fps ;

//---------------------------------------------------------------------------------------
bool exit_flag = false ;

//---------------------------------------------------------------------------------------
void interrupt_handler( int ) 
{ 
  exit_flag = true ;
}

//---------------------------------------------------------------------------------------
int 
main( int argc, char * argv[] ) 
{
  typedef examples::ipc::swmr::Message msg_t ;
  typedef ipc::swmr::ShmQueueWriter<msg_t, examples::ipc::swmr::Capacity> writer_t ;

  fs::Path shm_path( "/dev/shm", examples::ipc::swmr::Queue_Name ) ;
  if( shm_path.exists() ) 
    shm_path.rm() ;
  
  std::cout << "[ swmr::ShmQueueWriter ]" << std::endl ;
  writer_t writer ;
  std::cout << "|--[ Constructed ]" << std::endl << "|" << std::endl ;

  writer.open( examples::ipc::swmr::Queue_Name ) ;

  std::cout << "|--[ CPU            => '" << examples::ipc::swmr::Writer_CPU << "' ]" << std::endl 
            << "|--[ open()         <= '" << examples::ipc::swmr::Queue_Name << "' ]" << std::endl 
            << "|--[ is_open()      => '" << (writer.is_open()?"true":"false") << "' ]" << std::endl 
            << "|--[ shm.size()     => '" << writer.shared_memory().size() << "' ]" << std::endl
            << "|--[ shm_map.size() => '" << writer.mapped_memory().size() << "' ]" << std::endl
            << "|--[ last_error()   => '" << writer.last_error() << "' ]" << std::endl 
            << "|" << std::endl 
            ;

  if( !writer.is_open() ) 
  { std::cout << "|--[ ERROR        :: Failed to open writer instance ]" << std::endl 
              << "|" << std::endl ;
    return 1 ;
  }

  system::set_affinity( system::AffinityMask( examples::ipc::swmr::Writer_CPU ) ) ;

  util::signal::set_handler( util::signal::Sig_Int,  interrupt_handler ) ;
  util::signal::set_handler( util::signal::Sig_Term, interrupt_handler ) ;

  msg_t msg ;
  uint64_t write_count = 1 ;
  time::Timer console_timer ;
  console_timer.set( time::Nanos_Per_Second * 3 ) ;
  console_timer.start() ;

  while( !exit_flag ) 
  {
    uint64_t now_ts = time::Clock::now() ;
    msg.on_write( write_count, now_ts ) ;
    if( writer.write( msg ) ) 
      ++write_count ;

    if( console_timer.expired( now_ts ) ) 
    { std::cout << "|--[ write_count        => " << write_count    << " ]" << std::endl ;
      console_timer.restart() ;
    }
    else 
      ::usleep( 1 ) ;
  }

  return 0 ;
}
/*
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
*/

