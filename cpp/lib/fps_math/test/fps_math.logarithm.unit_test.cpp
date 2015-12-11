#define BOOST_TEST_MODULE fps_math__logarithm__test
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "fps_math/logarithm.h"

using namespace fps ;

BOOST_AUTO_TEST_CASE( fps_math__logarithm ) 
{
  
  uint32_t log2_255   = math::fast_log2( 255U ) ;
  uint32_t log2_10    = math::fast_log2( 10U  ) ;
  uint64_t log2_17000 = math::fast_log2( 17000UL ) ;

  int64_t  log10_100  = math::fast_log10( 100 ) ;
  int32_t  log10_999  = math::fast_log10( 999  ) ;
  int32_t  log10_1000 = math::fast_log10( 1000 ) ;
  int32_t  log10_1001 = math::fast_log10( 1001 ) ;

  std::cout << "  Log2 ( 255 )   : " << log2_255 << std::endl
            << "  Log2 ( 10  )   : " << log2_10 << std::endl 
            << "  Log2 ( 17000 ) : " << log2_17000 << std::endl 
            << "  Log10( 100 )   : " << log10_100 << std::endl 
            << "  Log10( 999  )  : " << log10_999 << std::endl 
            << "  Log10( 1000 )  : " << log10_1000 << std::endl 
            << "  Log10( 1001 )  : " << log10_1001 << std::endl 
            << std::endl ;

  BOOST_CHECK( log2_255   == 7 ) ;
  BOOST_CHECK( log2_10    == 3 ) ;
  BOOST_CHECK( log2_17000 == 14 ) ;
  
  BOOST_CHECK( log10_100  == 2 ) ;
  BOOST_CHECK( log10_999  == 2 ) ;
  BOOST_CHECK( log10_1000 == 3 ) ;
  BOOST_CHECK( log10_1001 == 3 ) ;
}
