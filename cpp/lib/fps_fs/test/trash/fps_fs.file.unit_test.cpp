#define BOOST_TEST_MODULE fps_fs__file
#include <boost/test/unit_test.hpp>

#include "fps_fs/glob.h"
#include "fps_fs/path.h"
#include "fps_fs/file.h"
#include "fps_fs/test/common.h"
#include "fps_process/process.h"

#include <iostream>

using namespace fps ;

BOOST_AUTO_TEST_CASE( fps_file__file )
{
  fs::Path test_dir( "/tmp"
                     , String::sprintf( "fps_fs.file.unit_test.%s.%d"
                                      , process::uname().c_str()
                                      , process::pid() 
                                      ) 
                     ) ;

  std::cout << "[ Unit Test -> fps::fs::File ]" <<std::endl ;
  test_dir.mkdirs() ;
  fs::test::to_stdout( test_dir, "Created Test Directory" ) ;
  { 
    fs::Path f1_path( test_dir, "test_01.txt" ) ;
    fs::File f1( fs::Path( test_dir, "test_01.txt" ), "w" ) ;
    fs::test::to_stdout( f1_path, "Created Test File" ) ;
    BOOST_REQUIRE( f1.is_open() ) ;
  }

  fs::test::to_stdout( test_dir, "Removed Test Directory" ) ;
}
