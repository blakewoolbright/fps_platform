#include "fps_json/fps_json.h" 
#include "fps_string/fps_string.h"
#include <iostream>

using namespace fps ;

int 
main( int argc, char * argv[] ) 
{
  // json::Schema schema ;
  // schema.  

  std::string json_src = "{ \"array_var\" : [ 1, 2, 3, 4 ] }" ;

  std::cout << "[ json::Document::deserialize( '" << json_src << "' )" << std::endl ;
  json::Document j_doc ;
  if( !j_doc.deserialize( json_src ) ) 
  { std::cout << "  Error  :: Failed to parse input string." << std::endl 
              << "  Reason :: " << j_doc.error_msg() << std::endl 
              << std::endl ;
    return EXIT_FAILURE ;
  }

  std::cout << j_doc.serialize() << std::endl ;

  

  return 0 ;
}
