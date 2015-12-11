#ifndef FPS__TIME__TZ_MANAGER__H
#define FPS__TIME__TZ_MANAGER__H

#include <string>

namespace fps  {
namespace time {

  //----------------------------------------------------------------------------------------
  class TZManager 
  {
  public :
    static const char * Zoneinfo_Path ;

  private :
    bool        valid_  ;
    std::string old_tz_ ;
    std::string cur_tz_ ;

  public :

    //--------------------------------------------------------------------------------------
    // This constuctor changes the value of the TZ env variable to the indicated argument.
    // Upon destruction, the original value will be restored to the environment 
    // automatically.
    //
    // The 'tz_name' argument must correspond to a file in /usr/share/zoneinfo.  If there is 
    // a problem changing to the indicated timezone, the 'valid()' member will return false
    // after construction completes.
    //
    // If the 'tz_name' argument is null or empty, the the TZ variable will be removed from the 
    // environment until destruction.  Upon destruction, the orginal value of TZ will be 
    // restored to the environment.  (If TZ was originally empty, it will be removed from the 
    // environment)
    // 
    // Examples : 
    //   time::TZManager tz_2_ac  ( "America/Chicago" ) ; 
    //   time::TZManager tz_2_utc ( "UTC" ) ;
    //   time::TZManager tz_2_cst6( "CST6CDT" ) ;
    //
    //--------------------------------------------------------------------------------------
    inline TZManager() : valid_( false ) {}

    //--------------------------------------------------------------------------------------
    inline 
    explicit 
    TZManager( const char * tz_name ) 
      : valid_( false ) 
    { set( tz_name ) ; 
    }

    //--------------------------------------------------------------------------------------
    inline 
    explicit 
    TZManager( const std::string & tz_name ) 
      : valid_( false ) 
    { set( tz_name ) ;
    }

    //--------------------------------------------------------------------------------------
    inline ~TZManager() { clear() ; }
    
    //--------------------------------------------------------------------------------------
    bool        clear() ;
    bool        set( const char * tz_name ) ;
    inline bool set( const std::string & tz_name ) { return set( tz_name.c_str() ) ; }    
    

    //--------------------------------------------------------------------------------------
    inline const std::string & cur_tz() const { return cur_tz_ ; }
    inline const std::string & old_tz() const { return old_tz_ ; }
    inline bool                valid()  const { return valid_ ; }
  } ;

}}

#endif
