#ifndef FPS__UTIL__DETAIL__SORTED_VECTOR_COMMON__H
#define FPS__UTIL__DETAIL__SORTED_VECTOR_COMMON__H

#include "fps_util/comparators.h"
#include "fps_util/macros.h"
#include "fps_system/fps_system.h"

#include <cstdint>
#include <type_traits>
#include <boost/iterator/iterator_facade.hpp>

namespace fps    {
namespace util   {
namespace detail 
{
  //--------------------------------------------------------------------------------------
  // The following "Construct" class is used to default-construct container members.  
  // User defined types are default constructed w/ placement new.  
  // Integral/primitive types are unaffected.
  //--------------------------------------------------------------------------------------
  template<typename T, typename T_Enable=void>
  struct Construct 
  {  
    inline void operator()( T * arg ) { new ( arg ) T() ; }
  } ;

  //--------------------------------------------------------------------------------------
  template<typename T>
  struct Construct<T, typename std::enable_if< std::is_integral<T>::value >::type>
  {  
    // inline void operator()( T * arg ) { *arg = 0 ; }
    inline void operator()( T * arg ) {}
  } ;

  //--------------------------------------------------------------------------------------
  // The following "Copy" class is used to copy container members.  
  // User defined types are copy constructed via placement new. 
  // Primitive types are directly assigned via the "=" operator.
  //--------------------------------------------------------------------------------------
  template<typename T, typename T_Enable=void>
  struct Copy 
  {  
    inline void operator()( T * arg, const T & src ) { new ( arg ) T( src ) ; }
  } ;

  //--------------------------------------------------------------------------------------
  template<typename T>
  struct Copy<T, typename std::enable_if< std::is_integral<T>::value >::type>
  {  
    inline void operator()( T * arg, T src ) { *arg = src ; }
  } ;

  //--------------------------------------------------------------------------------------
  // The following "Destruct" class is used to destroy container members.
  // User defined types have their destructor invoked, and primitive 
  // types are ignored/passed through.
  //--------------------------------------------------------------------------------------
  template<typename T, typename T_Enable=void>
  struct Destruct 
  { 
    inline void operator()( T * arg ) { arg->~T() ; }
  } ;

  //--------------------------------------------------------------------------------------
  template<typename T>
  struct Destruct<T, typename std::enable_if< std::is_integral<T>::value >::type>
  {  
    inline void operator()( T * arg ) {} 
  } ;

}}}
#endif
