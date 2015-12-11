#ifndef FPS__STRING__FORMAT__H
#define FPS__STRING__FORMAT__H

#include <unistd.h>
#include <cctype>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/case_conv.hpp>

namespace fps {
namespace string {

  //-----------------------------------------------------------------------
  // Return a std::string instance containing the sprintf'd result of 
  // formatting the provided argument list.
  // If there is an error, an empty string is returned.
  //-----------------------------------------------------------------------
  std::string sprintf( const char * fmt, ... ) ;

  //-----------------------------------------------------------------------
  // Expand sprintf style format characters in const char * "fmt" argument 
  // into the destination std::string instance.  The 'format' function 
  // replaces the content of the input string with the result of the 
  // sprintf conversion.  The 'append' version appends the result of 
  // the conversion to the existing content in the destination string
  //-----------------------------------------------------------------------
  std::string & format ( std::string & dest, const char * fmt, ... ) ;
  std::string & append ( std::string & dest, const char * fmt, ... ) ;

  //-----------------------------------------------------------------------
  // Check to see whether a string starts with the indicated prefix
  //-----------------------------------------------------------------------
  inline 
  bool
  starts_with( const std::string & str, const std::string & prefix ) 
  { 
    return str.find( prefix ) == 0 ;
  } 

  //-----------------------------------------------------------------------
  inline 
  bool
  starts_with( const std::string & str, const char * prefix ) 
  { 
    return str.find( prefix ) == 0 ;
  }

  //-----------------------------------------------------------------------
  inline 
  bool
  starts_with( const std::string & str, char prefix  ) 
  { 
    return !str.empty() && str[ 0 ] == prefix ;
  }

  //-----------------------------------------------------------------------
  // Check to see whether a string ends with the indicated suffix
  //-----------------------------------------------------------------------
  inline 
  bool
  ends_with( const std::string & str, const std::string & suffix ) 
  { 
    return !suffix.empty() &&
           str.rfind( suffix ) == ( str.size() - suffix.size() ) 
           ;
  }
 
  //-----------------------------------------------------------------------
  inline 
  bool
  ends_with( const std::string & str, const char * suffix ) 
  { 
    return suffix && 
           str.rfind( suffix ) == (str.size() - ::strlen( suffix ) ) 
           ;
  }

  //-----------------------------------------------------------------------
  inline 
  bool
  ends_with( const std::string & str, char suffix ) 
  { 
    return !str.empty() && str[ str.size() - 1 ] == suffix ;
  }

  //-----------------------------------------------------------------------
  // Strip whitespace characters from start or end of string (or both) 
  //
  // The 'stripped' versions of these functions return a stripped copy
  // of the input string.  The 'strip' versions modify the string 
  // argument in place
  //-----------------------------------------------------------------------
  std::string & l_strip   ( std::string & str ) ; // left strip
  std::string & r_strip   ( std::string & str ) ; // right strip
  std::string & strip     ( std::string & str ) ;
  std::string   l_stripped( const std::string & str ) ; 
  std::string   r_stripped( const std::string & str ) ; 
  std::string   stripped  ( const std::string & str ) ;

  //-----------------------------------------------------------------------
  // Split the input string into a container of substrings delimited by
  // any character in the 'delim_chars' argument
  //-----------------------------------------------------------------------
  template<typename T = std::vector<std::string>> 
  inline
  T
  split( const std::string & str, const std::string & delim_chars, bool compress=true ) 
  {
    T rv ;
    auto compress_type = compress 
                       ? boost::algorithm::token_compress_on 
                       : boost::algorithm::token_compress_off 
                       ;
    boost::split( rv, str, boost::is_any_of( delim_chars ), compress_type ) ;
    return rv ;
  }

  //-----------------------------------------------------------------------
  // Join a container full of substrings into a single string w/ the 
  // indicated delimiter
  //-----------------------------------------------------------------------
  template<typename T> 
  inline
  std::string 
  join( const T & container, const std::string & delim ) 
  { return boost::algorithm::join( container, delim.c_str() ) ;
  }

  //-----------------------------------------------------------------------
  // Implementation/detail namespace
  //-----------------------------------------------------------------------
  namespace detail 
  { 
    bool format( std::string & dest
               , uint32_t      offset
               , const char  * fmt
               , va_list       args 
               ) ;
  } 

}}

#endif
