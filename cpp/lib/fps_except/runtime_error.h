#ifndef FPS__EXCEPT__RUNTIME_ERROR__H
#define FPS__EXCEPT__RUNTIME_ERROR__H

#include "fps_except/exception.h"

namespace fps {
namespace except {

  //--------------------------------------------------------------------------
  class RuntimeError 
    : public Exception
  {
  public :
    //------------------------------------------------------------
    RuntimeError() = delete ;
    RuntimeError( const char * fmt, ... ) ;

    //------------------------------------------------------------
    virtual ~RuntimeError() ;
  } ;

}}

#endif
