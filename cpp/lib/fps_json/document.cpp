#include "fps_json/document.h"
#include "fps_string/fps_string.h"

namespace fps  {
namespace json {

  //----------------------------------------------------------------------------
  Document::Document() 
    : tree_() 
    , root_( tree_ )
  {}

  //----------------------------------------------------------------------------
  Document::~Document() 
  {}

  //----------------------------------------------------------------------------
  // TODO: Implement
  //----------------------------------------------------------------------------
  bool 
  Document::load( const std::string & filename ) 
  {
    return true ;
  }

  //----------------------------------------------------------------------------
  // TODO: Implement
  //----------------------------------------------------------------------------
  bool 
  Document::save( const std::string & filename ) const 
  {
    return false ;
  }

  //----------------------------------------------------------------------------
  std::string
  Document::serialize() const
  {
    std::stringstream s_str ;
    try 
    {
      detail::write_json( s_str, tree_ ) ;
    }
    catch( const detail::parser_exc_t & exc ) 
    {
      string::format( text_ 
                    , "Serialization Error"
                      "  Hint : '%s'\n"
                      "  Line : %lu\n"
                    , exc.message().c_str()
                    , exc.line() 
                    ) ;
    }
    
    return std::string( s_str.str() ) ;
  }

  //--------------------------------------------------------------------------
  bool 
  Document::deserialize( const std::string & json_data ) 
  {
    tree_.clear() ;
    std::stringstream json_stream ; 
    json_stream << json_data ;

    try 
    { 
      detail::read_json( json_stream, tree_ ) ; 
    }
    catch( const detail::parser_exc_t & exc ) 
    { 
      string::format( text_ 
                    , "Deserialization Error\n"
                      "  Hint : '%s'\n"
                      "  Line : %lu\n"
                    , exc.message().c_str()
                    , exc.line() 
                    ) ;

      return false ;
    }

    return true ;
  }


}}
