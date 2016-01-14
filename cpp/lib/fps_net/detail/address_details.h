#ifndef FPS__NET__DETAIL__ADDRESS_DETAILS__H
#define FPS__NET__DETAIL__ADDRESS_DETAILS__H

#include "fps_util/bswap.h"

#include <cstdint>
#include <type_traits>
#include <string>
#include <cstring>
#include <limits>

namespace fps {
namespace net {
namespace detail {

  //-------------------------------------------------------------------------------
  union UAddress
  {
  public :
    //-----------------------------------------------------------------------------
    struct Decoded 
    {
    private :
      uint32_t ip_   ;
      int32_t  port_ ;

    public :

      //------------------------------------------------------------
      inline uint32_t ip()   const { return ip_   ; }
      inline uint16_t port() const { return port_ ; }

      //------------------------------------------------------------
      inline bool empty() const { return port_ < 0 ; }

      //------------------------------------------------------------
      inline 
      bool 
      is_wildcard() const 
      { 
        return ip_ == 0 && port_ >= 0 ; 
      } 

      //------------------------------------------------------------
      inline void clear() { ip_ = 0 ; port_ = -1 ; }
      
      //------------------------------------------------------------
      inline 
      void 
      set( uint32_t ip, uint16_t port ) 
      { 
        ip_   = ip ;
        port_ = port ; 
      }

      //------------------------------------------------------------
      inline void set_ip  ( uint32_t ip   ) { ip_   = ip ; }
      inline void set_port( uint16_t port ) { port_ = port ; }
    } __attribute__(( packed )) ;

  private :
    //-----------------------------------------------------------------------------
    uint64_t encoded_ ;
    Decoded  decoded_ ;

  public :
    //-----------------------------------------------------------------------------
    inline UAddress() { decoded_.clear() ; }

    //-----------------------------------------------------------------------------
    inline void encode( uint32_t ip, uint16_t port ) { decoded_.set( ip, port ) ; }
    inline void decode( uint64_t value )             { encoded_ = value ; }

    //-----------------------------------------------------------------------------
    inline 
    void encode_ip( uint32_t ip ) 
    { 
      decoded_.set_ip( ip ) ; 
      if( decoded_.empty() ) 
        decoded_.set_port( 0 ) ;
    }

    //-----------------------------------------------------------------------------
    inline 
    void  
    encode_port( uint16_t port ) 
    { 
      decoded_.set_port( port ) ; 
    }

    //-----------------------------------------------------------------------------
    void clear() { decoded_.clear() ; }
    
    //-----------------------------------------------------------------------------
    inline uint64_t encoded()      const { return encoded_ ; }
    inline uint32_t decoded_ip()   const { return decoded_.ip() ; }
    inline uint16_t decoded_port() const { return decoded_.port() ; }

    //-----------------------------------------------------------------------------
    inline bool empty()       const { return decoded_.empty() ; }
    inline bool is_wildcard() const { return decoded_.is_wildcard() ; } 

  } __attribute__(( packed )) ;

}}}

//---------------------------------------------------------------------------------
static_assert( sizeof( fps::net::detail::UAddress ) == sizeof( uint64_t )
             , "sizeof( net::detail::UAddress ) != sizeof( uint64_t )"
             ) ;

#endif
