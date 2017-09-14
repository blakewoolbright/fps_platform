#ifndef FPS__CONTAINER__DETAIL__FLAT_OBJECT_SET__H
#define FPS__CONTAINER__DETAIL__FLAT_OBJECT_SET__H

#include "fps_container/comparators.h"
#include "fps_util/macros.h"
#include "fps_system/fps_system.h"
#include "fps_ntp/fps_ntp.h"
#include "fps_container/algorithms.h"
#include "fps_container/detail/flat_set_common.h"
#include "fps_container/options.h"

#include <cstdint>
#include <type_traits>
#include <boost/iterator/iterator_facade.hpp>

namespace fps    {
namespace container {
namespace detail {

  //----------------------------------------------------------------------------------------
  // Resizable sorted array intended for use with small structs or primitive types.
  //----------------------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  struct FlatObjectSet
  {
  public :

    //--------------------------------------------------------------------------------------
    static const bool Is_Integral = std::is_integral<T>::value ;
    static const bool Is_Trivial  = std::is_trivial<T>::value ;

    // static const bool Is_Trivial_To_Construct = std::is_trivially_constructable<T>::value ;

    //--------------------------------------------------------------------------------------
    static_assert( !Is_Integral
                 , "FlatObjectSet<> templated on integral type (use FlatIntegralSet)"
                 ) ;

    //--------------------------------------------------------------------------------------
    // Import compile time configuration via fps::ntp (named template parameter) library.
    //--------------------------------------------------------------------------------------

    //------------------------------------------------------------------------------------
    // Max_Capacity
    // Limit capacity to this number of elements.  The reserve() function will 
    // return false if it detects an attempt to reserve more than 'Max_Capacity' elements.
    //------------------------------------------------------------------------------------
    static 
    const 
    uint32_t 
    Max_Capacity = ntp::get_value< opt::Max_Capacity<0>, T_Args...>::value ;

    //------------------------------------------------------------------------------------
    // Default_Capacity
    // This is the default capacity of members that are default constructed.
    //------------------------------------------------------------------------------------
    static 
    const 
    uint32_t 
    Default_Capacity = ntp::get_value< opt::Default_Capacity<64>, T_Args...>::value ;

    //------------------------------------------------------------------------------------
    // Reverse
    // Sort in descending order ( default is ascending ).
    //------------------------------------------------------------------------------------
    static 
    const
    bool 
    Reverse = ntp::get_value< opt::Reverse<false>, T_Args...>::value ;

    //--------------------------------------------------------------------------------------
    static const bool Distinct = true ;

    //--------------------------------------------------------------------------------------
    // compare_t 
    // Memberwise comparison operations/
    //--------------------------------------------------------------------------------------
    typedef 
    typename 
    std::conditional< Reverse, compare::Descending<T>, compare::Ascending<T> >::type 
    compare_t ;

    //--------------------------------------------------------------------------------------
    typedef T         value_t ;
    typedef const T & value_arg_t ;

    //--------------------------------------------------------------------------------------
    typedef flat_set_iterator<value_t> iterator ;

    //--------------------------------------------------------------------------------------
    typedef fps::container::detail::Construct<T> construct_impl_t ;
    typedef fps::container::detail::Assign<T>    assign_impl_t ;
    typedef fps::container::detail::Destruct <T> destruct_impl_t ;

  private :
    //------------------------------------------------------------------------
    uint32_t  capacity_ alignas( system::cpu::Cache_Line_Size ) ; 
    uint32_t  size_     alignas( system::cpu::Cache_Line_Size ) ;
    value_t * data_ ;
  
    //------------------------------------------------------------------------
    inline int32_t find_member_index( value_arg_t target ) const ;
    inline int32_t find_insert_index( value_arg_t target ) const ;

    //------------------------------------------------------------------------
    inline 
    void 
    copy_range( T       * dest
              , const T * src
              , uint32_t  src_idx
              , uint32_t  dest_idx
              , uint32_t  count 
              ) 
    {
      const uint32_t stop_idx = src_idx + count ; 
      while( src_idx < stop_idx ) 
      { assign_impl_t()( &(dest[ dest_idx++ ]), &(src[ src_idx++ ]) ) ;
      }
    } 

  public :
    //------------------------------------------------------------------------
    inline FlatObjectSet()  ;
    inline FlatObjectSet( uint32_t capacity )  ;
    inline ~FlatObjectSet() ;

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
  } ;

  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  FlatObjectSet<T, T_Args...>::
  FlatObjectSet() 
    : capacity_( 0 ) 
    , size_    ( 0 )
    , data_    ( NULL ) 
  { 
    reserve( Default_Capacity ) ;
  }
  
  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  FlatObjectSet<T, T_Args...>::
  FlatObjectSet( uint32_t capacity ) 
    : capacity_( 0 ) 
    , size_    ( 0 )
    , data_    ( NULL ) 
  { 
    reserve( capacity ) ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  FlatObjectSet<T, T_Args...>::
  ~FlatObjectSet() 
  {
    if( data_ ) 
    { delete [] data_ ; 
      data_ = NULL ;
    }
    capacity_ = 0 ;
    size_     = 0 ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  int32_t
  FlatObjectSet<T, T_Args...>::
  find_member_index( value_arg_t target ) const 
  {
    typedef typename std::remove_pointer< decltype( this ) >::type this_t ;
    typedef algos::BinarySearch<this_t, compare_t>                 search_t ;
    return search_t::find_existing( target, *this, size_ ) ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  int32_t
  FlatObjectSet<T, T_Args...>::
  find_insert_index( value_arg_t target ) const 
  {
    typedef typename std::remove_pointer< decltype( this ) >::type this_t ;
    typedef algos::BinarySearch<this_t, compare_t>                 search_t ;
    return search_t::find_position( target, *this, size_ ) ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  void 
  FlatObjectSet<T, T_Args...>::
  clear() 
  { 
    size_ = 0 ;
    
    // TODO: Handle reset/destruction of non-trivial types
  }

  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  typename FlatObjectSet<T, T_Args...>::iterator 
  FlatObjectSet<T, T_Args...>::
  find( value_arg_t target ) const 
  {
    int32_t mbr_idx = find_member_index( target ) ;
    return ( mbr_idx < 0 ) 
           ? end() 
           : iterator( &(data_[ mbr_idx ]) ) 
           ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  typename FlatObjectSet<T, T_Args...>::iterator 
  FlatObjectSet<T, T_Args...>::
  insert( value_arg_t target ) 
  {
    return end() ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  typename FlatObjectSet<T, T_Args...>::iterator 
  FlatObjectSet<T, T_Args...>::
  erase( value_arg_t target ) 
  {
    return end() ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  typename FlatObjectSet<T, T_Args...>::iterator 
  FlatObjectSet<T, T_Args...>::
  erase( iterator itr ) 
  {
    return end() ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  bool
  FlatObjectSet<T, T_Args...>::
  reserve( uint32_t min_free_slots ) 
  {
    if( free_slots() >= min_free_slots ) 
      return true ;

    //
    // If the 'min_free_slots' is less than then current capacity, 
    // allocate a new array with double the current capacity.  Otherwise,
    // use 'min_free_slots' as expected.
    // 
    uint32_t new_cap = ( min_free_slots < capacity_ ) 
                     ? capacity_ * 2 
                     : capacity_ + min_free_slots 
                     ;

    // Save current array & capacity 
    // uint32_t   old_cap  = capacity_ ;
    T * old_data = data_ ;

    // Allocate new array with increased capacity, and include an extra 
    // slot to make the end() iterator's implementation less complex.
    data_ = new T [ new_cap + 1 ] ;
    if( fps_unlikely( data_ == NULL ) )
    { 
      data_ = old_data ;
      return false ;
    }

    copy_range( data_, old_data, 0, 0, size_ ) ;
    capacity_ = new_cap ;
    delete [] old_data ;

    return true ;
  }

}}}

#endif
