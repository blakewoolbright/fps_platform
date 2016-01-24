#ifndef FPS__CONTAINER__ALGORITHMS__H
#define FPS__CONTAINER__ALGORITHMS__H

#include "fps_util/macros.h"
#include "fps_container/comparators.h"
#include <cstdint>
#include <type_traits>

namespace fps  {
namespace container {
namespace algos {

  //------------------------------------------------------------------------------------
  template< typename T_Container
          , typename T_Order=compare::Ascending<typename T_Container::value_t> 
          > 
  struct BinarySearch
  {
    //----------------------------------------------------------------------------------
    typedef T_Container                   container_t ;
    typedef typename T_Container::value_t value_t ;

    //----------------------------------------------------------------------------------
    static const bool Is_Integral = std::is_integral<value_t>::value ;

    //----------------------------------------------------------------------------------
    static 
    inline 
    int32_t 
    find_existing( value_t value, const container_t & c_ref, uint32_t c_size ) 
    {
      if( fps_unlikely( c_size == 0 ) )
        return -1 ;

      int32_t  i_min = 0 ;
      int32_t  i_max = c_size - 1 ;
      int32_t  i_mid = 0 ;
      while( i_min <= i_max )
      {
        i_mid = (i_min + i_max) >> 1 ;

        if( T_Order::gt( value, c_ref[ i_mid ] ) ) 
        {
          i_min = i_mid + 1 ;
        }
        else if( T_Order::lt( value, c_ref[ i_mid ] ) ) 
        {
          i_max = i_mid - 1 ;
        }
        else
        {
          return i_mid ;
        }
      }

      return -1 ;
    }

    //----------------------------------------------------------------------------------
    static 
    inline 
    int32_t 
    find_position( value_t value, const container_t & c_ref, uint32_t c_size ) 
    {
      if( fps_unlikely( c_size == 0 ) )
        return 0 ;

      int32_t  i_min = 0 ;
      int32_t  i_max = c_size - 1 ;
      int32_t  i_mid = 0 ;
      while( i_min <= i_max )
      {
        i_mid = (i_min + i_max) >> 1 ;

        if( T_Order::gt( value, c_ref[ i_mid ] ) ) 
        { 
          i_min = i_mid + 1 ;
        }
        else if( T_Order::lt( value, c_ref[ i_mid ] ) ) 
        { 
          i_max = i_mid - 1 ;
        }
        else 
        { 
          return i_mid ;
        }
      }

      return i_mid ;
    }
  } ;

  //----------------------------------------------------------------------------------
  // Linear search implementation.
  //----------------------------------------------------------------------------------
  template< typename T_Container
          , typename T_Order=compare::Ascending<typename T_Container::value_t> 
          >
  struct LinearSearch
  {
    //--------------------------------------------------------------------------------
    typedef T_Container                   container_t ;
    typedef typename T_Container::value_t value_t ;
  
    //--------------------------------------------------------------------------------
    static 
    inline 
    int32_t 
    find_existing( value_t value, const container_t & c_ref, uint32_t c_size ) 
    {
      int32_t rv = 0 ;
      for( ; (rv < c_size) && T_Order::lt(c_ref[ rv ], value) ; ++rv ) ;
      return (rv < c_size && value==c_ref[ rv ]) ? rv : -1 ;
    }
  
    //--------------------------------------------------------------------------------
    static 
    inline 
    int32_t 
    find_position( value_t value, const container_t & c_ref, uint32_t c_size ) 
    {
      int32_t rv = 0 ;
      for( ; (rv < c_size) && T_Order::lt(c_ref[ rv ], value) ; ++rv ) ;
      return rv ;
    }
  } ;

}}}

#endif
