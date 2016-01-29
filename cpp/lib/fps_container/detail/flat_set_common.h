#ifndef FPS__CONTAINER__DETAIL__FLAT_SET_COMMON__H
#define FPS__CONTAINER__DETAIL__FLAT_SET_COMMON__H

#include "fps_container/comparators.h"
#include "fps_util/macros.h"
#include "fps_system/fps_system.h"

#include <cstdint>
#include <type_traits>
#include <boost/iterator/iterator_facade.hpp>

namespace fps    {
namespace container {
namespace detail 
{
  //--------------------------------------------------------------------------------------
  // Shared iterator definition for FlatIntegralSet, FlatObjectSet.
  //--------------------------------------------------------------------------------------
  template<typename T>
  struct flat_set_iterator 
    : public boost::iterator_facade< flat_set_iterator<T>
                                   , T
                                   , boost::forward_traversal_tag
                                   , T 
                                   >
  {
  private :
    //------------------------------------------------------------------------------
    friend class boost::iterator_core_access ;

    //------------------------------------------------------------------------------
    const T * ptr_ ;

    //------------------------------------------------------------------------------
    inline void increment() { ++ptr_; } 

    //------------------------------------------------------------------------------
    inline 
    bool 
    equal( const flat_set_iterator & rhs ) const 
    { return (ptr_ == rhs.ptr_) ; 
    }

    //------------------------------------------------------------------------------
    inline T dereference() const { return *ptr_ ; }

  public :
    //-----------------------------------------------------------------
    inline 
    flat_set_iterator() 
      : ptr_( NULL ) 
    {}

    //-----------------------------------------------------------------
    inline 
    explicit 
    flat_set_iterator( const T * ptr ) 
      : ptr_( ptr ) 
    {}

    //-----------------------------------------------------------------
    inline T         value() const { return *ptr_ ; }
    inline uint32_t  count() const { return 1 ; }

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

  //--------------------------------------------------------------------------------------
  // Shared iterator definition for FlatIntegralSet, FlatObjectSet.
  //--------------------------------------------------------------------------------------
  template<typename T>
  struct flat_multiset_iterator 
    : public boost::iterator_facade< flat_multiset_iterator<T>
                                   , typename T::value_t
                                   , boost::forward_traversal_tag
                                   , const typename T::value_t & 
                                   >
  {
  public :
    typedef typename T::value_t   value_t ;
    typedef typename T::counter_t counter_t ;

  private :
    friend class boost::iterator_core_access ;
    const T * ptr_ ;

  public :
    //-----------------------------------------------------------------
    inline 
    flat_multiset_iterator() 
      : ptr_( NULL ) 
    {}

    //-----------------------------------------------------------------
    inline 
    explicit 
    flat_multiset_iterator( const T * ptr ) 
      : ptr_( ptr ) 
    {}

    //-----------------------------------------------------------------
    inline const value_t & value() const { return ptr_->value() ; }
    inline counter_t       count() const { return ptr_->count() ; }

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

  private :
    //------------------------------------------------------------------------------
    inline void increment() { ++ptr_; } 

    //------------------------------------------------------------------------------
    inline 
    const value_t &
    dereference() const 
    { return ptr_->value() ; 
    }

    //------------------------------------------------------------------------------
    inline 
    bool 
    equal( const flat_multiset_iterator & rhs ) const 
    { 
      return (ptr_ == rhs.ptr_) ; 
    }
  } ;

  //--------------------------------------------------------------------------------------
  // The following "Construct" class is used to default-construct container members.  
  // User defined types are default constructed w/ placement new.  
  // Integral/primitive types are unaffected.
  //--------------------------------------------------------------------------------------
  template<typename T, typename T_Enable=void>
  struct Construct 
  {  
    inline void operator()( T * arg ) { new ( arg ) T() ; }
  } ;

  //--------------------------------------------------------------------------------------
  template<typename T>
  struct Construct<T, typename std::enable_if< std::is_integral<T>::value >::type>
  {  
    // inline void operator()( T * arg ) { *arg = 0 ; }
    inline void operator()( T * arg ) {}
  } ;

  //--------------------------------------------------------------------------------------
  // The following "Assign" class is used to copy container members.  
  // User defined types are copy constructed via placement new. 
  // Primitive types are directly assigned via the "=" operator.
  //--------------------------------------------------------------------------------------
  template<typename T, typename T_Enable=void>
  struct Assign 
  {  
    inline void operator()( T * arg, const T & src ) { new ( arg ) T( src ) ; }
  } ;

  //--------------------------------------------------------------------------------------
  template<typename T>
  struct Assign<T, typename std::enable_if< std::is_integral<T>::value >::type>
  {  
    inline void operator()( T * arg, T src ) { *arg = src ; }
  } ;

  //--------------------------------------------------------------------------------------
  // The following "Destruct" class is used to destroy container members.
  // User defined types have their destructor invoked, and primitive 
  // types are ignored/passed through.
  //--------------------------------------------------------------------------------------
  template<typename T, typename T_Enable=void>
  struct Destruct 
  { 
    inline void operator()( T * arg ) { arg->~T() ; }
  } ;

  //--------------------------------------------------------------------------------------
  template<typename T>
  struct Destruct<T, typename std::enable_if< std::is_integral<T>::value >::type>
  {  
    inline void operator()( T * arg ) {} 
  } ;

}}}
#endif
