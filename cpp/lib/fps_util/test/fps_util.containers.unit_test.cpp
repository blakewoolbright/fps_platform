#define BOOST_TEST_MODULE fps_util__containers

#include "fps_util/detail/sorted_vector.h"
#include "fps_util/detail/sorted_integral_vector.h"
#include "fps_string/fps_string.h"

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>

using namespace fps ;

//---------------------------------------------------------------------------------------------------
template<typename T>
void
to_stdout( const T & container, const std::string & label ) 
{
  std::cout << "[ " << label << " ]" << std::endl 
            << "|--[ Capacity : " << container.capacity() << " ]" << std::endl 
            << "|--[ Size     : " << container.size() << " ]" << std::endl 
            << "|--[ Empty    : " << (container.empty() ? "true" : "false") << " ]" << std::endl ;

  std::string values ;
  if( !container.empty() ) 
  { auto itr = container.begin() ; 
    string::append( values, "%lu", static_cast<uint64_t>( *itr ) ) ;

    while( ++itr != container.end() ) 
      string::append( values, ", %lu", static_cast<uint64_t>( *itr ) ) ;
  }
  std::cout << "|--[ " << values << " ]" << std::endl << std::endl ;
}

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

  std::vector<std::pair<uint64_t, uint64_t>> 
  input_vec = 
  { { 5, 1 }, { 5, 1 }, { 3, 2 }, { 3, 2 }
  , { 8, 3 }, { 5, 3 }, { 2, 4 }, { 1, 5 }
  , { 10, 6 }, { 0, 7 }
  } ;

  for( auto in_itr = input_vec.begin() ; in_itr != input_vec.end() ; ++in_itr )
  {
    auto itr = vec.insert( in_itr->first ) ;
    to_stdout( vec, string::sprintf( "insert( %lu ) -> size( %lu )", in_itr->first, in_itr->second ) ) ;

    BOOST_CHECK_MESSAGE
    ( itr != vec.end()
    , string::sprintf( "\n\tinsert( %lu ) unexpectedly returned end()", in_itr->first ) 
    ) ; 

    BOOST_CHECK_MESSAGE
    ( *itr == in_itr->first
    , string::sprintf( "\n\tinsert( %lu ) :: result value != expected value (%lu != %lu)"
                     , in_itr->first 
                     , *itr 
                     , in_itr->second
                     ) 
    ) ; 

    BOOST_CHECK_MESSAGE
    ( vec.size() == in_itr->second
    , string::sprintf( "\n\tinsert( %lu ) :: Result size != expected size (%u != %u)" 
                     , in_itr->first, vec.size(), static_cast<uint32_t>( in_itr->second )
                     ) 
    ) ; 
  }
  
  vec.clear() ;
  to_stdout( vec, "vec.clear()" ) ;

  BOOST_CHECK_MESSAGE
  ( vec.empty() 
  , "\n\tContainer not empty() after calling clear()"
  ) ;

  /*
  { 
    vec_t::value_t i_value = 2 ;
    auto           i_itr   = vec.insert( i_value ) ;
    to_stdout( vec, string::sprintf( "Insert %lu", i_value ) ) ;

    BOOST_CHECK_MESSAGE
    ( i_itr != vec.end() 
    , string::sprintf( "\n\tSortedIntegralVector::insert( %lu ) returned end()", i_value ) 
    ) ;

    BOOST_CHECK_MESSAGE
    ( vec.size() == 1
    , string::sprintf( "\n\tSortedIntegralVector::size() returned %u, expected 1", vec.size() ) 
    ) ;

    ++i_value ;
    i_itr = vec.insert( i_value ) ;
    to_stdout( vec, string::sprintf( "Insert %lu", i_value ) ) ;
    BOOST_CHECK_MESSAGE
    ( vec.size() == 2
    , string::sprintf( "\n\tSortedIntegralVector::size() returned %u, expected 2", vec.size() ) 
    ) ;

    i_itr = vec.insert( i_value ) ;
    to_stdout( vec, string::sprintf( "Insert %lu", i_value ) ) ;
    BOOST_CHECK_MESSAGE
    ( vec.size() == 2
    , string::sprintf( "\n\tSortedIntegralVector::size() returned %u, expected 2", vec.size() ) 
    ) ;

    i_value = 1 ;
    i_itr = vec.insert( i_value ) ;
    to_stdout( vec, string::sprintf( "Insert %lu", i_value ) ) ;
    BOOST_CHECK_MESSAGE
    ( vec.size() == 3
    , string::sprintf( "\n\tSortedIntegralVector::size() returned %u, expected 3", vec.size() ) 
    ) ;
    i_value = 3 ;
  }
  */
}

