#ifndef FPS__UTIL__DETAIL__SORTED_VECTOR__H
#define FPS__UTIL__DETAIL__SORTED_VECTOR__H

#include "fps_util/comparators.h"

namespace fps    {
namespace util   {

  //------------------------------------------------------------------------------
  template<typename T, typename T_Compare=util::compare::Ascending<T> >
  struct SortedVector
  {
  private :
    uint32_t capacity_ ;
    uint32_t size_ ;
  
  public :
    SortedVector()  ;
    ~SortedVector() ;

  
  } ;


}}

#endif
