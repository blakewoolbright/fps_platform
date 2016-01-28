#ifndef FPS__CONTAINER__DETAIL__MAKE_FLAT_SET__H
#define FPS__CONTAINER__DETAIL__MAKE_FLAT_SET__H

#include "fps_container/detail/flat_integral_set.h"
#include "fps_container/detail/flat_object_set.h"
#include "fps_container/options.h" 
#include "fps_ntp/fps_ntp.h"
#include <type_traits>

namespace fps {
namespace container {
namespace detail {

  template< typename T, typename... T_Args>
  struct make_flat_set
  {
  private :
    static 
    const bool 
    Is_Integral = std::is_integral<T>::value ;

    static 
    const bool 
    Is_Distinct = ntp::get_value< container::opt::Distinct<false>, T_Args...>::value ;
    
  public :
    /*
    typedef 
    typename 
    std::conditional< Is_Integral
                    , typename std::conditional
                               < Is_Distinct
                               , FlatIntegralSet<T, T_Args...>
                               , FlatIntegralSet<T, T_Args...>
                               >::type
                    , typename std::conditional
                               < Is_Distinct
                               , SortedVector<T>
                               , SortedVector<T>
                               >::type 
                    > type ;
    */
    typedef 
    typename 
    std::conditional< Is_Integral 
                    , typename std::conditional< Is_Distinct
                                               , FlatIntegralSet<T, T_Args...>
                                               , FlatIntegralSet<T, T_Args...> 
                                               >::type 
                    , FlatObjectSet<T, T_Args...>
                    >::type 
    type ;

  } ;


}}}

#endif
