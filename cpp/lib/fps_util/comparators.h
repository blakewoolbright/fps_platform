#ifndef FPS__UTIL__COMPARATORS__H
#define FPS__UTIL__COMPARATORS__H

#include <type_traits>

// 
// This header defines generalized comparison operations for use w/ various 
// internal containers/utilities.
//

namespace fps     {
namespace util    {
namespace compare {

  //-----------------------------------------------------------------------------------
  template<typename T, typename T_Enable = void>
  struct Ascending
  {
    static inline bool lt( const T & v1, const T & v2 ) { return v1 < v2 ; }
    static inline bool gt( const T & v1, const T & v2 ) { return v1 > v2 ; }
  } ;

  //-----------------------------------------------------------------------------------
  template<typename T>
  struct Ascending<T, typename std::enable_if< std::is_integral<T>::value >::type>
  {
    static inline bool lt( T v1, T v2 ) { return v1 < v2 ; }
    static inline bool gt( T v1, T v2 ) { return v1 > v2 ; }
  } ;

  //-----------------------------------------------------------------------------------
  template<typename T, typename T_Enable = void>
  struct Descending
  {
    static inline bool lt( const T & v1, const T & v2 ) { return v1 > v2 ; }
    static inline bool gt( const T & v1, const T & v2 ) { return v1 < v2 ; }
  } ;

  //-----------------------------------------------------------------------------------
  template<typename T>
  struct Descending<T, typename std::enable_if< std::is_integral<T>::value >::type>
  {
    static inline bool lt( T v1, T v2 ) { return v1 > v2 ; }
    static inline bool gt( T v1, T v2 ) { return v1 < v2 ; }
  } ;
}}}

#endif
