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
    inline void set( uint32_t ip, uint16_t port ) { impl_.encode( ip, port ) ; }

  public :
    //-----------------------------------------------------------------------------
    static bool        resolve_url( std::string url, Address & dest ) ;
    static std::string trim_url   ( const std::string & url ) ;

    //-----------------------------------------------------------------------------
    static uint16_t    get_port_from_url( const std::string & url ) ;
    static std::string get_host_from_url( const std::string & url ) ;

  public :
    //-----------------------------------------------------------------------------
    inline Address() ;

    //-----------------------------------------------------------------------------
    inline Address( const char * ip, uint16_t port ) ;

    //-----------------------------------------------------------------------------
    inline explicit Address( const char * url ) ;
    inline explicit Address( const std::string & url ) ;

    //-----------------------------------------------------------------------------
    inline uint8_t operator[]( uint32_t octet ) const ;

    //-----------------------------------------------------------------------------
    inline bool empty()       const { return impl_.empty()       ; }
    inline bool is_wildcard() const { return impl_.is_wildcard() ; }

    //-----------------------------------------------------------------------------
    inline uint32_t ip  () const { return impl_.decoded_ip()   ; }
    inline uint16_t port() const { return impl_.decoded_port() ; }

    //-----------------------------------------------------------------------------
    inline uint64_t encode() const { return impl_.encoded() ; }

    //-----------------------------------------------------------------------------
    inline void decode( uint64_t value ) { impl_.decode( value ) ; }

    //-----------------------------------------------------------------------------
    inline bool to_native  ( ::sockaddr_in & dest ) const ;
    inline void from_native( const sockaddr_in & src ) ;

    //-----------------------------------------------------------------------------
    inline void clear() { impl_.clear() ; }

    //-----------------------------------------------------------------------------
    inline std::string to_string( bool show_port=false ) const ;
  } ;

  //-------------------------------------------------------------------------------
  Address::Address() { impl_.clear() ; }

  //-------------------------------------------------------------------------------
  Address::Address( const char * ip, uint16_t port )   
  { 
    if( resolve_url( std::string( ip ), *this ) )
      impl_.encode_port( port ) ;
    else 
      clear() ;  // TODO: Throw exception?
  }

  //-------------------------------------------------------------------------------
  Address::Address( const char * url )
  { 
    if( !resolve_url( std::string( url ), *this ) )
      clear() ;  // TODO: Throw exception?
  }

  //-------------------------------------------------------------------------------
  Address::Address( const std::string & url )
  { 
    if( !resolve_url( url, *this ) )
      clear() ;  // TODO: Throw exception?
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

  //-----------------------------------------------------------------------------
  inline 
  uint8_t 
  Address::operator[]( uint32_t octet ) const
  { 
    if( octet > 3 ) return 0 ;

    uint32_t value = impl_.decoded_ip() ;
    return reinterpret_cast<const uint8_t *>( &value )[ 3 - octet ] ;
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

