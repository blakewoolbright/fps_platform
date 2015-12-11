#define BOOST_TEST_MODULE fps_time__core__test

#include "fps_time/fps_time.h"
#include <boost/test/unit_test.hpp>
#include <iostream>

using namespace fps ;

//-------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_time__clock ) 
{
  int64_t sleep_usecs = 100000 ;

  std::cout << "[ time::Clock unit tests ]" << std::endl ;
  uint64_t epoch_ts_1 = time::Clock::now() ;
  ::usleep( sleep_usecs ) ;
  uint64_t epoch_ts_2 = time::Clock::now() ;
  
  int64_t delta_usecs = (epoch_ts_2 - epoch_ts_1) / time::Nanos_Per_Micro ;

  std::cout << "|--[ Sleep USecs  : " << sleep_usecs << " ]" << std::endl 
            << "|--[ TS1 (Before) : " << epoch_ts_1  << " ]" << std::endl 
            << "|--[ TS2 (After)  : " << epoch_ts_2  << " ]" << std::endl 
            << "|--[ Delta Usecs  : " << delta_usecs << " ]" << std::endl 
            << "|" << std::endl 
            ;

  BOOST_CHECK_MESSAGE( epoch_ts_1 < epoch_ts_2
                     , "\n\ttime::Clock | Clock not incrementing"
                     ) ;

  BOOST_CHECK_MESSAGE( ::abs(sleep_usecs - delta_usecs) < 1000
                     , "\n\ttime::Clock | Clock not accurate"
                     ) ;

  std::cout << "|--[ Success ]" << std::endl << std::endl ;
}

//-------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_time__core ) 
{
  std::cout << "[ time::Timestamp/Datetime unit tests ]" << std::endl ;

  uint64_t clk_now = time::Clock::now() ;

  time::Timestamp ts_now( clk_now ) ;
  time::Timestamp ts_mdt( ts_now.midnight() ) ;

  time::Datetime dtm_ts_now( ts_now ) ;
  time::Datetime dtm_ts_mdt( ts_mdt ) ;
  time::Datetime dtm_now   ( clk_now ) ;
  time::Datetime dtm_mdt   ( dtm_now.midnight() ) ;

  std::cout << "|--[ TS (@Now) :: " << ts_now.to_string<time::DT_Nano>() << " ]" << std::endl 
            << "|" << std::endl 
            << "|--[ TS (@Mdt) :: " << ts_mdt.to_string<time::DT_Nano>() << " ]" << std::endl 
            << "|" << std::endl 
            << "|--[ DTM(@Now) :: " << dtm_now.to_string<time::DT_Nano>() << " ]" << std::endl 
            << "|" << std::endl 
            << "|--[ DTM(@Mdt) :: " << dtm_mdt.to_string<time::DT_Nano>() << " ]" << std::endl 
            << "|" << std::endl 
            ;

  BOOST_CHECK_MESSAGE( ts_now.epoch_nanos() >= ts_mdt.epoch_nanos() 
                     , "\n\ttime::Timestamp - Midnight greater than now"
                     ) ;

  BOOST_CHECK_MESSAGE( dtm_now == dtm_ts_now, "\n\tConversion Error :: Timestamp -> Datetime" ) ;
  BOOST_CHECK_MESSAGE( dtm_mdt == dtm_ts_mdt, "\n\tConversion Error :: Timestamp -> Datetime" ) ;
  BOOST_CHECK_MESSAGE( (0 == dtm_mdt.hour()) 
                     &&(0 == dtm_mdt.minute())
                     &&(0 == dtm_mdt.second())
                     &&(0 == dtm_mdt.nanosecond())
                     , "\n\tInvalid hour/minute/second/nano detected in output of Datetime::midnight() function"
                     ) ;
  std::cout << "|--[ Success ]" << std::endl << std::endl ;
}

//-------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_time__tz_mgr ) 
{
  std::string env_tz ; 
  if( ::getenv( "TZ" ) ) 
    env_tz = ::getenv( "TZ" ) ;

  std::cout << "[ TZManager unit tests ]" << std::endl ;
  time::Timestamp ts_mdt = time::Timestamp::now().midnight() ;
  
  std::string pre_test_ts = ts_mdt.to_string<time::DT_Nano>() ;

  std::cout << "|--[ Timezone - Unset ]" << std::endl 
            << "|  |--[ TS : " << pre_test_ts << " ]" << std::endl 
            << "|" << std::endl ;
  {
    time::TZManager tz_mgr ;
    std::string tz_name( "America/New_York" ) ;

    bool set_rv = tz_mgr.set( tz_name ) ;
    BOOST_CHECK_MESSAGE
    ( set_rv  
    , string::sprintf( "\n\tFailure reported from TZManger::set( '%s' )", tz_name.c_str() )
    ) ; 
    
    std::cout << "|--[ TZManager( " << tz_name << " ) ]" << std::endl 
              << "|  |--[ TS : " << ts_mdt.to_string<time::DT_Nano>() << " ]" << std::endl 
              << "|" << std::endl ;
  
    tz_name = "America/Chicago" ;
    set_rv = tz_mgr.set( tz_name.c_str() ) ;
    BOOST_CHECK_MESSAGE
    ( set_rv  
    , string::sprintf( "\n\tFailure reported from TZManger::set( '%s' )", tz_name.c_str() )
    ) ; 
    
    std::cout << "|--[ TZManager( " << tz_name << " ) ]" << std::endl 
              << "|  |--[ TS : " << ts_mdt.to_string<time::DT_Nano>() << " ]" << std::endl 
              << "|" << std::endl ;

    tz_name = "America/Los_Angeles" ;
    set_rv = tz_mgr.set( tz_name.c_str() ) ;
    BOOST_CHECK_MESSAGE
    ( set_rv  
    , string::sprintf( "\n\tFailure reported from TZManger::set( '%s' )", tz_name.c_str() )
    ) ; 
    
    std::cout << "|--[ TZManager( " << tz_name << " ) ]" << std::endl 
              << "|  |--[ TS : " << ts_mdt.to_string<time::DT_Nano>() << " ]" << std::endl 
              << "|" << std::endl ;
  }

  std::string post_test_ts = ts_mdt.to_string<time::DT_Nano>() ;
  std::cout << "|--[ Timezone - Unset ]" << std::endl 
            << "|  |--[ TS : " << post_test_ts << " ]" << std::endl 
            << "|" << std::endl ;

  BOOST_CHECK_MESSAGE
  ( pre_test_ts == post_test_ts 
  , "\n\ttime::TZManager did not properly restore the state of the TZ variable"
  ) ;

  std::cout << "|--[ Success ]" << std::endl << std::endl ;
}

