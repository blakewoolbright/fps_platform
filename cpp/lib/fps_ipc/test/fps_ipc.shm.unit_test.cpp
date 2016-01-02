#define BOOST_TEST_MODULE fps_ipc__shared_memory

#include "fps_ipc/fps_ipc.h"
#include "fps_string/format.h"
#include "fps_util/signal.h"
#include "fps_time/fps_time.h"
#include "fps_fs/path.h"

#include <boost/test/unit_test.hpp>
#include <thread>
#include <iostream>

using namespace fps ;

BOOST_AUTO_TEST_CASE( fps_ipc__shm__basics )
{
  /*
  struct AlignTester
  {
    uint64_t a_ ;
    uint64_t b_ ;
    uint64_t c_ ;
    uint64_t d_ ;

    void *   e_ ;
    uint64_t f_[3] ;
    char     x_ ;
  } ;

  typedef ipc::detail::Align64<AlignTester> test_t ;
  std::cout << "sizeof ( test_t )      : " << sizeof ( test_t ) << std::endl 
            << "alignof( test_t )      : " << alignof( test_t ) << std::endl 
            << "sizeof ( AlignTester ) : " << sizeof ( AlignTester ) << std::endl 
            << "alignof( AlignTester ) : " << alignof( AlignTester ) << std::endl 
            << std::endl ;
  */

  std::cout << std::endl 
            << "[ ipc::ShmRegion unit tests ]" 
            << std::endl ;


  {
    fs::Path test_path( "/dev/shm/fps_ipc.shared_memory.unit_test" ) ;
    uint32_t test_size( 4096 ) ;

    if( test_path.exists() ) 
    { BOOST_CHECK_MESSAGE
      ( test_path.rm() 
      , string::sprintf( "\n\tipc::ShmRegion :: Failed to remove dangling shm file '/dev/shm/%s'"
                       , test_path.c_str()
                       ) 
      ) ;
    }

    ipc::ShmRegion shm ;
    BOOST_CHECK_MESSAGE
    ( !shm.is_writable() 
    , string::sprintf( "\n\tipc::ShmRegion :: Member 'is_writable()' should fail prior to init." )
    ) ;

    BOOST_CHECK_MESSAGE
    ( !shm.is_readable() 
    , string::sprintf( "\n\tipc::ShmRegion :: Member 'is_readable()' should fail prior to init." )
    ) ;

    BOOST_CHECK_MESSAGE
    ( shm.size() == 0
    , string::sprintf( "\n\tipc::ShmRegion :: Member 'size()' should return zero prior to init." ) 
    ) ;

    if( !shm.try_create( test_path.leaf(), test_size ) )
    { std::cout << "Error creating shared memory file: '" << shm.name() << "', errno: " << shm.last_error() << std::endl ;
    }
    
    std::cout << "|--[ size : " << shm.size() << " ]" << std::endl ;
  }
    
}

