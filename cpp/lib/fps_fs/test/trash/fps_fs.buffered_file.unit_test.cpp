#define BOOST_TEST_MODULE fps_fs__buffered_file
#include <boost/test/unit_test.hpp>

#include "fps_fs/test/common.h"
#include "fps_fs/buffered_file.h"
#include "fps_process/process.h"

#include <iostream>

using namespace fps ;

struct NewlineExtractor
{
  static const char * apply( const char * begin, uint32_t length )
  { 
    const char * end = begin + length ;
    while( begin < end && *begin != '\n' ) { ++begin ; }
    return (*begin == '\n') ? begin + 1 : NULL ;
  } 
} ;

BOOST_AUTO_TEST_CASE( fps_file__buffered_file )
{
  fs::Path test_dir( "/tmp", "fps_fs.buffered_file.unit_test"
                     /*, String::sprintf( "fps_fs.buffered_file.unit_test.%s.%d"
                                      , process::uname().c_str()
                                      , process::pid() 
                                      ) 
                     */
                     ) ;
  test_dir.mkdirs() ;

  std::cout << "[ Unit Test -> fps::fs::BufferedFile ]" <<std::endl ;
  fs::test::to_stdout( test_dir, "Created Test Directory" ) ;
  {
    fs::Path         f1_path( test_dir, "test_01.buffered_file.txt" ) ;
    fs::BufferedFile f1_file( f1_path.str(), "w" ) ;

    fs::test::to_stdout( f1_file, "Opened Test File" ) ;
    
    f1_file.write( "abcd\n", 5 ) ;

    // f1_file.writef( "'%s' : { v1: %u, v2: %u }\n", "test_object", 1, 2 ) ;
  }
  std::cout << std::endl ;

  // test_dir.rmtree() ;
}

