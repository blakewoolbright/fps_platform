#ifndef FPS__UTIL__SEND_EMAIL__H
#define FPS__UTIL__SEND_EMAIL__H

#include <string>

namespace fps {
namespace util {

  //----------------------------------------------------------------------------------------------------------------
  // send_email
  // 
  // Send an email to one or more recipients.
  //
  // to_addrs   : One or more destination email addresses delimited by semicolons (;)
  // from_addr  : Sender's email address
  // subject    : Subject of message
  // body       : Content of message
  // command    : Shell command to execute to send the message ( default: "/usr/sbin/sendmail -t" )
  //
  // Throws     : fps::except::RuntimeError() 
  //              ( if 'to_addrs', 'from_addr', 'command', or both 'subject' and 'body' are empty )
  //
  //----------------------------------------------------------------------------------------------------------------
  bool send_email( const std::string & to_addrs
                 , const std::string & from_addr 
                 , const std::string & subject
                 , const std::string & body 
                 , const std::string & command = std::string( "/usr/sbin/sendmail -t" ) 
                 ) ;


}}

#endif
