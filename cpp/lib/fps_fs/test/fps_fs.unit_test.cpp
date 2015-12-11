#define BOOST_TEST_MODULE fps_fs
#include <boost/test/unit_test.hpp>

#include "fps_string/fps_string.h"
#include "fps_fs/fps_fs.h"

#include <iostream>

using namespace fps ;

//-------------------------------------------------------------------------------------------
// Helper class that ensures any directories/files created during testing are removed
//-------------------------------------------------------------------------------------------
class TestDirectory 
{
  fs::Path dir_ ;

public :
  TestDirectory( const std::string & dir ) 
    : dir_( dir )
  { 
    dir_.mkdirs() ;
  }
  
  ~TestDirectory() 
  {
    if( dir_.exists() ) 
      dir_.rmtree() ;
  }
  
  const fs::Path & path() const { return dir_ ; }
} ;

//-------------------------------------------------------------------------------------------
void 
to_stdout( std::string label, const fs::Path & path ) 
{
  std::cout << "|--[ " << label << " ]" << std::endl 
            << "|  |--{ path string  : " << path.str() << " }" << std::endl
            << "|  |--{ parent name  : " << path.parent() << " }" << std::endl 
            << "|  |--{ leaf name    : " << path.leaf() << " }" << std::endl 
//             << "|  |--{ leaf idx     : " << path.leaf_idx() << " }" << std::endl
            << "|  |--{ exists       ? " << (path.exists()?"true":"false") << " }" << std::endl 
            << "|  |--{ is_absolute  ? " << (path.is_absolute()?"true":"false") << " }" << std::endl 
            << "|  |--{ is_relative  ? " << (path.is_relative()?"true":"false") << " }" << std::endl 
            << "|  |--{ is_file      ? " << (path.is_file()?"true":"false") << " }" << std::endl 
            << "|  |--{ is_dir       ? " << (path.is_dir()?"true":"false") << " }" << std::endl 
            << "|  |--{ is_symlink   ? " << (path.is_symlink()?"true":"false") << " }" << std::endl 
            << "|  |--{ size         : " << path.size() << " }" << std::endl 
            << "|  |--{ owner        : " << path.owner() << " }" << std::endl 
            << "|" << std::endl ;
}

//-------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_fs__path )
{
  TestDirectory test_dir( "./fps_net.path.unit_test/" ) ;

  std::cout << "[ fps::fs::Path tests ]" << std::endl ;
  to_stdout( "Create <DIR>", test_dir.path() ) ;
  BOOST_CHECK_MESSAGE
  ( test_dir.path().exists() && test_dir.path().is_dir() && test_dir.path().is_relative()
  , string::sprintf( "\n\tFailed to create test directory '%s'", test_dir.path().c_str() ).c_str()
  ) ;

  fs::Path tst_path( fs::Path::join( test_dir.path().str(), "." ) ) ;
  to_stdout( "<DIR> + \".\"", tst_path ) ;
  BOOST_CHECK_MESSAGE
  ( tst_path.exists() && tst_path.is_dir() && tst_path.is_relative()
  , string::sprintf( "\n\tFailed to process path '%s'", tst_path.c_str() ).c_str()
  ) ;

  tst_path.assign( fs::Path::join( test_dir.path().str(), ".." ) ) ;
  to_stdout( "<DIR> + \"..\"", tst_path ) ;
  BOOST_CHECK_MESSAGE
  ( tst_path.exists() && tst_path.is_dir() && tst_path.is_relative()
  , string::sprintf( "\n\tFailed to process path '%s'", tst_path.c_str() ).c_str()
  ) ;

  tst_path = tst_path.absolute() ;  
  to_stdout( "absolute( <DIR> + \"..\" )", tst_path ) ;
  BOOST_CHECK_MESSAGE
  ( tst_path.exists() && tst_path.is_dir() && tst_path.is_absolute()
  , "Failed to convert relative path to absolute"
  ) ;

  tst_path.assign( fs::Path::join( test_dir.path().str(), "file_01" ) ) ;
  tst_path.touch() ;
  to_stdout( string::sprintf( "<DIR>/%s", tst_path.leaf().c_str() ), tst_path ) ;
  BOOST_CHECK_MESSAGE
  ( tst_path.exists() && tst_path.is_file() && tst_path.is_relative()
  , string::sprintf( "\n\tFailed to touch file '%s'", tst_path.c_str() ).c_str()
  ) ;

  tst_path.assign( fs::Path::join( test_dir.path().str(), "subdir_01/file_01" ) ) ;
  tst_path.touch() ;
  to_stdout( string::sprintf( "<DIR>/%s/%s", fs::Path( tst_path.parent() ).leaf().c_str(), tst_path.leaf().c_str() ), tst_path ) ;
  BOOST_CHECK_MESSAGE
  ( tst_path.exists() && tst_path.is_file() && tst_path.is_relative()
  , string::sprintf( "\n\tFailed to touch nested file '%s'", tst_path.c_str() ).c_str()
  ) ;
  std::cout << "|--[ Success ]" << std::endl << std::endl ;
}

//-----------------------------------------------------------------------------------------
void 
to_stdout( const fs::Glob & glob ) 
{
  std::cout << "|--[ " << glob.expression() << " : " << glob.size() << " matches ]" << std::endl ;
  for( auto itr = glob.begin() ; itr != glob.end() ; ++itr ) 
    std::cout << "|  |--[ " << *itr << " ]" << std::endl ;
  std::cout << "|" << std::endl ;
}

