#define BOOST_TEST_MODULE fps_system

#include "fps_string/format.h"
#include "fps_system/fps_system.h"

#include <boost/test/unit_test.hpp>
#include <thread>
#include <iostream>

using namespace fps ;

BOOST_AUTO_TEST_CASE( fps_system__cpu )
{
  std::cout << "[ fps::system unit tests ]" << std::endl  
            << "|--[ CPU Cores          :: " << system::cpu::core_count() << " ]" << std::endl 
            << "|" << std::endl ;

  // Save initial affinity setting
  system::cpu::AffinityMask af_start = system::cpu::get_affinity() ;
  // std::cout << "|--[ Start CPU Affinity :: " << af_start.to_string() << " ]" << std::endl ;

  // Create new mask containing cpu 1 and 2.
  system::cpu::AffinityMask af_new( 1 ) ;
  af_new.set( 2 ) ;

  BOOST_CHECK_MESSAGE
  ( af_new.size() == 2 
  , string::sprintf( "\n\tcpu::AffinityMask::size() should be %u not %u", 2, af_new.size() ) 
  ) ;

  BOOST_CHECK_MESSAGE
  ( af_new.is_set( 1 ) 
  , string::sprintf( "\n\tcpu::AffinityMask::is_set( 1 ) should return true" )
  ) ;

  BOOST_CHECK_MESSAGE
  ( af_new.is_set( 2 ) 
  , string::sprintf( "\n\tcpu::AffinityMask::is_set( 2 ) should return true" )
  ) ;

  BOOST_CHECK_MESSAGE
  ( !af_new.is_set( 0 ) 
  , string::sprintf( "\n\tcpu::AffinityMask::is_set( 0 ) should return false" )
  ) ;

  // Apply new affinity mask & check return value
  BOOST_CHECK_MESSAGE
  ( system::cpu::set_affinity( af_new ) 
  , string::sprintf( "\n\tFailure reported by system::set_affinity w/ cpus '%s'"
                   , af_new.to_string().c_str()
                   )
  ) ;

  // Make sure the new mask was actually applied
  system::cpu::AffinityMask af_end = system::cpu::get_affinity() ;  
  BOOST_CHECK_MESSAGE
  ( af_end == af_new 
  , string::sprintf( "\n\tCurrent affinity '%s' != expected affinity '%s'"
                   , af_end.to_string().c_str(), af_new.to_string().c_str() 
                   ) 
  ) ;

  system::cpu::set_affinity( af_start ) ;
}

