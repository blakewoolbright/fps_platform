#ifndef FPS__CONTAINER__DETAIL__FLAT_INTEGRAL_MULTI_SET__H
#define FPS__CONTAINER__DETAIL__FLAT_INTEGRAL_MULTI_SET__H

#include "fps_container/comparators.h"
#include "fps_util/macros.h"
#include "fps_container/algorithms.h"
#include "fps_util/intrinsics.h"
#include "fps_ntp/fps_ntp.h"
#include "fps_container/detail/flat_set_common.h"
#include "fps_system/fps_system.h"
#include "fps_container/options.h"

#include <cstdint>
#include <type_traits>
#include <boost/iterator/iterator_facade.hpp>

namespace fps    {
namespace container {
namespace detail 
{

  //----------------------------------------------------------------------------------------
  // FlatIntegralMultiSet
  //
  // Resizable sorted array of integral values that may be inserted multiple times.  
  //
  // T :
  //   This template argument should be a primitive type (char, int16_t, uint32_t, etc.).
  //   If the type of "T" is anything else, a static assertion will fail. 
  //
  // T_Args : 
  //   This template parameter pack should be composed of one or more named 
  //   option structures. See: fps_container/options.h for a listing of valid options.
  //
  // Note: This implementation does ALLOWS duplicate values to be inserted.
  //
  //----------------------------------------------------------------------------------------
  template<typename T, typename... T_Args> 
  struct FlatIntegralMultiSet
  {
  public :
    //--------------------------------------------------------------------------------------
    // Expose type information for member counter and member value
    //--------------------------------------------------------------------------------------
    typedef T value_t ;

    typedef 
    typename 
    ntp::get_type<opt::Counter_Type<uint32_t>, T_Args...>::value 
    counter_t ;

    //--------------------------------------------------------------------------------------
    static_assert( std::is_integral<T>::value 
                 , "FlatIntegralMultiSet<> templated on non-integral type (use FlatObjectMultiSet)"
                 ) ;

    //--------------------------------------------------------------------------------------
    static_assert( std::is_integral<counter_t>::value 
                 , "FlatIntegralMultiSet<> configured w/ non-integral counter type"
                 ) ;

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
    static const bool Distinct = false ;

    //--------------------------------------------------------------------------------------
    typedef 
    typename 
    std::conditional< Reverse, compare::Descending<T>, compare::Ascending<T> >::type 
    compare_t ;

    //--------------------------------------------------------------------------------------
    template<typename U_Value, typename U_Counter>
    struct Member 
    {
    public :
      //------------------------------------------------------------------------------------
      typedef U_Value   value_t ;
      typedef U_Counter counter_t ;

    private :
      value_t   value_ ;
      counter_t count_ ;

    public :
      //------------------------------------------------------------------------------------
      inline value_t   value() const { return value_ ; }
      inline counter_t count() const { return count_ ; }

      //------------------------------------------------------------------------------------
      inline counter_t operator++( int ) { return ++count_ ; }
      inline counter_t operator++()      { return count_++ ; }
      inline counter_t operator--(int )  { return --count_ ; }
      inline counter_t operator--()      { return count_-- ; }

      //------------------------------------------------------------------------------------
      inline 
      counter_t 
      operator-=( counter_t rhs ) 
      { 
        return ( count_ > rhs ) 
               ? (count_ -= rhs) 
               : (count_ = 0) 
               ; 
      }

      //------------------------------------------------------------------------------------
      inline bool operator< ( value_t rhs ) const { return value_ < rhs ; }
      inline bool operator> ( value_t rhs ) const { return value_ > rhs ; }
      inline bool operator==( value_t rhs ) const { return value_ == rhs ; }
      inline bool operator!=( value_t rhs ) const { return value_ != rhs ; }

      //------------------------------------------------------------------------------------
      inline 
      void 
      operator=( value_t rhs ) 
      { 
        if( rhs != value_ ) 
        { count_ = 1 ;
          value_ = rhs ;
        }
        else 
          ++count_ ;
      }

      //------------------------------------------------------------------------------------
      inline 
      void
      operator=( const Member & rhs ) 
      { value_ = rhs.value_ ;
        count_ = rhs.count_ ;
      }
    } ;
    
    //------------------------------------------------------------------------
    typedef Member<value_t, counter_t>       member_t ;
    typedef flat_multiset_iterator<member_t> iterator_t ;
    typedef iterator_t iterator ;

