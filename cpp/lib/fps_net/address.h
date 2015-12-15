#ifndef FPS__NET__ADDRESS__H
#define FPS__NET__ADDRESS__H

#include "fps_net/detail/address_details.h"
#include "fps_string/fps_string.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


namespace fps {
namespace net {

  //-------------------------------------------------------------------------------
  class Address
  {
  private :
    detail::UAddress impl_ ; 

    //-----------------------------------------------------------------------------
    static bool resolve( const std::string & url, Address & dest ) ;

  public :
    //-----------------------------------------------------------------------------
    inline Address() ;

    //-----------------------------------------------------------------------------
    inline Address( const char * ip, uint16_t port ) ;

    //-----------------------------------------------------------------------------
    inline Address( const char * url ) ;

    //-----------------------------------------------------------------------------
    inline bool empty()       const { return impl_.empty()       ; }
    inline bool is_wildcard() const { return impl_.is_wildcard() ; }

    //-----------------------------------------------------------------------------
    inline uint32_t ip  ()    const { return impl_.decoded_ip()   ; }
    inline uint16_t port()    const { return impl_.decoded_port() ; }

    //-----------------------------------------------------------------------------
    inline uint64_t encode() const { return impl_.encoded() ; }

    //-----------------------------------------------------------------------------
    inline void decode( uint64_t value ) { impl_.decode( value ) ; }

    //-----------------------------------------------------------------------------
    inline bool to_native  ( ::sockaddr_in & dest ) const ;
    inline void from_native( const sockaddr_in & src ) ;

    //-----------------------------------------------------------------------------
    inline std::string to_string( bool show_port=false ) const ;

    //-----------------------------------------------------------------------------
    inline 
    uint8_t 
    operator[]( uint32_t octet ) const
    { 
      if( octet > 3 ) return 0 ;

      uint32_t value = impl_.decoded_ip() ;
      return reinterpret_cast<const uint8_t *>( &value )[ 3 - octet ] ;
    }
  } ;

  //-------------------------------------------------------------------------------
  Address::Address() { impl_.clear() ; }

  //-------------------------------------------------------------------------------
  Address::Address( const char * ip, uint16_t port )   
  { 
    impl_.clear() ;
    
    ::in_addr dst_addr ;
    if( 0 != ::inet_aton( ip, &dst_addr ) ) 
      impl_.encode( util::bswap( dst_addr.s_addr ), port ) ;
    else 
      impl_.encode_port( port ) ;
  }

  //-------------------------------------------------------------------------------
  bool
  Address::to_native( ::sockaddr_in & dest ) const 
  { 
    if( impl_.empty() )
      return false ;

    dest.sin_family      = AF_INET ;
    dest.sin_port        = util::bswap( impl_.decoded_port() ) ;
    dest.sin_addr.s_addr = util::bswap( impl_.decoded_ip()   ) ;
    return true ;
  }
  
  //-------------------------------------------------------------------------------
  void 
  Address::from_native( const ::sockaddr_in & src ) 
  { 
    impl_.encode( util::bswap( src.sin_addr.s_addr ), util::bswap( src.sin_port ) ) ;
  }

  //-------------------------------------------------------------------------------
  std::string
  Address::to_string( bool show_port ) const 
  {
    return show_port 
           ? string::sprintf( "%hhu.%hhu.%hhu.%hhu:%hu"
                            , (*this)[0], (*this)[1], (*this)[2], (*this)[3], port() 
                            )
           : string::sprintf( "%hhu.%hhu.%hhu.%hhu"
                            , (*this)[0], (*this)[1], (*this)[2], (*this)[3] 
                            )
           ;
  }
}}


#endif

