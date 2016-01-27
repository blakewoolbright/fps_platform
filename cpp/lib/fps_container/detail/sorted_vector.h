#ifndef FPS__CONTAINER__DETAIL__SORTED_VECTOR__H
#define FPS__CONTAINER__DETAIL__SORTED_VECTOR__H

#include "fps_container/comparators.h"
#include "fps_util/macros.h"
#include "fps_system/fps_system.h"
#include "fps_ntp/fps_ntp.h"
#include "fps_container/algorithms.h"
#include "fps_container/detail/sorted_vector_common.h"
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
  struct SortedVector
  {
  public :

    //--------------------------------------------------------------------------------------
    static_assert( !std::is_integral<T>::value 
                 , "SortedVector<> templated on integral type (use SortedIntegralVector)"
                 ) ;

    //--------------------------------------------------------------------------------------
    static const bool     Is_Integral      = std::is_integral<T>::value ;
    static const bool     Is_Trivial       = std::is_trivial<T>::value ;
    // static const bool     Is_Trivial_To_Copy = std::is_trivially_copyable<T>::value ;

    //--------------------------------------------------------------------------------------
    static 
    const 
    uint32_t 
    Max_Capacity = ntp::get_value< opt::Max_Capacity<0>, T_Args...>::value ;

    //--------------------------------------------------------------------------------------
    static 
    const 
    uint32_t 
    Default_Capacity = ntp::get_value< opt::Default_Capacity<64>, T_Args...>::value ;

    //--------------------------------------------------------------------------------------
    static 
    const
    bool 
    Reverse = ntp::get_value< opt::Reverse<false>, T_Args...>::value ;

    //--------------------------------------------------------------------------------------
    typedef 
    typename 
    std::conditional< Reverse, compare::Descending<T>, compare::Ascending<T> >::type 
    compare_t ;

    //--------------------------------------------------------------------------------------
    typedef T         value_t ;
    typedef const T & value_arg_t ;

    //--------------------------------------------------------------------------------------
    typedef distinct_sorted_vector_iterator<value_t> iterator ;

    //--------------------------------------------------------------------------------------
    typedef Construct<T> default_construct ;
    typedef Copy<T>      copy_construct    ;
    typedef Destruct <T> default_destruct  ;

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
    inline SortedVector()  ;
    inline SortedVector( uint32_t capacity )  ;
    inline ~SortedVector() ;

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
  SortedVector<T, T_Args...>::
  SortedVector() 
    : capacity_( 0 ) 
    , size_    ( 0 )
    , data_    ( NULL ) 
  { 
    reserve( Default_Capacity ) ;
  }
  
  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  SortedVector<T, T_Args...>::
  SortedVector( uint32_t capacity ) 
    : capacity_( 0 ) 
    , size_    ( 0 )
    , data_    ( NULL ) 
  { 
    reserve( capacity ) ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  SortedVector<T, T_Args...>::
  ~SortedVector() 
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
  SortedVector<T, T_Args...>::
  find_member_index( value_arg_t target ) const 
  {
    typedef typename std::remove_pointer< decltype( this ) >::type this_t ;
    typedef algos::BinarySearch<this_t, compare_t>                 search_t ;
    return search_t::find_existing( target, *this, size_ ) ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  int32_t
  SortedVector<T, T_Args...>::
  find_insert_index( value_arg_t target ) const 
  {
    typedef typename std::remove_pointer< decltype( this ) >::type this_t ;
    typedef algos::BinarySearch<this_t, compare_t>                 search_t ;
    return search_t::find_position( target, *this, size_ ) ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  void 
  SortedVector<T, T_Args...>::
  clear() 
  { 
    size_ = 0 ;
    
    // TODO: Handle reset/destruction of non-trivial types
  }

  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  typename SortedVector<T, T_Args...>::iterator 
  SortedVector<T, T_Args...>::
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
  typename SortedVector<T, T_Args...>::iterator 
  SortedVector<T, T_Args...>::
  insert( value_arg_t target ) 
  {
    return end() ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  typename SortedVector<T, T_Args...>::iterator 
  SortedVector<T, T_Args...>::
  erase( value_arg_t target ) 
  {
    return end() ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  typename SortedVector<T, T_Args...>::iterator 
  SortedVector<T, T_Args...>::
  erase( iterator itr ) 
  {
    return end() ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  bool
  SortedVector<T, T_Args...>::
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
    uint32_t   old_cap  = capacity_ ;
    T        * old_data = data_ ;

    // Allocate new array with increased capacity, and include an extra 
    // slot to make the end() iterator's implementation less complex.
    data_ = new T [ new_cap + 1 ] ;
    if( fps_unlikely( data_ == NULL ) )
    { data_ = old_data ;
      return false ;
    }
    
    if( Is_Integral ) 
    {  
      // TODO : In addition to copying the old data into the new buffer,
      //        consider default-initializing any remaining 
      //        uninitialized slots.
      if( size_ > 0 ) 
        std::memcpy( data_, old_data, sizeof( T ) * size_ ) ;
    }
    else
    {
      // For each member in the new array, initialize it by either 
      // copy constructing it from the corresponding slot in the old 
      // array, or default constructing it if no such slot exists.
      for( uint32_t idx = 0 ; idx < new_cap ; ++idx ) 
      {
        T * cur_member = &( data_[ idx ] ) ;
        if( idx < size_ )   
        { 
          copy_construct  ()( cur_member, &(old_data[ idx ]) ) ;
          default_destruct()( &(old_data[ idx ] ) ) ;
        }
        else 
          default_construct()( cur_member ) ;
      }
    }

    capacity_ = new_cap ;
    delete [] old_data ;
    return true ;
  }

}}}

#endif
