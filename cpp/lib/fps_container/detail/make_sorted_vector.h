#ifndef FPS__CONTAINER__DETAIL__MAKE_SORTED_VECTOR__H
#define FPS__CONTAINER__DETAIL__MAKE_SORTED_VECTOR__H

#include "fps_container/detail/sorted_integral_vector.h"
#include "fps_container/detail/sorted_vector.h"
#include <type_traits>

namespace fps {
namespace container {
namespace detail {

  template< typename T, bool T_Multi>
  struct MakeSortedVector
  {
  private :
    static const bool Is_Integral = std::is_integral<T>::value ;
    
  public :
    typedef typename 
    std::conditional<Is_Integral, SortedIntegralVector<T>, SortedVector<T>>::type 
    type_t ;

  } ;

  template< typename T>
  struct MakeSortedVector<T, true>
  {
  private :
    static const bool Is_Integral = std::is_integral<T>::value ;
    
  public :
    /*
    typedef typename 
    std::conditional<Is_Integral, SortedIntegralMultiVector<T>, SortedMultiVector<T>>::type 
    type_t ;
    */
    
  } ;

}}}

#endif
