#define BOOST_TEST_MODULE fps_ipc

#include "fps_ipc/thread_fifo.h"
#include "fps_ipc/ipc_util.h"
#include "fps_string/format.h"
#include "fps_util/signal.h"
#include "fps_time/fps_time.h"

#include <boost/test/unit_test.hpp>
#include <thread>
#include <iostream>

using namespace fps ;

//--------------------------------------------------------------------------------
struct TestMessage
{
  uint64_t ts_  ;
  uint64_t seq_ ;
  bool     stop_ ;

  TestMessage() : ts_( 0 ), seq_( 0 ), stop_( 0 ) {}
  TestMessage( uint64_t ts, uint64_t seq, bool stop ) 
    : ts_  ( ts ) 
    , seq_ ( seq ) 
    , stop_( stop )
  {}
  ~TestMessage() {}

  TestMessage & 
  operator=( const TestMessage & rhs ) 
  { ts_   = rhs.ts_ ;
    seq_  = rhs.seq_ ;
    stop_ = rhs.stop_ ;
    return *this ;
  }
  
  inline bool     valid() const { return ts_ > 0 && seq_ > 0 ; } 
  inline uint64_t ts   () const { return ts_ ; }
  inline uint64_t seq  () const { return seq_ ; }
  inline bool     stop () const { return stop_ ; }
} ;

//--------------------------------------------------------------------------------
typedef ipc::ThreadFifo<TestMessage> fifo_t ;
static const uint32_t ThreadFifo_Test_Messages = 1000 ;

//--------------------------------------------------------------------------------
void 
run_reader_thread( fifo_t * fifo ) 
{
  TestMessage tm ;
  uint32_t r_count = 1 ;
  uint32_t r_fail  = 0 ;

  std::vector<TestMessage> r_vec ;
  r_vec.reserve( 1024 ) ;
  
  bool rdr_exit_flag = false ;
  while( !rdr_exit_flag )
  {
    while( fifo->read( tm ) ) 
    { 
      ++r_count ;
      r_vec.push_back( tm ) ;
      if( r_vec.back().stop() ) 
        rdr_exit_flag = true ;
    }

    // ::usleep( 1 ) ;
  }
  
  BOOST_CHECK_MESSAGE
  ( !r_vec.empty()
  , string::sprintf( "\n\tipc::ThreadFifo :: Reader :: No messages received during test" )
  ) ;

  uint64_t seq_next = r_vec[ 0 ].seq() + 1 ;
  for( uint32_t idx = 1 ; idx < r_vec.size() ; ++idx ) 
  { 
    /*
    std::cout << string::sprintf( "|  |--[ %3lu :: %lu :: %s ]"
                                , r_vec[idx].seq()
                                , r_vec[idx].ts() 
                                , (r_vec[idx].stop()?"stop":"go")
                                ) << std::endl ;
    */
    BOOST_CHECK_MESSAGE
    ( r_vec[idx].seq() == seq_next
    , string::sprintf( "\n\tipc::ThreadFifo :: Reader :: Message sequencing error %lu != %lu"
                     , r_vec[ idx ].seq(), seq_next
                     ) 
    ) ;
    ++seq_next ;
  }

  std::cout << "[ Reader Summary ]" << std::endl 
            << "|--[ r_count :: " << r_count << " ]" << std::endl
            << "|--[ r_fail  :: " << r_fail  << " ]" << std::endl 
            << "|" << std::endl ;
}

//--------------------------------------------------------------------------------
void 
run_writer_thread( fifo_t * fifo ) 
{
  time::Timer w_timer( time::Nanos_Per_Micro * 1 ) ;

  uint32_t w_max  = ThreadFifo_Test_Messages ;
  uint32_t w_seq  = 1 ;
  uint32_t w_wait = 0 ;
  
  w_timer.start() ;
  while( true ) 
  {
    // if( w_timer.expired() ) 
    {
      bool stop_flag = (w_seq >= w_max) ;
      if( fifo->write( TestMessage( time::Clock::now(), w_seq, stop_flag ) ) )
      {
        ++w_seq ;

        if( stop_flag ) 
          break ;
      }
      else
        ++w_wait ;
      
      // w_timer.restart() ;
    }
    // ::usleep( 1 ) ;
  }
  
  std::cout << "[ Writer Summary ]" << std::endl 
            << "|--[ w_count :: " << (w_seq - 1) << " ]" << std::endl
            << "|--[ w_wait  :: " << w_wait  << " ]" << std::endl
            << std::endl << std::flush ;
}

struct AlignTester
{
  uint64_t a_ ;
  uint64_t b_ ;
  uint64_t c_ ;
  uint64_t d_ ;

  void *   e_ ;
} ;

BOOST_AUTO_TEST_CASE( fps_ipc__fifo )
{
  fifo_t * fifo = new fifo_t( 128 ) ;
  
  std::cout << "[ ipc::ThreadFifo<> unit test ]" << std::endl 
            << "|--[ fifo.size()     :: " << fifo->size() << " ]" << std::endl 
            << "|--[ fifo.capacity() :: " << fifo->capacity() << " ]" << std::endl 
            << "|" << std::endl ;

  std::thread reader_thread( run_reader_thread, fifo ) ;
  std::thread writer_thread( run_writer_thread, fifo ) ;

  reader_thread.join() ;
  writer_thread.join() ;

  typedef ipc::detail::Align64<AlignTester> test_t ;
  std::cout << "sizeof ( test_t )      : " << sizeof ( test_t ) << std::endl 
            << "alignof( test_t )      : " << alignof( test_t ) << std::endl 
            << "sizeof ( AlignTester ) : " << sizeof ( AlignTester ) << std::endl 
            << "alignof( AlignTester ) : " << alignof( AlignTester ) << std::endl 
            << std::endl ;
}

