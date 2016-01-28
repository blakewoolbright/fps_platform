#ifndef FPS__CONTAINER__DETAIL__MAKE_SORTED_VECTOR__H
#define FPS__CONTAINER__DETAIL__MAKE_SORTED_VECTOR__H

#include "fps_container/detail/sorted_integral_vector.h"
#include "fps_container/detail/sorted_object_vector.h"
#include "fps_container/options.h" 
#include "fps_ntp/fps_ntp.h"
#include <type_traits>

namespace fps {
namespace container {
namespace detail {

  template< typename T, typename... T_Args>
  struct MakeSortedVector
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
                               , SortedIntegralVector<T, T_Args...>
                               , SortedIntegralVector<T, T_Args...>
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
                                               , SortedIntegralVector<T, T_Args...>
                                               , SortedIntegralVector<T, T_Args...> 
                                               >::type 
                    , SortedObjectVector<T, T_Args...>
                    >::type 
    type ;

  } ;


}}}

#endif
