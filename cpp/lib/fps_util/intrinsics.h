#ifndef FPS__UTIL__INTRINSICS__H
#define FPS__UTIL__INTRINSICS__H

#include <cstdint>

namespace fps {
namespace util {
namespace intrinsic {

  #define FPS_FORCE_INLINE __attribute__(( always_inline )) 

  
  //--------------------------------------------------------------------------------------
  inline void * memmove( void * dest, const void * src, std::size_t bytes ) FPS_FORCE_INLINE ;
  inline void * memset( void * dest, char value, std::size_t count )        FPS_FORCE_INLINE ;
  inline void * memcpy( void * dest, const void * src, std::size_t bytes )  FPS_FORCE_INLINE ;

  //--------------------------------------------------------------------------------------
  void * 
  memmove( void * dest, const void * src, std::size_t bytes ) 
  {
    return ::__builtin_memmove( dest, src, bytes ) ; 
  }
  
  //--------------------------------------------------------------------------------------
  void * 
  memset( void * dest, char value, std::size_t count )
  {
    return ::__builtin_memset( dest, value, count ) ; 
  }

  //--------------------------------------------------------------------------------------
  void * 
  memcpy( void * dest, const void * src, std::size_t bytes )
  {
    return ::__builtin_memcpy( dest, src, bytes ) ; 
  }

}}}

#endif
