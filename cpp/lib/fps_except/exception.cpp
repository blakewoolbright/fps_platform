#include "fps_except/exception.h"

namespace fps {
namespace except {

  //-------------------------------------------------------
  Exception::Exception( const char * fmt, ... ) 
  { 
    va_list va_args ;
    va_start              ( va_args, fmt ) ;
    string::detail::format( msg_, 0, fmt, va_args ) ;
    va_end                ( va_args ) ;
  }

  //-------------------------------------------------------
  Exception::~Exception() {}
}}
