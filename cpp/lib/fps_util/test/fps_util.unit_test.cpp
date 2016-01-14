#define BOOST_TEST_MODULE fps_util__unit_tests

#include "fps_json/fps_json.h"
#include "fps_string/fps_string.h"
#include "fps_util/singleton.h"
#include "fps_util/byte_queue.h"

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>

using namespace fps ;

//---------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_util__byte_queue ) 
{
  std::cout << "[ util::ByteQueue unit tests ]" << std::endl ;
  
  uint32_t Capacity = 1024 ;
  util::ByteQueue bq( Capacity ) ;
  
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
    util::ByteRange q_data = bq.extract<extract::ByNewline>() ;
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

