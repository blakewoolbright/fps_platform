#define BOOST_TEST_MODULE fps__container

#include "fps_container/byte_queue.h"
#include "fps_container/detail/sorted_vector.h"
#include "fps_container/detail/sorted_integral_vector.h"
#include "fps_string/fps_string.h"

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>

using namespace fps ;

//---------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_container__byte_queue ) 
{
  std::cout << "[ util::ByteQueue unit tests ]" << std::endl ;
  
  uint32_t Capacity = 1024 ;
  container::ByteQueue bq( Capacity ) ;
  
  BOOST_CHECK_MESSAGE
  ( bq.empty() 
  , "\n\tByteQueue::empty() should return true after construction" 
  ) ;

  BOOST_CHECK_MESSAGE
  ( (bq.capacity() < Capacity ) 
  , string::sprintf( "\n\tByteQueue::capacity() returned %u - expected %u"
                  , bq.capacity()
                  , Capacity 
                  ) 
  ) ;

  std::string content( "abc\ndef\nghi\njkl\n" ) ;
  bq.insert( content.c_str(), content.size() ) ;
  
  BOOST_CHECK_MESSAGE
  ( bq.size() == content.size() 
  , string::sprintf( "\n\tByteQueue::size() returned %u - expected %u"
                   , bq.size()
                   , content.size() 
                   )
  ) ;

  bq.insert( content.c_str(), content.size() ) ;
  
  BOOST_CHECK_MESSAGE
  ( bq.size() == (content.size() * 2)
  , string::sprintf( "\n\tByteQueue::size() returned %u - expected %u"
                   , bq.size()
                   , (content.size() * 2)
                   )
  ) ;

  std::vector<std::string> dq_vec ;
  while( !bq.empty() ) 
  {
    container::ByteRange q_data = bq.extract<container::extract::ByNewline>() ;
    if( !q_data ) 
      break ;

    dq_vec.push_back( std::string( q_data.begin(), q_data.begin() + q_data.size() ) ) ;
  }

  BOOST_CHECK_MESSAGE
  ( dq_vec.size() == (4 * 2)
  , string::sprintf( "\n\tByteQueue::dequeue<\n>() returned %u items - expected %u items"
                   , static_cast<uint32_t>( dq_vec.size() )
                   , (4 * 2)
                   )
  ) ;
  
  BOOST_CHECK_MESSAGE
  ( bq.empty() 
  , "\n\tByteQueue::empty() should return true after all dequeing all content" 
  ) ;
                  
}

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
BOOST_AUTO_TEST_CASE( fps_container__sorted_vector )
{
  typedef container::detail::SortedIntegralVector<uint64_t> vec_t ;

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

  //----------------------------------------------
  // Input values & expected post install size 
  //----------------------------------------------
  std::vector<std::pair<uint64_t, uint64_t>> 
  input_vec = 
  { { 5, 1 }, { 5, 1 }, { 3, 2 }, { 3, 2 }
  , { 8, 3 }, { 5, 3 }, { 2, 4 }, { 1, 5 }
  , { 10, 6 }, { 0, 7 }
  } ;

  //----------------------------------------------
  // Populate container
  //----------------------------------------------
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

  //----------------------------------------------
  // Erase container entries
  //----------------------------------------------
  {
    uint32_t last_sz   = vec.size() ;
    uint64_t erase_val = 5 ;
    vec.erase( erase_val ) ;
    to_stdout( vec, string::sprintf( "erase( %lu ) -> size( %lu )", erase_val, vec.size() ) ) ;
    BOOST_CHECK_MESSAGE
    ( vec.size() == (last_sz-1)
    , string::sprintf( "\n\terase( %lu ) :: Result size != expected size ( %u != %u )"
                     , erase_val, vec.size(), last_sz 
                     )
    ) ;
    
    erase_val = 0 ;
    last_sz = vec.size() ;
    vec.erase( erase_val ) ;
    to_stdout( vec, string::sprintf( "erase( %lu ) -> size( %lu )", erase_val, vec.size() ) ) ;
    BOOST_CHECK_MESSAGE
    ( vec.size() == (last_sz-1)
    , string::sprintf( "\n\terase( %lu ) :: Result size != expected size ( %u != %u )"
                     , erase_val, vec.size(), last_sz 
                     )
    ) ;

    erase_val = 2 ;
    last_sz = vec.size() ;
    vec.erase( erase_val ) ;
    to_stdout( vec, string::sprintf( "erase( %lu ) -> size( %lu )", erase_val, vec.size() ) ) ;
    BOOST_CHECK_MESSAGE
    ( vec.size() == (last_sz-1)
    , string::sprintf( "\n\terase( %lu ) :: Result size != expected size ( %u != %u )"
                     , erase_val, vec.size(), last_sz 
                     )
    ) ;
    --last_sz ; 
  }
  
  //----------------------------------------------
  // Clear
  //----------------------------------------------
  vec.clear() ;
  to_stdout( vec, "vec.clear()" ) ;

  BOOST_CHECK_MESSAGE
  ( vec.empty() 
  , "\n\tContainer not empty() after calling clear()"
  ) ;
}

