#include "fps_except/runtime_error.h"
#include "fps_string/format.h"

namespace fps {
namespace except {

  //-------------------------------------------------------
  RuntimeError::RuntimeError( const char * fmt, ... ) 
    : Exception( "RuntimeError : " )
  { 
    va_list va_args ;
    va_start              ( va_args, fmt ) ;
    string::detail::format( msg_, msg_.length(), fmt, va_args ) ;
    va_end                ( va_args ) ;
  }

  //-------------------------------------------------------
  RuntimeError::~RuntimeError() {} 

}}
