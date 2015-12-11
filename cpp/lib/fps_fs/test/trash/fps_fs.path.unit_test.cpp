#define BOOST_TEST_MODULE fps_fs__path 
#include <boost/test/unit_test.hpp>

#include "fps_string/string.h"
#include "fps_process/process.h"
#include "fps_fs/path.h"
#include "fps_fs/glob.h"
#include "fps_fs/test/common.h"

using namespace fps ;

BOOST_AUTO_TEST_CASE( fps_fs__path ) 
{
  std::cout << "[ Unit Test -> fps::fs::Path ]" << std::endl ;

  fs::Path test_dir( "/tmp"
                     , String::sprintf( "fps_fs.path.unit_test.%s.%d"
                                      , process::uname().c_str()
                                      , process::pid() 
                                      )
                     ) ;

  {
    fs::Path p1( "." ) ;
    fs::test::to_stdout( p1, "Current working directory (relative)" ) ;
    BOOST_REQUIRE( p1.exists() ) ;
    BOOST_REQUIRE( p1.is_directory() ) ;

    fs::Path p2( p1.absolute() ) ;
    fs::test::to_stdout( p2, "Current working directory (absolute)" ) ;
    BOOST_REQUIRE( p1.absolute().str() == p2.str() ) ;
    BOOST_REQUIRE( p2.is_directory() ) ;
    std::cout << " p1 : " << p1.str() << std::endl 
              << " p2 : " << p2.str() << std::endl << std::flush ;
    BOOST_REQUIRE( p1.str() != p2.str() ) ;
  }

  {
    test_dir.mkdirs() ;
    fs::test::to_stdout( test_dir, "Created test directory" ) ;
    BOOST_REQUIRE( test_dir.exists() && test_dir.is_directory() ) ;

    fs::Path p1( test_dir.str(), "test_file.01" ) ;
    p1.touch() ;
    fs::test::to_stdout( p1, "Created test file" ) ;
    BOOST_REQUIRE( p1.is_regular_file() ) ;

    p1.rm() ;
    fs::test::to_stdout( p1, "Deleted test file" ) ;
    BOOST_REQUIRE( !p1.exists() && !p1.is_regular_file() ) ;
  }

  test_dir.rmtree() ;
  fs::test::to_stdout( test_dir, "Removed test directory" ) ;
  BOOST_REQUIRE( !test_dir.exists() ) ;
}
