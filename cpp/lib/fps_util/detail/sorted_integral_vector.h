#ifndef FPS__UTIL__DETAIL__SORTED_INTEGRAL_VECTOR__H
#define FPS__UTIL__DETAIL__SORTED_INTEGRAL_VECTOR__H

#include "fps_util/comparators.h"
#include "fps_util/macros.h"
#include "fps_system/fps_system.h"
#include "fps_util/sorted_vector_common.h"

#include <cstdint>
#include <type_traits>
#include <boost/iterator/iterator_facade.hpp>

namespace fps    {
namespace util   {
namespace detail 
{
  //----------------------------------------------------------------------------------------
  // Resizable sorted array intended for use with primitive types.
  //----------------------------------------------------------------------------------------
  template<typename T, typename T_Compare=util::compare::Ascending<T> >
  struct SortedIntegralVector
  {
  public :
    //--------------------------------------------------------------------------------------
    static_assert( std::is_integral<T>::value 
                 , "SortedIntegralVector<> templated on non-integral type (use SortedVector)"
                 ) ;

    //--------------------------------------------------------------------------------------
    static const uint32_t Default_Capacity = 32 ;

    //--------------------------------------------------------------------------------------
    typedef T         value_t ;
    typedef T_Compare compare_t ;
    typedef T         value_arg_t ;

    //--------------------------------------------------------------------------------------
    struct iterator 
      : public boost::iterator_facade<iterator, T, boost::forward_traversal_tag, T>
    {
    private :
      //-----------------------------------------------------------------
      friend class boost::iterator_core_access ;

      //-----------------------------------------------------------------
      const T * ptr_ ;

      //-----------------------------------------------------------------
      inline 
      void 
      increment() 
      { ++ptr_; 
      } 

      //-----------------------------------------------------------------
      inline 
      bool equal( const iterator & rhs ) const 
      { return (ptr_ == rhs.ptr_) ; 
      }

      //-----------------------------------------------------------------
      inline 
      T 
      dereference() const 
      { return *ptr_ ; 
      }

    public :
      //-----------------------------------------------------------------
      inline 
      iterator() 
        : ptr_( NULL ) 
      {}

      //-----------------------------------------------------------------
      inline 
      explicit 
      iterator( const T * ptr )
        : ptr_( ptr )
      {}

      //-----------------------------------------------------------------
      inline value_arg_t value() const { return *ptr_ ; }
      inline uint32_t    count() const { return 1 ; }

      //-----------------------------------------------------------------
      inline 
      bool 
      bounds_test( const T * begin, const T * end ) const
      { return (ptr_ < end && ptr_ >= begin) ;
      }

      //-----------------------------------------------------------------
      inline 
      int64_t     
      distance_from( const T * other ) const 
      { return static_cast<int64_t>( ptr_ - other ) ; 
      }
    } ;

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
    inline SortedIntegralVector()  ;
    inline SortedIntegralVector( uint32_t capacity )  ;
    inline ~SortedIntegralVector() ;

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
  template<typename T, typename T_Compare>
  SortedIntegralVector<T, T_Compare>::
  SortedIntegralVector() 
    : capacity_( 0 ) 
    , size_    ( 0 )
    , data_    ( NULL ) 
  { 
    reserve( Default_Capacity ) ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  SortedIntegralVector<T, T_Compare>::
  SortedIntegralVector( uint32_t capacity ) 
    : capacity_( 0 ) 
    , size_    ( 0 )
    , data_    ( NULL ) 
  { 
    reserve( capacity ) ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  SortedIntegralVector<T, T_Compare>::
  ~SortedIntegralVector() 
  {
    if( data_ ) 
    { delete [] data_ ; 
      data_ = NULL ;
    }
    capacity_ = 0 ;
    size_     = 0 ;
  }

  //---------------------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  int32_t
  SortedIntegralVector<T, T_Compare>::
  find_member_index( value_arg_t target ) const 
  {
    return algos::BinarySearch<this_t, compare_t>::find_existing( target, *this, size_ ) ;
  }

  //---------------------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  int32_t
  SortedIntegralVector<T, T_Compare>::
  find_insert_index( value_arg_t target ) const 
  {
    return algos::BinarySearch<this_t, compare_t>::find_position( target, *this, size_ ) ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  void 
  SortedIntegralVector<T, T_Compare>::
  clear() 
  { 
    size_ = 0 ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  typename SortedIntegralVector<T, T_Compare>::iterator 
  SortedIntegralVector<T, T_Compare>::
  find( value_arg_t target ) const 
  {
    int32_t mbr_idx = find_member_index( target ) ;
    return ( mbr_idx < 0 ) 
           ? end() 
           : iterator( &(data_[ mbr_idx ]) ) 
           ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  typename SortedIntegralVector<T, T_Compare>::iterator 
  SortedIntegralVector<T, T_Compare>::
  insert( value_arg_t target ) 
  {
    return end() ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  typename SortedIntegralVector<T, T_Compare>::iterator 
  SortedIntegralVector<T, T_Compare>::
  erase( value_arg_t target ) 
  {
    return end() ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  typename SortedIntegralVector<T, T_Compare>::iterator 
  SortedIntegralVector<T, T_Compare>::
  erase( iterator itr ) 
  {
    return end() ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  bool
  SortedIntegralVector<T, T_Compare>::
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
    
    if( size_ > 0 ) 
      std::memcpy( data_, old_data, sizeof( T ) * size_ ) ;

    capacity_ = new_cap ;
    delete [] old_data ;
    return true ;
  }

}}}

#endif
