#define BOOST_TEST_MODULE fps_util__containers

#include "fps_util/detail/sorted_vector.h"
#include "fps_util/detail/sorted_integral_vector.h"
#include "fps_string/fps_string.h"

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>

using namespace fps ;

//---------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_util__containers__sorted_vector )
{
  typedef util::detail::SortedIntegralVector<uint64_t> vec_t ;

  vec_t vec ;
  BOOST_CHECK_MESSAGE
  ( vec.empty() 
  , "\n\tSortedIntegralVector::empty() should return true after default construction" 
  ) ;

  BOOST_CHECK_MESSAGE
  ( vec.size() == 0 
  , "\n\tSortedIntegralVector::size() should return 0 after default construction" 
  ) ;

  BOOST_CHECK_MESSAGE
  ( vec.capacity() > 0 
  , "\n\tSortedIntegralVector::capacity() should return non-zero after default construction" 
  ) ;

  
  { 
    vec_t::value_t i_value = 1 ;
    auto           i_itr   = vec.insert( i_value ) ;
    BOOST_CHECK_MESSAGE
    ( i_itr != vec.end() 
    , string::sprintf( "\n\tSortedIntegralVector::insert( %lu ) returned end()", i_value ) 
    ) ;

    BOOST_CHECK_MESSAGE
    ( vec.size() == 1
    , string::sprintf( "\n\tSortedIntegralVector::size() returned %u, expected 1", vec.size() ) 
    ) ;
  }

}

