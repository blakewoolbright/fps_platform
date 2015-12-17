#define BOOST_TEST_MODULE fps_net

#include "fps_net/address.h"

#include <boost/test/unit_test.hpp>
#include <iostream>

using namespace fps ;

BOOST_AUTO_TEST_CASE( fps_net__address )
{
  std::vector<std::string> test_addrs = { "192.168.1.1:12345"
                                        , "tcp://192.168.1.1:27501"
                                        , "http://192.168.254.10:27502/dir/script.ext?var=value"
                                        , "http://www.google.com"
                                        , "https://www.google.com:6666"
                                        } ;
                                         

  std::cout << "[ fps::net::Address unit tests ]" << std::endl ;
  for( auto itr = test_addrs.begin() ; itr != test_addrs.end() ; ++itr ) 
  {
    net::Address addr( *itr ) ;
    std::cout << "|--[ '" << *itr << " :: " << addr.to_string( true ) << " ]" << std::endl ;
  }
  std::cout << "|" << std::endl ;

  
  std::cout << std::endl ;
}


