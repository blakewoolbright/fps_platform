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
  typedef ipc::swmr::ShmQueueReader<msg_t, examples::ipc::swmr::Capacity> reader_t ;

  std::cout << "[ swmr::ShmQueueReader ]" << std::endl ;
  reader_t reader ;
  std::cout << "|--[ Constructed ]" << std::endl << "|" << std::endl ;

  reader.open( examples::ipc::swmr::Queue_Name ) ;
  
  std::cout << "|--[ CPU            => '" << examples::ipc::swmr::Reader_CPU << "' ]" << std::endl 
            << "|--[ open()         <= '" << examples::ipc::swmr::Queue_Name << "' ]" << std::endl 
            << "|--[ is_open()      => '" << (reader.is_open()?"true":"false") << "' ]" << std::endl 
            << "|--[ shm.size()     => '" << reader.shared_memory().size() << "' ]" << std::endl
            << "|--[ shm_map.size() => '" << reader.mapped_memory().size() << "' ]" << std::endl
            << "|--[ last_error()   => '" << reader.last_error() << "' ]" << std::endl 
            << "|" << std::endl 
            ;

  if( !reader.is_open() ) 
  { std::cout << "|--[ ERROR        :: Failed to open reader instance ]" << std::endl 
              << "|" << std::endl ;
    return 1 ;
  }

  system::set_affinity( system::AffinityMask( examples::ipc::swmr::Reader_CPU ) ) ;

  util::signal::set_handler( util::signal::Sig_Int,  interrupt_handler ) ;
  util::signal::set_handler( util::signal::Sig_Term, interrupt_handler ) ;
  
  msg_t       msg ;
  uint32_t    read_count = 0 ;
  time::Timer console_timer ;
  console_timer.set( time::Nanos_Per_Second * 3 ) ;
  console_timer.start() ;
  uint64_t last_delta = 0 ;
  uint64_t last_seq   = 0 ;
  uint64_t mismatches = 0 ;

  while( !exit_flag ) 
  {
    uint64_t loop_ts = time::Clock::now() ;
    if( reader.read( msg ) ) 
    { ++read_count ;
      msg.on_read( loop_ts ) ;
      if( last_seq + 1 != msg.sequence() ) 
        ++mismatches ;
      last_seq   = msg.sequence() ;
      last_delta = msg.read_ts() - msg.write_ts() ;
    }

    if( console_timer.expired() ) 
    { 
      std::cout << "|--[ read_count         => " << read_count     << " ]" << std::endl ;
      std::cout << "|--[ last sequence      => " << msg.sequence() << " ]" << std::endl ;
      std::cout << "|--[ last delta (nanos) => " << last_delta     << " ]" << std::endl ;
      std::cout << "|--[ mismatches         => " << mismatches     << " ]" << std::endl ;
      console_timer.restart() ;
    }
    // else { ::usleep( 10 ) ; }
  }

  return 0 ;
}

/*
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
*/

