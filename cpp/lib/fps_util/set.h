#ifndef FPS__UTIL__SET__H
#define FPS__UTIL__SET__H

#include "fps_util/macros.h"
#include "fps_util/comparators.h"
#include "fps_util/detail/sorted_vector.h"
#include "fps_util/detail/sorted_integral_vector.h"
#include "fps_system/fps_system.h"

#include <cstdint>
#include <type_traits>
#include <boost/iterator/iterator_facade.hpp>

namespace fps    {
namespace util   {

  //----------------------------------------------------------------------------------------
  // Resizable sorted array intended for use with small structs or primitive types.
  //----------------------------------------------------------------------------------------
  template<typename T, typename T_Compare=util::compare::Ascending<T> >
  struct Set 
  {
  public :
    //--------------------------------------------------------------------------------------
    static const uint32_t Default_Capacity   = 32 ;
    static const bool     Is_Integral        = std::is_integral<T>::value ;
    static const bool     Is_Trivial         = std::is_trivial<T>::value ;
    // static const bool     Is_Trivial_To_Copy = std::is_trivially_copyable<T>::value ;

    //--------------------------------------------------------------------------------------
    typedef T         value_t ;
    typedef T_Compare compare_t ;
    typedef typename std::conditional<Is_Integral, T, const T &>::type value_arg_t ;

    //--------------------------------------------------------------------------------------
    typedef detail::Construct<T> default_construct ;
    typedef detail::Copy<T>      copy_construct    ;
    typedef detail::Destruct <T> default_destruct  ;

  private :
    //------------------------------------------------------------------------
    uint32_t  capacity_ alignas( system::cpu::Cache_Line_Size ) ; 
    uint32_t  size_     alignas( system::cpu::Cache_Line_Size ) ;
    value_t * data_ ;
  
    //------------------------------------------------------------------------
    inline int32_t find_member_index( value_arg_t target ) const ;
    inline int32_t find_insert_index( value_arg_t target ) const ;

  public :
    //------------------------------------------------------------------------
    inline Set()  ;
    inline Set( uint32_t capacity )  ;
    inline ~Set() ;

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