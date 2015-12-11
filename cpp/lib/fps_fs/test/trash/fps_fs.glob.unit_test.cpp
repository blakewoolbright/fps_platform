#define BOOST_TEST_MODULE fps_fs__glob
#include <boost/test/unit_test.hpp>

#include "fps_fs/glob.h"
#include "fps_fs/path.h"
#include "fps_fs/test/common.h"
#include "fps_process/process.h"

#include <iostream>

using namespace fps ;

BOOST_AUTO_TEST_CASE( fps_fs__glob ) 
{
  fs::Path test_dir( "/tmp"
                     , String::sprintf( "fps_fs.glob.unit_test.%s.%d"
                                      , process::uname().c_str()
                                      , process::pid() 
                                      ) 
                     ) ;

  std::cout << "[ Unit Test -> fps::fs::Glob ]" << std::endl ;
  {
    fs::test::to_stdout( test_dir, "Test Directory" ) ;

    if( test_dir.exists() )
      test_dir.rmtree() ;

    BOOST_REQUIRE( !test_dir.exists() || test_dir.is_directory() ) ;

    if( !test_dir.exists() ) 
    { if( !test_dir.mkdirs() ) 
      { std::cout << "  ! ERROR :: Failed to create test directory " << test_dir.str() << std::endl  
                  << "          :: " << test_dir.last_error_str() << std::endl 
                  << std::endl ;
      }
    }
    std::cout << std::flush ;

    BOOST_REQUIRE( test_dir.is_directory() && "Failed to create test directory" ) ;

    std::vector<fps::String> f_names = { "abc_1", "abc_2", "abc_3", "bcd_1", "bcd_2", "bcd_2.old"
                                       , "Abc_1", "ABC_1"
                                       , "abcvery_very_very_very_very_very_very_long_suffix"
                                       } ;

    std::cout << "  ( Creating " << f_names.size() << " test files )" << std::endl ;
    for( auto itr = f_names.begin() ; itr != f_names.end() ; ++itr ) 
    { std::cout << "    " << *itr << std::endl ;
      fs::Path p( test_dir.str(), *itr ) ;
      if( !p.is_regular_file() ) 
        p.touch() ;
    }
    std::cout << std::endl ;

    fs::Glob test_glob( test_dir.str() + "/*" ) ; // "/tmp/fps_fs" ) ;
    fs::test::to_stdout( test_glob, "Listing all files" ) ;
    BOOST_REQUIRE( test_glob.size() == f_names.size() ) ;

    test_glob.reset( fs::Path::join( test_dir.str(), "abc*" ) ) ;
    fs::test::to_stdout( test_glob, "Names starting w/ 'abc'" ) ;
    BOOST_REQUIRE( test_glob.size() == 4 ) ;

    test_glob.reset( fs::Path::join( test_dir.str(), "{a,b}*_1" ) ) ;
    fs::test::to_stdout( test_glob, "Names starting w/ a|b and ending w/ '_1'" ) ;
    BOOST_REQUIRE( test_glob.size() == 2 ) ;

    test_glob.reset( fs::Path::join( test_dir.str(), "{a,b,A,B}*_{1,2}" ) ) ;
    fs::test::to_stdout( test_glob, "Names starting w/ a|A|b|B and ending w/ '_1' or '_2'" ) ;
    BOOST_REQUIRE( test_glob.size() == 6 ) ;
  }
  
  test_dir.rmtree() ;
  BOOST_REQUIRE( !test_dir.exists() ) ;
}

