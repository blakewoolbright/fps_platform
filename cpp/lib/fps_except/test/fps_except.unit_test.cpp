#define BOOST_TEST_MODULE fps_exception__test

#include <boost/test/unit_test.hpp>
#include "fps_except/fps_except.h"
#include "fps_time/fps_time.h"
#include <iostream>
#include <vector>

using namespace fps ;

void throw__runtime_error( const std::string & msg ) 
{
  throw except::RuntimeError( "%lu : %s", 987654321, msg.c_str() ) ;
}

BOOST_AUTO_TEST_CASE( fps_exception )
{
  std::cout << "[ fps_exception : Testing fps::RuntimeError ]" << std::endl     
            << "|" << std::endl ;

  bool success = false ;
  try 
  { throw__runtime_error( "fps::RuntimeError - Thrown from fps_exception unit test" ) ;
    success = false ;
  }
  catch( const std::exception & exc ) 
  { 
    std::cout << "|--{ CATCH => " << exc.what() << " }" << std::endl 
              << "|" << std::endl ;
    success = true ;
  }
  catch( ... ) 
  { std::cout << "|--{ CATCH => Generic C++ Exception }" << std::endl 
              << "|" << std::endl ;
    success = false ;
  }
  BOOST_CHECK_MESSAGE( success, "fps_exception : Failed to catch fps::RuntimeError" ) ;

}
