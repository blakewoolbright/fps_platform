#include "fps_net/address.h"
#include <cctype>
#include <netdb.h>

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

    string::strip( rv ) ;
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

    string::strip( rv ) ;
    if( rv.empty() ) 
      return 0 ; 

    for( uint32_t idx = 0 ; idx < rv.length() ; ++idx ) 
      if( !::isdigit( rv[ idx ] ) ) return 0 ;

    return string::convert::to<uint16_t>( rv ) ;
  }

  //-----------------------------------------------------------------------------
  bool 
  Address::resolve_url( std::string url, Address & dest )
  {
    dest.clear() ;
    url = trim_url( url ) ;
  
    std::string url_host = Address::get_host_from_url( url ) ;
    uint16_t    url_port = Address::get_port_from_url( url ) ;

    if( url_host.empty() ) 
      return false ;
  
    // First, try to resolve this as if it were in standard dotted-decimal notation. 
    ::in_addr tmp_addr ;
    if( ::inet_aton( url_host.c_str(), &tmp_addr ) )
    { dest.set( util::bswap( tmp_addr.s_addr ), url_port ) ;    
      return true ;
    }

    // Next, try to resolve it as if it were a hostname
    int32_t rv     = ERANGE ;
    int32_t rv_err = 0 ;
    struct ::hostent   h_ent ;
    struct ::hostent * h_ent_ptr = NULL ;
  
    uint32_t buf_sz = 1024 ;
    char   * buf    = new char[ buf_sz ] ;
    
    while( true )
    { 
      rv = ::gethostbyname_r( url_host.c_str(), &h_ent, buf, buf_sz, &h_ent_ptr, &rv_err ) ;
      if( rv == ERANGE ) 
      { 
        delete [] buf ;
        buf_sz *= 2 ;
        buf = new char [ buf_sz ] ; 
        continue ;
      }
      break ; 
    } 
    
    if( ( rv != 0 || h_ent_ptr == NULL )  
     || ( h_ent_ptr->h_addr_list[0] == NULL ) 
     || ( h_ent_ptr->h_addrtype != AF_INET ) 
      ) 
    {
      delete [] buf ;
      return false ;
    }

    int32_t raw_addr = reinterpret_cast<const ::in_addr *>( h_ent_ptr->h_addr_list[0] )->s_addr ;
    dest.set( util::bswap( raw_addr ), url_port ) ;
    delete [] buf ;
    return true ; 
  }
  

}}
