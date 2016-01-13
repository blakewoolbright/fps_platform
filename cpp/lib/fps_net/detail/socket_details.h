#ifndef FPS__NET__DETAIL__SOCKET_DETAILS__H
#define FPS__NET__DETAIL__SOCKET_DETAILS__H

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

namespace fps {
namespace net {

  //----------------------------------------------------------------------------------------
  // Option containers for use w/ set_option & get_option.
  //----------------------------------------------------------------------------------------
  struct Non_Block
  {
    typedef bool value_t ;
    static const uint32_t Flag  = NONBLOCK ;
  } ;

  //----------------------------------------------------------------------------------------
  struct Reuse_Addr
  { 
    typedef bool value_t ;
    static const uint32_t Flag  = SO_REUSEADDR ;
    static const uint32_t Level = SOL_SOCKET ;
  } ;

  //----------------------------------------------------------------------------------------
  struct MC_Add
  { 
    static const uint32_t Flag  = IP_ADD_MEMBERSHIP ;
    static const uint32_t Level = IP_PROTO_IP ;
  } ;

  //----------------------------------------------------------------------------------------
  struct MC_Drop
  { 
    static const uint32_t Flag  = IP_DROP_MEMBERSHIP ;
    static const uint32_t Level = IP_PROTO_IP ;
  } ;

  //----------------------------------------------------------------------------------------
  struct MC_Interface
  { 
    static const uint32_t Flag  = IP_MULTICAST_IF ;
    static const uint32_t Level = IP_PROTO_IP ;
  } ;

  //----------------------------------------------------------------------------------------
  struct MC_TTL
  { 
    static const uint32_t Flag  = IP_MULTICAST_TTL ;
    static const uint32_t Level = IP_PROTO_IP ;
  } ;

  //----------------------------------------------------------------------------------------
  struct MC_Loop
  { 
    static const uint32_t Flag  = IP_MULTICAST_LOOP ;
    static const uint32_t Level = IP_PROTO_IP ;
  } ;

namespace detail {

  //----------------------------------------------------------------------------------------
  inline int32_t udp_open() { return ::socket( AF_INET, SOCK_DGRAM, 0 ) ; }

  //----------------------------------------------------------------------------------------
  inline int32_t tcp_open() { return ::socket( AF_INET, SOCK_STREAM, 0 ) ; }

  //----------------------------------------------------------------------------------------
  inline int32_t close( int32_t fd ) { return ::close( fd ) ; }

  //----------------------------------------------------------------------------------------
  inline 
  int32_t 
  bind( int32_t fd, const ::sockaddr_in & addr ) 
  {
    return ( !::bind( fd, reinterpret_cast<const ::sockaddr *>( &addr ), sizeof( addr ) ) )
           ? 0 
           : errno 
           ;
  }

  //----------------------------------------------------------------------------------------
  inline
  int32_t 
  listen( int32_t fd, uint32_t backlog = 128 ) 
  {
    return !::listen( fd, backlog ) 
           ? 0 
           : errno 
           ;
  }

  //----------------------------------------------------------------------------------------
  template<typename T_Opt>
  bool 
  set_option( int32_t fd, bool value ) 
  {
    return true ;
  }

  //----------------------------------------------------------------------------------------
  template<typename T_Opt>
  int32_t  
  get_option( int32_t fd ) 
  {
    return 0 ;
  }
   
  //----------------------------------------------------------------------------------------
  template<>
  bool 
  set_option<fps::net::Non_Block>( int32_t fd, bool value ) 
  {
    return true ;
  }
  
  //----------------------------------------------------------------------------------------
  template<>
  bool
  get_option<fps::net::Non_Block>( int32_t fd ) 
  {
    int32_t flagset = ::fcntl( fd, F_GETFL, 0 ) ;
    if( flagset > 0 )
      return flagset & fps::net::Non_Block::Flag ;
    else 
      return false ;
  }
}

}}

#endif
