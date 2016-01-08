#include "fps_util.h"
#include <iostream>
#include <ios>

namespace fps  {
namespace util {

  //-------------------------------------------------------------------------------
  bool 
  set_stdio_synchronization( bool yes_no ) 
  {
    return std::ios_base::sync_with_stdio( yes_no ) ;
  }

}}
