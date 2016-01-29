#ifndef FPS__CONTAINER__DETAIL__MAKE_FLAT_SET__H
#define FPS__CONTAINER__DETAIL__MAKE_FLAT_SET__H

#include "fps_container/detail/flat_integral_set.h"
#include "fps_container/detail/flat_integral_multiset.h"
#include "fps_container/detail/flat_object_set.h"
#include "fps_container/options.h" 
#include "fps_ntp/fps_ntp.h"
#include <type_traits>

namespace fps {
namespace container {
namespace detail {

  //----------------------------------------------------------------------------------------
  template< typename T, typename... T_Args>
  struct make_flat_set
  {
  public :
    //----------------------------------------------------------------------------------------
    /*
    typedef 
    typename 
    std::conditional< std::is_integral<T>::value || std::is_trivial<T>::value 
                    , FlatIntegralSet<T, T_Args...>
                    , FlatObjectSet  <T, T_Args...>
                    >::type 
    type ;
    */
    typedef FlatIntegralSet<T, T_Args...> type ;
    typedef type type_t ;
  } ;

  //----------------------------------------------------------------------------------------
  template< typename T, typename... T_Args>
  struct make_flat_multiset
  {
    //----------------------------------------------------------------------------------------
    /*
    typedef 
    typename 
    std::conditional< std::is_integral<T>::value || std::is_trivial<T>::value 
                    , FlatIntegralMultiSet<T, T_Args...>
                    , FlatObjectSet      <T, T_Args...>
                    >::type 
    type ;
    */
    typedef FlatIntegralMultiSet<T, T_Args...> type ;
    typedef type type_t ;
  } ;
}}}

#endif