//-----------------------------------------------------------------------------------------
// fps::fs::Glob
//-----------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_fs__glob ) 
{
  TestDirectory test_dir( "./fps_net.glob.unit_test/" ) ;
  std::cout << "[ fps::fs::Glob unit tests ]" << std::endl << "|" << std::endl ;

  fs::Path test_path = test_dir.path() ;

  //
  // Create some files to match w/ glob expressions
  // 
  std::vector<std::string> f_names = { "abc_1", "abc_2", "abc_3", "bcd_1", "bcd_2"
                                     , "Abc_1", "ABC_1"
                                     , "abcvery_very_very_very_very_very_very_long_suffix"
                                     } ;

  std::cout << "|--[ Creating " << f_names.size() << " files... ]" << std::endl ;
  for( auto itr = f_names.begin() ; itr != f_names.end() ; ++itr ) 
  {
    fs::Path p( test_path.str(), *itr ) ;
    if( !p.is_file() ) 
      p.touch() ;
   
    BOOST_CHECK_MESSAGE
    ( p.is_file()
    , string::sprintf( "/n/tFailed to create fs::Glob test file '%s'", p.c_str() ) 
    ) ;

    std::cout << "|  |--[ + " << p.c_str() << " ]" << std::endl ;
  }
  std::cout << "|" << std::endl ;

  fs::Glob test_glob( fs::Path::join( test_path.str(), "*" ) ) ; 
  to_stdout( test_glob ) ;

  BOOST_CHECK_MESSAGE
  ( test_glob.valid() 
  , string::sprintf( "\n\tGlob '%s' is invalid (glob.valid() == false)", test_glob.expression().c_str() ).c_str() 
  ) ;

  BOOST_CHECK_MESSAGE
  ( test_glob.size() == f_names.size() 
  , string::sprintf( "\n\tGlob '%s' returned %u of %u expected matches"
                   , test_glob.expression().c_str()
                   , test_glob.size()
                   , f_names.size() 
                   ).c_str()
  ) ;
    

  test_glob.assign( fs::Path::join( test_path.str(), "abc*" ) ) ;
  to_stdout( test_glob ) ;
  BOOST_CHECK_MESSAGE
  ( test_glob.size() == 4 
  , string::sprintf( "Glob '%s' returned %u of %u expected matches", test_glob.expression().c_str(), test_glob.size(), 4 ) 
  ) ;

  test_glob.assign( fs::Path::join( test_path.str(), "{a,b}*_1" ) ) ;
  to_stdout( test_glob ) ;
  BOOST_CHECK_MESSAGE
  ( test_glob.size() == 2 
  , string::sprintf( "Glob '%s' returned %u of %u expected matches", test_glob.expression().c_str(), test_glob.size(), 2 ) 
  ) ;

  test_glob.assign( fs::Path::join( test_path.str(), "{a,b,A,B}*_{1,2}" ) ) ;
  to_stdout( test_glob ) ;
  BOOST_CHECK_MESSAGE
  ( test_glob.size() == 6 
  , string::sprintf( "Glob '%s' returned %u of %u expected matches", test_glob.expression().c_str(), test_glob.size(), 6 ) 
  ) ;
  std::cout << "|--[ Success ]" << std::endl << std::endl ;
}

//-----------------------------------------------------------------------------------------
// fps::fs::File 
//-----------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_fs__file )
{
  TestDirectory test_dir( "./fps_net.file.unit_test/" ) ;
  std::cout << "[ fs::File unit tests ]" << std::endl ;

  BOOST_CHECK_MESSAGE
  ( test_dir.path().is_dir()
  , string::sprintf( "\n\tfps::File :: Failed to create unit test temp directory '%s'"
                   , test_dir.path().c_str() 
                   ) 
  ) ;

  fs::File f1( fs::Path::join( test_dir.path().str(), "test_01.txt" ), "w" ) ;
  BOOST_CHECK_MESSAGE
  ( f1.is_open() 
  , string::sprintf( "\n\tfs::File :: Failed to create test file '%s'", f1.name().c_str() )
  ) ;
  
  std::string w_str( "ABCD\nEFGH\nIHKL\nM\nN\nOPQ\nRSTU\nVWXYZ" ) ;

  int32_t w_rv = f1.write( w_str.c_str(), w_str.length() ) ;
  BOOST_CHECK_MESSAGE
  ( w_rv == w_str.length() 
  , string::sprintf( "\n\tfs::File :: Wrote %d of %u expected characters", w_rv, w_str.length() )
  ) ;

  f1.close() ;
  BOOST_CHECK_MESSAGE
  ( !f1.is_open()
  , string::sprintf( "\n\tfs::File :: Failed to close file '%s'", f1.name().c_str() )
  ) ;

  f1.open( f1.name(), "rb" ) ;
  BOOST_CHECK_MESSAGE
  ( f1.is_open() 
  , string::sprintf( "\n\tfs::File :: Failed to reopen test file '%s' for reading"
                   , f1.name().c_str()
                   ) 
  ) ;

  BOOST_CHECK_MESSAGE
  ( !f1.eof() 
  , string::sprintf( "\n\tfs::File :: Input file file '%s' unexpectedly empty"
                   , f1.name().c_str()
                   ) 
  ) ;

  std::string r_str ;
  r_str.resize( w_str.length() * 2 ) ;
  int32_t r_rv = f1.read( &(r_str[0]), r_str.size() ) ;
  BOOST_CHECK_MESSAGE
  ( r_rv == w_str.length() 
  , string::sprintf( "\n\tfs::File :: Read %d of %u expected bytes from file '%s'"
                   , r_rv, w_str.length(), f1.name().c_str() 
                   ) 
  ) ;
  r_str.resize( r_rv ) ;

  std::cout << "|--[ Success ]" << std::endl << std::endl ;
}