  private :
    //------------------------------------------------------------------------
    uint32_t   capacity_  alignas( system::cpu::Cache_Line_Size ) ; 
    uint32_t   size_      alignas( system::cpu::Cache_Line_Size ) ;
    uint32_t   m_size_    alignas( system::cpu::Cache_Line_Size ) ;
    member_t * data_ ;

    //------------------------------------------------------------------------
    inline int32_t find_member_index( T target ) const ;
    inline int32_t find_insert_index( T target ) const ;

  public :
    //------------------------------------------------------------------------
    inline FlatIntegralMultiSet()  ;
    inline FlatIntegralMultiSet( uint32_t capacity )  ;
    inline ~FlatIntegralMultiSet() ;

    //------------------------------------------------------------------------
    // Note: The begin() and end() iterators iterate over DISTINCT values
    //       only.
    // 
    // TODO: Add iteration over all values.
    //------------------------------------------------------------------------
    inline iterator begin() const { return iterator( &data_[ 0 ] ) ; }
    inline iterator end  () const { return iterator( &data_[ size_ ] ) ; }

    //------------------------------------------------------------------------
    // size() vs m_size() 
    // The size() function returns the number of distinct values in the set.
    // The m_size() function returns the number of values inserted in the set
    // (including duplicates)
    //------------------------------------------------------------------------
    inline uint32_t size()       const { return size_ ; }     
    inline uint32_t m_size()     const { return m_size_ ; }

    //------------------------------------------------------------------------
    inline uint32_t capacity()   const { return capacity_ ; } 
    inline uint32_t free_slots() const { return ( capacity_ - size_ ) ; }
    inline bool     empty()      const { return size_ == 0 ; }

    //------------------------------------------------------------------------
    inline bool reserve( uint32_t min_free_slots ) ;

    //------------------------------------------------------------------------
    inline void clear() ;

    //------------------------------------------------------------------------
    // Note: The operator[] members are used by the selected comparison
    //       functor to during sorting operations.  Don't create a non-const 
    //       version or it risks breaking the sort.
    //------------------------------------------------------------------------
    inline T operator[]( uint32_t idx ) const ;

    //------------------------------------------------------------------------
    inline iterator find  ( T target ) const ;
    inline iterator insert( T target ) ;
    inline iterator erase ( T target ) ;
    inline iterator erase ( iterator itr ) ;
  } ;

  //----------------------------------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  FlatIntegralMultiSet<T, T_Args...>::
  FlatIntegralMultiSet() 
    : capacity_( 0 ) 
    , size_    ( 0 )
    , m_size_  ( 0 )
    , data_    ( NULL ) 
  { 
    reserve( Default_Capacity ) ;
  }

  //----------------------------------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  FlatIntegralMultiSet<T, T_Args...>::
  FlatIntegralMultiSet( uint32_t capacity ) 
    : capacity_( 0 ) 
    , size_    ( 0 )
    , m_size_  ( 0 )
    , data_    ( NULL ) 
  { 
    reserve( capacity ) ;
  }

  //----------------------------------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  FlatIntegralMultiSet<T, T_Args...>::
  ~FlatIntegralMultiSet() 
  {
    if( data_ ) 
    { delete [] data_ ; 
      data_ = NULL ;
    }
    capacity_ = 0 ;
    size_     = 0 ;
    m_size_   = 0 ;
  }

  //----------------------------------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  int32_t
  FlatIntegralMultiSet<T, T_Args...>::
  find_member_index( T target ) const 
  {
    typedef typename std::remove_reference<decltype( *this ) >::type    this_t ;
    typedef typename container::algos::BinarySearch<this_t, compare_t>  search_t ;
    return search_t::find_existing( target, *this, size_ ) ;
  }

  //----------------------------------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  int32_t
  FlatIntegralMultiSet <T, T_Args...>::
  find_insert_index( T target ) const 
  {
    typedef typename std::remove_reference<decltype( *this ) >::type   this_t ;
    typedef typename container::algos::BinarySearch<this_t, compare_t> search_t ;
    return search_t::find_position( target, *this, size_ ) ;
  }

  //----------------------------------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  void 
  FlatIntegralMultiSet <T, T_Args...>::
  clear() 
  { 
    size_   = 0 ;
    m_size_ = 0 ;
  }

