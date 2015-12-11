#include "tz_manager.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <unistd.h>
// #include <iostream>

namespace fps  {
namespace time {

    //--------------------------------------------------------------------------------------
    const char * TZManager::Zoneinfo_Path = "/usr/share/zoneinfo" ;

    //--------------------------------------------------------------------------------------
    bool 
    TZManager::clear() 
    {
      if( !valid_ ) 
        return true ;

      int32_t env_rv = ( old_tz_.empty() ) 
                     ? ::unsetenv( "TZ" ) 
                     : ::setenv( "TZ", old_tz_.c_str(), 1 ) 
                     ;

      if( env_rv == 0 ) 
        ::tzset() ;

      old_tz_.clear() ;
      cur_tz_.clear() ;
      valid_ = false ;
      return env_rv == 0 ;
    }

    //--------------------------------------------------------------------------------------
    bool
    TZManager::set( const char * new_tz ) 
    {
      clear() ;

      if( new_tz != NULL && *new_tz !=  0 ) 
      {
        std::string zi_path( Zoneinfo_Path ) ;
        zi_path += '/' ;
        zi_path += new_tz ;

        // std::cout << "TZManager :: zi_path = '" << zi_path << "'" << std::endl ;

        int32_t zi_exists = ::access( zi_path.c_str(), F_OK ) ;
        if( zi_exists != 0 ) 
          return false ;

        // std::cout << "TZManager :: zi_exists = '" << (!zi_exists?"true":"false") << "'" << std::endl ;
      }

      const char * env_tz = ::getenv( "TZ" ) ;
      if( env_tz != NULL && env_tz[0] != 0 ) 
        old_tz_ = env_tz ;

      // std::cout << "TZManager :: old_tz_ = '" << old_tz_ << "'" << std::endl ;

      if( new_tz == NULL || *new_tz == 0 ) 
      { int32_t unset_rv = ::unsetenv( "TZ" ) ;
        if( 0 != unset_rv ) 
          return false ; 
        
        cur_tz_.clear() ;
      }
      else
      {
        int32_t set_rv = ::setenv( "TZ", new_tz, 1 ) ;
        if( 0 != set_rv ) 
          return false ;

        cur_tz_ = new_tz ;
      }

      ::tzset() ;
    
      valid_ = true ;
      return true ;
    }
}}
