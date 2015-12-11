#ifndef FPS__FS__TEST__COMMON__H
#define FPS__FS__TEST__COMMON__H

#include "fps_fs/path.h"
#include "fps_fs/glob.h"
#include "fps_string/string.h"
#include "fps_process/process.h"
#include "fps_fs/file.h"
#include "fps_fs/buffered_file.h"
#include <iostream>
#include <sys/types.h>
#include <cstdlib>

namespace fps {
namespace fs  { 
namespace test{


  //----------------------------------------------------------------------------------
  struct NewlineExtractor
  {
    static const char * apply( const char * begin, uint32_t length )
    { 
      const char * end = begin + length ;
      while( begin < end && *begin != '\n' ) { ++begin ; }
      return (*begin == '\n') ? begin + 1 : NULL ;
    } 
  } ;

  //----------------------------------------------------------------------------------
  inline bool 
  create_directories( const std::vector<fps::String> names ) 
  {
    for( auto itr = names.begin() ; itr != names.end() ; ++itr )
    { fs::Path p( *itr ) ;
      if( p.is_directory() )
        continue ;
      p.mkdirs() ;
    }
    
    for( auto itr = names.begin() ; itr != names.end() ; ++itr ) 
    { fs::Path p( *itr ) ;
      if( !p.is_directory() ) 
        return false ;
    }
    return true ;
  }


  //-------------------------------------------------------------------------------------------------------
  inline bool 
  create_files( const std::vector<fps::String> names ) 
  {
    for( auto itr = names.begin() ; itr != names.end() ; ++itr )
    { fs::Path p( *itr ) ;
      if( p.is_regular_file() )
        continue ;
      p.touch() ;
    }
    
    for( auto itr = names.begin() ; itr != names.end() ; ++itr ) 
    { fs::Path p( *itr ) ;
      if( !p.is_regular_file() ) 
        return false ;
    }
    return true ;
  }

  //-------------------------------------------------------------------------------------------------------
  inline void 
  to_stdout( const Glob & glob, const String & label ) 
  {
    std::cout << "  ( " << label << " )" << std::endl 
              << "    empty()     : " << String().from( glob.empty() ) << std::endl 
              << "    pattern()   : " << glob.pattern() << std::endl 
              << "    size()      : " << glob.size() << std::endl 
              << "    content()   : " ;

      if( !glob.empty() ) 
      { auto itr=glob.begin() ;
        std::cout << *itr << std::endl ;
        ++itr ;
        for( ; itr != glob.end() ; ++itr ) 
          std::cout << "                , " << *itr << std::endl ;
        std::cout << std::endl ;
      }
  }

  //-------------------------------------------------------------------------------------------------------
  inline void 
  to_stdout( const Path & path, const String & label )
  {
    std::cout << "  ( " << label << " )" << std::endl 
              << "    str()              : " << path.str() << std::endl 
              << "    absolute()         : " << path.absolute().str() << std::endl 
              << "    basename()         : " << path.basename() << std::endl 
              << "    dirname ()         : " << path.dirname() << std::endl 
              << "    owner   ()         : " << path.owner() << std::endl 
              << "    uid     ()         : " << path.uid() << std::endl
              << "    gid     ()         : " << path.gid() << std::endl
              << "    exists()           : " << String().from( path.exists() ) << std::endl 
              << "    is_regular_file()  : " << String().from( path.is_regular_file() ) << std::endl 
              << "    is_directory()     : " << String().from( path.is_directory() ) << std::endl 
              << "    is_char_device()   : " << String().from( path.is_char_device() ) << std::endl 
              << "    is_block_device()  : " << String().from( path.is_block_device() ) << std::endl 
              << "    is_pipe()          : " << String().from( path.is_pipe() ) << std::endl 
              << "    is_symlink()       : " << String().from( path.is_symlink() ) << std::endl 
              << "    is_socket()        : " << String().from( path.is_socket() ) << std::endl 
              << "    is_dir()           : " << String().from( path.is_dir() ) << std::endl 
              << "    is_file()          : " << String().from( path.is_file() ) << std::endl 
              << "    last_accessed_ts() : " << path.last_accessed_ts().to_string( "%Y-%m-%d %H:%M:%S" ) 
                                             << " (" << path.last_accessed_ts().epoch_nanos() 
                                             << ")" << std::endl 
              << "    last_modified_ts() : " << path.last_modified_ts().to_string( "%Y-%m-%d %H:%M:%S" ) 
                                             << " (" << path.last_modified_ts().epoch_nanos() 
                                             << ")" << std::endl 
              << std::endl ;
  }

  //-------------------------------------------------------------------------------------------------------
  inline void 
  to_stdout( const File & file, const String & label )
  {
    std::cout << "  ( " << label << " )" << std::endl 
              << "    name         : " << file.name() << std::endl
              << "    mode         : " << file.mode() << std::endl 
              << "    is_open      : " << String().from( file.is_open() ) << std::endl 
              << "    eof          : " << String().from( file.eof() ) << std::endl 
              << std::endl ;
  }

  //-------------------------------------------------------------------------------------------------------
  inline void 
  to_stdout( const BufferedFile & file, const String & label )
  {
    std::cout << "  ( " << label << " )" << std::endl 
              << "    name         : " << file.name() << std::endl
              << "    mode         : " << file.mode() << std::endl 
              << "    is_open      : " << String().from( file.is_open() ) << std::endl 
              << "    eof          : " << String().from( file.eof() ) << std::endl 
              << std::endl ;
  }
}}}

#endif
