#include "fps_json/document.h"
#include "fps_string/fps_string.h"
#include "fps_fs/file.h"
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>

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
    return false ;
  }

  //----------------------------------------------------------------------------
  bool 
  Document::save( const std::string & filename ) const 
  {
    std::string out_data = serialize() ;
    if( out_data.empty() && !err_text_.empty() ) 
      return false ;

    fs::File out_file( filename, "wb" ) ;
    if( !out_file.is_open() ) 
    { 
      string::format( err_text_
                    , "Failed to open file '%s' for writing"
                    , filename.c_str() 
                    ) ;
      return false ;
    }

    // Use blocking I/O so we don't have to worry about partial writes.
    out_file.set_blocking_io( false ) ;
    int32_t w_rv = out_file.write( out_data.c_str(), out_data.size() ) ;
    if( w_rv < 0 ) 
    { 
      string::format( err_text_
                    , "Failed to write %u bytes to output file (errno: %d)"
                    , out_data.size() 
                    , errno
                    ) ;
      return false ;
    }

    return true ;
  }

  //----------------------------------------------------------------------------
  std::string
  Document::serialize() const
  {
    std::stringstream s_str ;
    try 
    {
      detail::write_json( s_str, tree_ ) ;
      return std::string( s_str.str() ) ;
    }
    catch( const detail::parser_exc_t & exc ) 
    {
      string::format( err_text_ 
                    , "Serialization Error"
                      "  Hint : '%s'\n"
                      "  Line : %lu\n"
                    , exc.message().c_str()
                    , exc.line() 
                    ) ;
    }
    return std::string() ;
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
      string::format( err_text_ 
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
