#define BOOST_TEST_MODULE fps_net

#include "fps_net/address.h"

#include <boost/test/unit_test.hpp>
#include <iostream>

using namespace fps ;

BOOST_AUTO_TEST_CASE( fps_net__address )
{
  net::Address a1( "192.168.1.1", 12345 ) ;

  std::cout << "[ fps::net::Address unit tests ]" << std::endl 
            << "|--[ 1) " << a1.to_string( true ) << " ]" << std::endl 
            << "|" << std::endl ;
  
}


