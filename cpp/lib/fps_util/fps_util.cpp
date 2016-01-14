#include "fps_util/fps_util.h"
#include "fps_util/bswap.h"
#include "fps_util/signal.h"
#include "fps_util/singleton.h"

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
