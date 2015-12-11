#include "fps_except/logic_error.h"
#include "fps_string/format.h"

namespace fps {
namespace except {

  //-------------------------------------------------------
  LogicError::LogicError( const char * fmt, ... ) 
    : Exception( "LogicError : " )
  { 
    va_list va_args ;
    va_start              ( va_args, fmt ) ;
    string::detail::format( msg_, msg_.length(), fmt, va_args ) ;
    va_end                ( va_args ) ;
  }

  //-------------------------------------------------------
  LogicError::~LogicError() {} 

}}
