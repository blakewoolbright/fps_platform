#include "format.h"
#include <cstring>
#include <iostream>
#include <stdarg.h>
#include <boost/algorithm/string/trim.hpp>

namespace fps {
namespace string {
  //------------------------------------------------------------------------------------
  std::string &
  l_strip( std::string & str ) 
  { 
    boost::algorithm::trim_left( str ) ;
    return str ;
  }

  //------------------------------------------------------------------------------------
  std::string &
  r_strip( std::string & str ) 
  { 
    boost::algorithm::trim_right( str ) ;
    return str ;
  }

  //------------------------------------------------------------------------------------
  std::string &
  strip( std::string & str ) 
  { 
    boost::algorithm::trim( str ) ;
    return str ;
  }

  //------------------------------------------------------------------------------------
  std::string 
  l_stripped( const std::string & str ) 
  { 
    return boost::algorithm::trim_left_copy( str ) ;
  }

  //------------------------------------------------------------------------------------
  std::string 
  r_stripped( const std::string & str ) 
  { 
    return boost::algorithm::trim_right_copy( str ) ;
  }

  //------------------------------------------------------------------------------------
  std::string 
  stripped( const std::string & str ) 
  { 
    return boost::algorithm::trim_copy( str ) ;
  }

  //------------------------------------------------------------------------------------
  namespace detail 
  { 
    bool
    format( std::string & rv, uint32_t offset, const char * fmt, va_list args ) 
    {
      va_list  tmp_args ;
      uint32_t fmt_size = std::strlen( fmt ) ;
      uint32_t in_size  = rv.size() ;
  
      rv.resize( rv.size() + (fmt_size * 4 ) + 1 ) ;

      for( uint32_t loop_idx = 1 ; loop_idx < 10 ; ++loop_idx ) 
      { 
        int32_t  max_size = rv.size() - offset  ;
        va_copy( tmp_args, args ) ;
        size_t req_size = ::vsnprintf( &(rv[offset]), max_size, fmt, tmp_args ) ;

        // A negative return value means that there was insufficient buffer
        // space or that an internal error was encountered.
        if( req_size <= 0 )
        { rv.resize( rv.size() * 2 ) ;
        }
        // A return value that's greater than max size indicates insufficient 
        // buffer space but the return value should provide a hint for us to use
        // when resizing our destination buffer.
        else if( req_size > max_size )
        { rv.resize( rv.size() + req_size ) ;
        }
        // A return value that's less than or equal to our max size indicates success
        else 
        {   
          rv.resize( in_size + req_size ) ;
          return true ;
        }
      }
      return false ;
    }
  } 

  //-------------------------------------------------------------------------------------------
  std::string 
  sprintf( const char * fmt, ... ) 
  {
    std::string rv ;
    if( fmt == NULL ) 
      return rv ;

    va_list arg_list ;
    va_start( arg_list, fmt ) ;
    detail::format( rv, 0, fmt, arg_list ) ;
    va_end( arg_list ) ;

    return rv ;
  }

  //-------------------------------------------------------------------------------------------
  std::string &
  format( std::string & rv, const char * fmt, ... ) 
  {
    rv.clear() ;
    if( fmt == NULL ) 
      return rv ;

    va_list arg_list ;
    va_start( arg_list, fmt ) ;
    detail::format( rv, 0, fmt, arg_list ) ;
    va_end( arg_list ) ;

    return rv ;
  }

  //-------------------------------------------------------------------------------------------
  std::string &
  append( std::string & rv, const char * fmt, ... ) 
  {
    if( fmt == NULL ) 
      return rv ;

    va_list arg_list ;
    va_start( arg_list, fmt ) ;
    detail::format( rv, rv.size(), fmt, arg_list ) ;
    va_end( arg_list ) ;

    return rv ;
  }
}}

