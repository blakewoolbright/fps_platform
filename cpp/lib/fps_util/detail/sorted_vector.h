#ifndef FPS__UTIL__DETAIL__SORTED_VECTOR__H
#define FPS__UTIL__DETAIL__SORTED_VECTOR__H

#include "fps_util/comparators.h"
#include "fps_util/macros.h"
#include "fps_util/detail/sorted_vector_common.h"
#include "fps_system/fps_system.h"

#include <cstdint>
#include <type_traits>
#include <boost/iterator/iterator_facade.hpp>

namespace fps    {
namespace util   {
namespace detail {

  //----------------------------------------------------------------------------------------
  // Resizable sorted array intended for use with small structs or primitive types.
  //----------------------------------------------------------------------------------------
  template<typename T, typename T_Compare=util::compare::Ascending<T> >
  struct SortedVector
  {
  public :

    //--------------------------------------------------------------------------------------
    static_assert( !std::is_integral<T>::value 
                 , "SortedVector<> templated on integral type (use SortedIntegralVector)"
                 ) ;

    //--------------------------------------------------------------------------------------
    static const uint32_t Default_Capacity = 32 ;
    static const bool     Is_Integral      = std::is_integral<T>::value ;
    static const bool     Is_Trivial       = std::is_trivial<T>::value ;
    // static const bool     Is_Trivial_To_Copy = std::is_trivially_copyable<T>::value ;

    //--------------------------------------------------------------------------------------
    typedef T         value_t ;
    typedef T_Compare compare_t ;
    typedef const T & value_arg_t ;

    //--------------------------------------------------------------------------------------
    typedef fps::util::detail::Construct<T> default_construct ;
    typedef fps::util::detail::Copy<T>      copy_construct    ;
    typedef fps::util::detail::Destruct <T> default_destruct  ;

    //--------------------------------------------------------------------------------------
    struct iterator 
      : public boost::iterator_facade<iterator, T, boost::forward_traversal_tag, T>
    {
    private :
      //-----------------------------------------------------------------
      friend class boost::iterator_core_access ;

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
  template<typename T, typename T_Compare>
  SortedVector<T, T_Compare>::
  SortedVector() 
    : capacity_( 0 ) 
    , size_    ( 0 )
    , data_    ( NULL ) 
  { 
    reserve( Default_Capacity ) ;
  }
  
  //--------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  SortedVector<T, T_Compare>::
  SortedVector( uint32_t capacity ) 
    : capacity_( 0 ) 
    , size_    ( 0 )
    , data_    ( NULL ) 
  { 
    reserve( capacity ) ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  SortedVector<T, T_Compare>::
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
  template<typename T, typename T_Compare>
  int32_t
  SortedVector<T, T_Compare>::
  find_member_index( value_arg_t target ) const 
  {
    typedef typename std::remove_pointer< decltype( this ) >::type this_t ;
    typedef algos::BinarySearch<this_t, compare_t>                 search_t ;
    return search_t::find_existing( target, *this, size_ ) ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  int32_t
  SortedVector<T, T_Compare>::
  find_insert_index( value_arg_t target ) const 
  {
    typedef typename std::remove_pointer< decltype( this ) >::type this_t ;
    typedef algos::BinarySearch<this_t, compare_t>                 search_t ;
    return search_t::find_position( target, *this, size_ ) ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  void 
  SortedVector<T, T_Compare>::
  clear() 
  { 
    size_ = 0 ;
    
    // TODO: Handle reset/destruction of non-trivial types
  }

  //--------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  typename SortedVector<T, T_Compare>::iterator 
  SortedVector<T, T_Compare>::
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
  typename SortedVector<T, T_Compare>::iterator 
  SortedVector<T, T_Compare>::
  insert( value_arg_t target ) 
  {
    return end() ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  typename SortedVector<T, T_Compare>::iterator 
  SortedVector<T, T_Compare>::
  erase( value_arg_t target ) 
  {
    return end() ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  typename SortedVector<T, T_Compare>::iterator 
  SortedVector<T, T_Compare>::
  erase( iterator itr ) 
  {
    return end() ;
  }

  //--------------------------------------------------------------------------
  template<typename T, typename T_Compare>
  bool
  SortedVector<T, T_Compare>::
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
