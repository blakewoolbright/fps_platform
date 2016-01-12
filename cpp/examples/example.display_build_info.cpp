#include "fps_string/fps_string.h"
#include "fps_build_info/fps_build_info.h"
#include "fps_util/singleton.h"

#include <iostream>

using namespace fps ;

//-------------------------------------------------------------------------------------------
// Applications built by fpsmake carry a JSON encoded fingerprint in the .data ELF 
// segment that can be examined to determine when, where, and how the application was created.  
//
// The fps_util library's fps::util::BuildInfo exposes a simple mechanism for working
// with this embedded content.  The following example demonstrates it's usage.
//-------------------------------------------------------------------------------------------
int main( int argc, char * argv[] )
{
  util::BuildInfo & b_nfo = util::Singleton<util::BuildInfo>::instance() ;
  
  std::cout << "[ fps::BuildInfo ]" << std::endl 
            << "|" << std::endl 
            << "|--[ Is_Available : " << (b_nfo.Is_Available ? "true" : "false") << " ]" << std::endl 
            << "|--[ Tag          : " << b_nfo.tag() << " ]" << std::endl 
            << "|--[ Timestamp    : " << b_nfo.timestamp() << " ]" << std::endl 
            << "|--[ Type/Opt     : " << b_nfo.type() << " ]" << std::endl 
            << "|--[ Linkage      : " << b_nfo.linkage() << " ]" << std::endl 
            << "|--[ Host         : " << b_nfo.host() << " ]" << std::endl 
            << "|--[ User         : " << b_nfo.user() << " ]" << std::endl 
            << std::endl ;

  std::cout << "[ Raw content of ELF section ]" << std::endl 
            << b_nfo.to_json() << std::endl
            << std::endl ;

  return 0 ;
}

