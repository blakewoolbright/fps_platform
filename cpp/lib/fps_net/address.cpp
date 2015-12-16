#include "fps_net/address.h"
#include <cctype>

namespace fps {
namespace net {

  //-----------------------------------------------------------------------------
  std::string 
  Address::trim_url( const std::string & url )
  {
    std::string rv = string::stripped( url ) ;
    int32_t find_rv = rv.find( "://" ) ;
    if( find_rv < rv.length() ) 
      rv.erase( 0, find_rv + 3 ) ;
    
    find_rv = rv.find( "&" ) ;
    if( find_rv < rv.length() ) 
      rv.erase( find_rv, rv.length() - find_rv ) ;
  
    find_rv = rv.find( "/" ) ;
    if( find_rv < rv.length() ) 
      rv.erase( find_rv, rv.length() - find_rv ) ;
  
    return rv ;
  }

  //-----------------------------------------------------------------------------
  std::string
  Address::get_host_from_url( const std::string & url ) 
  {
    std::string rv = trim_url( url ) ;
    
    int32_t find_rv = rv.find( ":" ) ;
    if( find_rv < rv.length() ) 
      rv.erase( find_rv, rv.length() - find_rv ) ;
  
    return rv ;
  }

  //-----------------------------------------------------------------------------
  uint16_t
  Address::get_port_from_url( const std::string & url ) 
  {
    std::string rv = trim_url( url ) ;
    
    int32_t find_rv = rv.find( ":" ) ;
    if( find_rv < rv.length() ) 
      rv.erase( 0, find_rv + 1 ) ;

    for( uint32_t idx = 0 ; idx < rv.length() ; ++idx ) 
      if( !::isdigit( rv[ idx ] ) ) return 0 ;

    return string::convert::to<uint16_t>( rv ) ;
  }

  //-----------------------------------------------------------------------------
  bool 
  Address::resolve( std::string url, Address & dest )
  {
    url = trim_url( url ) ;
  
    std::string url_host = Address::get_host_from_url( url ) ;
    uint16_t    url_port = Address::get_port_from_url( url ) ;

    if( url_host.empty() ) 
      return false ;
  
    ::in_addr tmp_addr ;
    if( ::inet_aton( url_host.c_str(), &tmp_addr ) )
    { dest.set( util::bswap( tmp_addr.s_addr ), url_port ) ;    
      return true ;
    }
    
    return false ;
  }

}}
