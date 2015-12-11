#ifndef FPS__EXCEPT__EXCEPTION_BASE__H
#define FPS__EXCEPT__EXCEPTION_BASE__H

#include "fps_string/format.h"
#include <unistd.h>
#include <cstdarg>
#include <stdexcept>
#include <string>

namespace fps {
namespace except {

  //--------------------------------------------------------------------------
  // Generic base class for internal exceptions.
  // Supports printf like syntax for formatting exception text.
  //
  // Don't instantiate this class directly if possible, instead, derive from it
  // as per runtime_error.h and logic_error.h, found in this directory.
  //--------------------------------------------------------------------------
  class Exception 
    : public std::exception 
  {
  protected :
    std::string msg_ ;

  public :
    //------------------------------------------------------------------------
    inline Exception() {} 

    //------------------------------------------------------------------------
    Exception( const char * fmt, ... ) ;
    virtual ~Exception() throw() ;

    //------------------------------------------------------------------------
    inline virtual const char * what() const throw() { return msg_.c_str() ; }
  } ;
}}

#endif