  //----------------------------------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  typename FlatIntegralMultiSet <T, T_Args...>::iterator 
  FlatIntegralMultiSet <T, T_Args...>::
  find( T target ) const 
  {
    int32_t mbr_idx = find_member_index( target ) ;
    return ( mbr_idx < 0 ) 
           ? end() 
           : iterator( &data_[ mbr_idx ] ) 
           ;
  }

  //----------------------------------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  T 
  FlatIntegralMultiSet <T, T_Args...>::
  operator[]( uint32_t idx ) const 
  {
    return data_[ idx ].value() ;
  }

  //----------------------------------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  typename FlatIntegralMultiSet <T, T_Args...>::iterator 
  FlatIntegralMultiSet <T, T_Args...>::
  insert( T value ) 
  {
    if( empty() ) 
    { 
      data_[ 0 ] = value ; 
      ++size_ ;
      ++m_size_ ;
      return begin() ;
    }

    int32_t idx = find_insert_index( value ) ;

    //
    // The following conditional block is entered when the insert index 
    // targets a currently occupied location.
    // 
    if( idx < size_ )
    {
      if( data_[ idx ] == value ) 
      { 
        ++data_[ idx ] ; // Increment counter if already present
        ++m_size_ ;
        return iterator( &data_[ idx ] ) ;
      }
  
      if( compare_t::lt( data_[ idx ].value(), value ) ) 
        ++idx ;

      util::intrinsic::memmove( &data_[ idx+1 ], &data_[ idx ], sizeof( member_t ) * (size_ - idx) ) ;
      data_[ idx ] = value ;
      ++size_ ;
      ++m_size_ ;
    }
    //
    // If we enter this block, the insert index specifies an index that's 
    // potentially outside of our current allocation.  
    // 
    else 
    {
      // Make sure we have room for the new 
      if( idx >= capacity_ )
      { 
        // If we fail to reserve more space, return end() 
        if( !reserve( capacity_ ) ) 
          return end() ;
      }
      
      data_[ idx ] = value ;
      ++size_ ;
      ++m_size_ ;
    }

    return iterator( &data_[ idx ] ) ;
  }

  //----------------------------------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  typename FlatIntegralMultiSet <T, T_Args...>::iterator 
  FlatIntegralMultiSet <T, T_Args...>::
  erase( T value ) 
  {
    int32_t idx = find_member_index( value ) ;
    if( idx < 0 ) 
      return end() ;
    
    --data_[ idx ] ;
    --m_size_ ;
    if( data_[ idx ].count() == 0 ) 
    {
      if( idx < ( size_ - 1 ) ) 
        util::intrinsic::memmove( &data_[ idx ], &data_[ idx + 1 ], sizeof( member_t ) * (size_ - idx) ) ;

      --size_ ;
    }

    return iterator( &data_[ idx ] ) ;
  }

  //----------------------------------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  typename FlatIntegralMultiSet <T, T_Args...>::iterator 
  FlatIntegralMultiSet <T, T_Args...>::
  erase( iterator itr ) 
  {
    // TODO: This should probably trigger a fatal exception since it indicates improper 
    //       usage of the container.
    if( empty() || !itr.bounds_test( begin(), end() ) ) 
      return end() ;

    int64_t idx = itr.distance_from( begin() ) ;
    --data_[ idx ] ;
    --m_size_ ;
    if( data_[ idx ].count() == 0 ) 
    {
      if( idx < (size_ - 1) )
        util::intrinsic::memmove( &data_[ idx ], &data_[ idx + 1 ], sizeof( member_t ) * (size_ - idx) ) ;
      --size_ ;
    }

    return iterator( &data_[ idx ] ) ;
  }

  //----------------------------------------------------------------------------------------------------
  template<typename T, typename... T_Args>
  bool
  FlatIntegralMultiSet <T, T_Args...>::
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

    if( Max_Capacity > 0 && new_cap > Max_Capacity ) 
    { 
      if( capacity_ >= Max_Capacity ) 
        return false ;

      new_cap = Max_Capacity ;
    }

    // Save current array content
    member_t * old_data = data_ ;

    // Allocate new array with increased capacity, and include an extra 
    // slot to make the end() iterator's implementation less complex.
    data_ = new member_t[ new_cap + 1 ] ;
    if( fps_unlikely( data_ == NULL ) )
    { data_ = old_data ;
      return false ;
    }
    
    if( size_ > 0 ) 
      util::intrinsic::memcpy( data_, old_data, sizeof( member_t  ) * size_ ) ;

    capacity_ = new_cap ;
    delete [] old_data ;
    return true ;
  }

}}}

#endif
