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
    /*
    //------------------------------------------------------------------------
    inline iterator begin()      const { return iterator( &data_[ 0 ] ) ; }
    inline iterator end  ()      const { return iterator( &data_[ size_ ] ) ; }
    inline uint32_t capacity()   const { return capacity_ ; }
    inline uint32_t size()       const { return size_ ; }
    inline uint32_t free_slots() const { return ( capacity_ - size_ ) ; }
    inline bool     empty()      const { return size_ == 0 ; }

    //------------------------------------------------------------------------
    inline bool reserve( uint32_t min_free_slots ) ;

    //------------------------------------------------------------------------
    inline void clear() ;

    //------------------------------------------------------------------------
    inline iterator find  ( value_arg_t target ) const ;
    inline iterator insert( value_arg_t target ) ;
    inline iterator erase ( value_arg_t target ) ;
    inline iterator erase ( iterator itr ) ;
    */
  } ;

}}

#endif
