#ifndef FPS__JSON__DETAIL__TYPES__H
#define FPS__JSON__DETAIL__TYPES__H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace fps  {
namespace json { 

  //-------------------------------------------------------------------------------
  namespace detail 
  { 
    //-----------------------------------------------------------------------------
    typedef boost::property_tree::ptree             tree_t ;
    typedef boost::property_tree::json_parser_error parser_exc_t ;

    //-----------------------------------------------------------------------------
    using boost::property_tree::read_json ;
    using boost::property_tree::write_json ;
  }
}}

#endif
