#define BOOST_TEST_MODULE fps_util__unit_tests

#include "fps_json/fps_json.h"
#include "fps_string/fps_string.h"
#include "fps_util/build_info.h"
#include "fps_util/singleton.h"
#include "fps_util/byte_queue.h"

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>

using namespace fps ;

//---------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_util__build_info ) 
{
  std::cout << "[ util::BuildInfo unit tests ]" << std::endl ;
  BOOST_CHECK_MESSAGE( util::BuildInfo::Is_Available
                     , "\n\tfps::util::BuildInfo :: Embedded build information not present in binary" 
                     ) ;
  
  util::BuildInfo & bi = util::Singleton<util::BuildInfo>::instance() ;
  BOOST_CHECK_MESSAGE( bi.valid() 
                     , "\n\tfps::util::BuildInfo :: Build info present but invalid"
                     ) ;

  std::cout << "|--[ build_info.tag       : '" << bi.tag() << "'" << std::endl 
            << "|--[ build_info.timestamp : '" << bi.timestamp() << "'" << std::endl 
            << "|--[ build_info.user      : '" << bi.user() << "'" << std::endl 
            << "|--[ build_info.host      : '" << bi.host() << "'" << std::endl 
            << "|--[ build_info.type      : '" << bi.type() << "'" << std::endl 
            << "|--[ build_info.linkage   : '" << bi.linkage() << "'" << std::endl 
            << "|--[ build_info.compiler  : '" << bi.compiler() << "'" << std::endl 
            << "|" << std::endl ;
  
}

//---------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_util__byte_queue ) 
{
  std::cout << "[ util::ByteQueue unit tests ]" << std::endl ;
  
  uint32_t Capacity = 1024 ;
  fps::ByteQueue bq( Capacity ) ;
  
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
    fps::ByteRange q_data = bq.extract<extract::ByNewline>() ;
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

