#ifndef FPS__CONTAINER__FLAT_SET__H
#define FPS__CONTAINER__FLAT_SET__H

#include "fps_util/macros.h"
#include "fps_container/detail/make_flat_set.h"
#include "fps_system/fps_system.h"

#include <cstdint>
#include <type_traits>
#include <boost/iterator/iterator_facade.hpp>

namespace fps       {
namespace container {

  //----------------------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  struct FlatSet 
    : public detail::make_flat_set<T, T_Args...>::type 
  {
  } ;

  //----------------------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  struct FlatMultiSet 
    : public detail::make_flat_multiset<T, T_Args...>::type 
  {
  } ;
}}

#endif
