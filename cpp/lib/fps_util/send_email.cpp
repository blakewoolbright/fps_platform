#include "fps_util/send_email.h"
#include "fps_string/fps_string.h"
#include "fps_except/fps_except.h"

#include <errno.h>
#include <cstdio>

namespace fps {
namespace util {

  //----------------------------------------------------------------------------------------------------------------
  bool 
  send_email( const std::string & shell_command 
            , const std::string & to_addrs
            , const std::string & from_addr 
            , const std::string & subject
            , const std::string & body 
            , const std::string & command 
            )
  {
    std::string content ;
    content.reserve( body.length() 
                   + subject.length() 
                   + from_addr.length() 
                   + to_addrs.length() 
                   + 128
                   ) ;

    if( to_addrs.empty() )
      throw except::RuntimeError( "send_email :: No recipient addresses found" ) ;

    if( from_addr.empty() ) 
      throw except::RuntimeError( "send_email :: No sender address found" ) ;

    if( subject.empty() && body.empty() ) 
      throw except::RuntimeError( "send_email :: No subject and no body found" ) ;

    if( command.empty() )
      throw except::RuntimeError( "send_email :: No shell command (eg. /usr/sbin/sendmail -t) found" ) ;

    // To       ( "To: <text>" )
    content.assign( "To: " ) ;
    content += to_addrs ; 

    // From     ( "\nFrom: <text>" )
    content.push_back( '\n' ) ;
    content += "From: " ;
    content += from_addr ;

    // Subject  ( "\nSubject: <text>" )
    content.push_back( '\n' ) ;
    content += "Subject: " ;
    content +=  subject ;

    // Body     ( "\n\n<text>" )
    content.push_back( '\n' ) ;
    content.push_back( '\n' ) ;
    content += body ;

    // Terminal ( "\n.\n" )
    content.push_back( '\n' ) ;
    content.push_back( '.' ) ;
    content.push_back( '\n' ) ;

    FILE * ph = ::popen( command.c_str(), "w" ) ;
    if( !ph )
    { throw 
      except::RuntimeError( "send_email :: Error in popen() w/ input '%s' (errno: %d)" 
                          , command.c_str()
                          , errno 
                          ) ;
    }

    int32_t  bytes_out   = 0 ; 
    uint32_t max_retries = 10 ;
    uint32_t cur_retries = 0 ;
    while( cur_retries < max_retries ) 
    { 
      bytes_out += ::fwrite( content.c_str() + bytes_out
                           , 1
                           , content.length() - bytes_out
                           , ph 
                           ) ;
      if( bytes_out < content.length() ) 
        ++cur_retries ;
      else 
        break ;
    }
    ::pclose( ph ) ;

    return ( bytes_out >= content.length() ) ;
  }

}}
