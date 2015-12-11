#define BOOST_TEST_MODULE fps_fs
#include <boost/test/unit_test.hpp>

#include "fps_fs/buffer.h"
#include "fps_fs/file.h"
#include "fps_fs/path.h"
#include "fps_fs/glob.h"
#include "fps_process/process.h"

#include <iostream>

using namespace fps ;

//-----------------------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE( fps_fs__common, fps::fs::test::UnitTestFixture ) ;

//-----------------------------------------------------------------------------------------
// fps::fs::Path
//-----------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------
// fps::fs::Glob
//-----------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------
// fps::fs::Buffer
//-----------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_fs__buffer )
{
  std::cout << "[ Unit Test -> fps::fs::Buffer ]" <<std::endl ;
  fs::Buffer buffer ;
  BOOST_REQUIRE( buffer.empty() ) ;
  BOOST_REQUIRE( buffer.capacity() == fs::Buffer::Default_Capacity ) ;

  String alphabet ( "abcdefghijklmnopqrstuvwxyz" ) ;
  String delimiter( "\n" ) ; 

  const uint32_t insert_count = 30 ;
  buffer.assign( alphabet.c_str(), alphabet.size() ) ;
  buffer.append( delimiter.c_str(), delimiter.size() ) ;
  for( uint32_t idx = 1 ; idx < insert_count; ++idx ) 
  { buffer.append( alphabet.c_str(), alphabet.size() ) ;
    buffer.append( delimiter.c_str(), delimiter.size() ) ;
  }
  BOOST_REQUIRE( buffer.size() == (alphabet.size() + delimiter.size()) * insert_count ) ;

  uint32_t extract_count = 0 ;
  while( !buffer.empty() ) 
  {
    fs::Buffer::Slice result = buffer.extract<fs::test::NewlineExtractor>() ;
    if( result ) 
    { String result_str( result.begin(), result.length() ) ;
      result_str.rstrip( delimiter[0] ) ;
      ++extract_count ;
      BOOST_REQUIRE( result_str == alphabet ) ;
    }
    else 
      break ;
  }
  BOOST_REQUIRE( extract_count == insert_count ) ;
  BOOST_REQUIRE( buffer.empty() ) ;

  std::cout << std::endl ;
}

//-----------------------------------------------------------------------------------------
// fps::fs::File 
//-----------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_fs__file )
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

BOOST_AUTO_TEST_SUITE_END() ;

