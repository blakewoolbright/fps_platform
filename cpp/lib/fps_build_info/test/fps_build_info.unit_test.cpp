#define BOOST_TEST_MODULE fps_build_info__unit_tests

#include "fps_json/fps_json.h"
#include "fps_string/fps_string.h"
#include "fps_build_info/fps_build_info.h"
#include "fps_util/singleton.h"

#include <boost/test/unit_test.hpp>
#include <iostream>

using namespace fps ;

//---------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_build_info__basics ) 
{
  std::cout << "[ util::BuildInfo unit tests ]" << std::endl ;
  BOOST_CHECK_MESSAGE( util::BuildInfo::Is_Available
                     , "\n\tfps::util::BuildInfo :: Embedded build information not present in binary" 
                     ) ;
  
  util::BuildInfo & bi = util::Singleton<util::BuildInfo>::instance() ;
  BOOST_CHECK_MESSAGE( bi.valid() 
                     , "\n\tfps::util::BuildInfo :: Build info present but invalid"
                     ) ;

  std::cout << "|--[ build_info.tag       : '" << bi.tag() << "'" << std::endl 
            << "|--[ build_info.timestamp : '" << bi.timestamp() << "'" << std::endl 
            << "|--[ build_info.user      : '" << bi.user() << "'" << std::endl 
            << "|--[ build_info.host      : '" << bi.host() << "'" << std::endl 
            << "|--[ build_info.type      : '" << bi.type() << "'" << std::endl 
            << "|--[ build_info.linkage   : '" << bi.linkage() << "'" << std::endl 
            << "|--[ build_info.compiler  : '" << bi.compiler() << "'" << std::endl 
            << "|" << std::endl ;
  
}

