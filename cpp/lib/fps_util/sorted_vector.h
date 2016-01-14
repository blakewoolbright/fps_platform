#ifndef FPS__UTIL__DETAIL__SORTED_VECTOR__H
#define FPS__UTIL__DETAIL__SORTED_VECTOR__H

#include "fps_util/comparators.h"

namespace fps    {
namespace util   {

  //------------------------------------------------------------------------------
  template<typename T, typename T_Compare=util::compare::Ascending<T> >
  struct SortedVector
  {
  } ;


}}

#endif
