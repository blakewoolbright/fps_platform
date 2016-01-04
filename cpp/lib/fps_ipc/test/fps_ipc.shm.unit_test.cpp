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
            << "[ ipc::SharedMemory unit tests ]" 
            << std::endl ;


  {
    fs::Path test_path( "/dev/shm/fps_ipc.shm.unit_test" ) ;
    uint32_t test_size( 4096 ) ;

    if( test_path.exists() ) 
    { BOOST_CHECK_MESSAGE
      ( test_path.rm() 
      , string::sprintf( "\n\tipc::SharedMemory :: Failed to remove dangling shm file '/dev/shm/%s'"
                       , test_path.c_str()
                       ) 
      ) ;
    }

    ipc::SharedMemory shm ;
    BOOST_CHECK_MESSAGE
    ( !shm.is_writable() 
    , string::sprintf( "\n\tipc::SharedMemory :: Member 'is_writable()' should fail prior to init." )
    ) ;

    BOOST_CHECK_MESSAGE
    ( !shm.is_readable() 
    , string::sprintf( "\n\tipc::SharedMemory :: Member 'is_readable()' should fail prior to init." )
    ) ;

    BOOST_CHECK_MESSAGE
    ( shm.size() == 0
    , string::sprintf( "\n\tipc::SharedMemory :: Member 'size()' should return zero prior to init." ) 
    ) ;
  
    //
    // Create the shared memory region
    //
    try 
    { shm.open( test_path.leaf(), ipc::access::Create | ipc::access::Read_Write ) ;
    }
    catch( const std::exception & exc ) 
    { std::cout << "Exception :: " << exc.what() << std::endl << std::endl ;
    }
  
    BOOST_CHECK_MESSAGE
    ( shm.is_open() 
    , string::sprintf( "\n\tipc::SharedMemory :: Failed to open/create shared memory region '%s'"
                     , test_path.c_str() 
                     ) 
    ) ;
    std::cout << "|--[ name           : " << shm.name() << " ]" << std::endl ;
    std::cout << "|--[ size (initial) : " << shm.size() << " ]" << std::endl ;

    BOOST_CHECK_MESSAGE
    ( shm.resize( test_size ) 
    , string::sprintf( "\n\tipc::SharedMemory :: Failed to resize shared memory region '%s' to %u bytes"
                     , test_path.c_str() 
                     , test_size
                     ) 
    ) ;
    std::cout << "|--[ size (adjusted) : " << shm.size() << " ]" << std::endl ;
  }
}

