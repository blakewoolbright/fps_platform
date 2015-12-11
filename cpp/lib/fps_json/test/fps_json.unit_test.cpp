#define BOOST_TEST_MODULE fps_json__unit_tests

#include "fps_json/fps_json.h"
#include "fps_string/fps_string.h"

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>


using namespace fps ;

BOOST_AUTO_TEST_CASE( fps_json ) 
{
  std::string b_tag   = "B0001.D" ;
  std::string b_ts    = "2015-11-28 22:39:39 PST" ;
  std::string b_user  = "blake" ; 
  std::string b_host  = "Blake-Laptop" ;
  std::string b_type  = "debug" ;
  std::string b_link  = "static" ;
  std::string b_cc    = "gcc-4.8.4" ;

  std::string json_str ; 
  string::format( json_str
                , "{ \"build_info\" :\n"
                  "  { \"tag\"       : \"%s\"\n"
                  "  , \"timestamp\" : \"%s\"\n"
                  "  , \"user\"      : \"%s\"\n"
                  "  , \"host\"      : \"%s\"\n"
                  "  , \"type\"      : \"%s\"\n"
                  "  , \"linkage\"   : \"%s\"\n"
                  "  , \"compiler\"  : \"%s\"\n"
                  "  }\n"
                  "}\n"
                , b_tag.c_str()
                , b_ts.c_str()
                , b_user.c_str()
                , b_host.c_str()
                , b_type.c_str() 
                , b_link.c_str() 
                , b_cc.c_str() 
                ) ;
                    

  std::cout << "[ fps_json : deserialize() test ]" << std::endl ;
  json::Document bi_doc ;
  bool ok = bi_doc.deserialize( json_str ) ;
  if( !ok ) 
    std::cout << bi_doc.error_msg() << std::endl ;
  BOOST_CHECK( ok ) ;

  json::Value bi_node = bi_doc.root().find( "build_info" ) ;
  BOOST_CHECK( bi_node.valid() ) ;

  std::cout << "|--[ build_info.tag       : '" << bi_node.find( "tag" ).data() << "'" << std::endl 
            << "|--[ build_info.timestamp : '" << bi_node.find( "timestamp" ).data() << "'" << std::endl 
            << "|--[ build_info.user      : '" << bi_node.find( "user" ).data() << "'" << std::endl 
            << "|--[ build_info.host      : '" << bi_node.find( "host" ).data() << "'" << std::endl 
            << "|--[ build_info.type      : '" << bi_node.find( "type" ).data() << "'" << std::endl 
            << "|--[ build_info.linkage   : '" << bi_node.find( "linkage" ).data() << "'" << std::endl 
            << "|--[ build_info.compiler  : '" << bi_node.find( "compiler" ).data() << "'" << std::endl 
            << "|" << std::endl ;

  BOOST_CHECK( bi_node.find( "tag" ).data() == b_tag ) ;
  BOOST_CHECK( bi_node.find( "timestamp" ).data() == b_ts ) ;
  BOOST_CHECK( bi_node.find( "user" ).data() == b_user ) ;
  BOOST_CHECK( bi_node.find( "host" ).data() == b_host ) ;
  BOOST_CHECK( bi_node.find( "type" ).data() == b_type ) ;
  BOOST_CHECK( bi_node.find( "linkage" ).data() == b_link ) ;
  BOOST_CHECK( bi_node.find( "compiler" ).data() == b_cc ) ;
}


