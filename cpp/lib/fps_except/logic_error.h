#ifndef FPS__EXCEPT__LOGIC_ERROR__H
#define FPS__EXCEPT__LOGIC_ERROR__H

#include "fps_except/exception.h"

namespace fps {
namespace except {

  //--------------------------------------------------------------------------
  class LogicError : public Exception
  {
  public :
    //-----------------------------------------
    LogicError() = delete ;

    //-----------------------------------------
    LogicError( const char * fmt, ... ) ;
    virtual ~LogicError() throw() ;
  } ;

}}

#endif
