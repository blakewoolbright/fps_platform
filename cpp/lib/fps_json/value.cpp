#include "value.h"
#include "detail/types.h"

#include "fps_string/fps_string.h"
#include <stdexcept>

namespace fps  {
namespace json {

  //--------------------------------------------------------------------------------------------------
  Value 
  Value::find( const std::string & key ) const
  { 
    tree_t fail_tree ;
    const tree_t & child_tree = node_->get_child( key, fail_tree ) ;
    return ( &child_tree != &fail_tree ) 
           ? Value( key, child_tree ) 
           : Value()
           ;
  }

  /*
  //----------------------------------------------------------------------------
  std::string 
  Value::serialize() const
  {
    std::stringstream s_str ;
    try 
    {
      detail::write_json( s_str, node_ ) ;
    }
    catch( const detail::parser_exc_t & exc ) 
    {
      throw std::runtime_error
            ( string::sprintf( "Serialization Error"
                               "  Hint : '%s'\n"
                               "  Line : %lu\n"
                             , exc.message().c_str()
                             , exc.line() 
                             ) 
            ) ;
    }
    
    return std::string( s_str.str() ) ;
  }


  //--------------------------------------------------------------------------
  void 
  Value::deserialize( const std::string & json_input ) 
  {
    node_.clear() ;
    std::stringstream json_stream ; 
    json_stream << json_input ;

    try 
    { detail::read_json( json_stream, node_ ) ; 
    }
    catch( const detail::parser_exc_t & exc ) 
    { 
      node_.clear() ;

      throw 
      std::runtime_error
      ( string::sprintf( "Deserialization error\n"
                         "  Hint : '%s'\n"
                         "  Line : %lu\n"
                       , exc.message().c_str()
                       , exc.line() 
                       ) 
      ) ;
    }

    return true ;
  }
  */

}}
