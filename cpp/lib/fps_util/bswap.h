#ifndef FPS__UTIL__BSWAP__H
#define FPS__UTIL__BSWAP__H

#include <byteswap.h>
#include <cstdint>

namespace fps {
namespace util {

  //--------------------------------------------------------------------------------------
  template<typename T> 
  inline  
  T bswap( T value ) 
  { 
    return value ; 
  }

  //--------------------------------------------------------------------------------------
  template<> inline uint16_t bswap( uint16_t value ) { return bswap_16( value ) ; }
  template<> inline uint32_t bswap( uint32_t value ) { return bswap_32( value ) ; }
  template<> inline uint64_t bswap( uint64_t value ) { return bswap_64( value ) ; }
  template<> inline int16_t  bswap( int16_t  value ) { return bswap_16( value ) ; }
  template<> inline int32_t  bswap( int32_t  value ) { return bswap_32( value ) ; }
  template<> inline int64_t  bswap( int64_t  value ) { return bswap_64( value ) ; }
}}

#endif
