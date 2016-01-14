#ifndef FPS__MATH__LOGARITHM_H
#define FPS__MATH__LOGARITHM_H

#include <cstdint>

namespace fps {
namespace math {

  namespace detail 
  {
    //--------------------------------------------------------------------------------------------
    //
    // Note : These logarithm generators aren't equipped to handle the value 0 (zero).  Calling
    //        calculate will 
    //
    //--------------------------------------------------------------------------------------------
    template< typename T > struct Log2 {} ;

    //--------------------------------------------------------------------------------------------
    template<> 
    struct Log2<uint64_t> 
    {
      static 
      inline 
      uint64_t 
      calculate( uint64_t value ) 
      {
        uint64_t msg_pos = 64 - __builtin_clzll(value) ;
        return ( value == (((uint64_t)1) << msg_pos ) )
               ? msg_pos 
               : msg_pos - 1 ;
      }
    } ;

    //--------------------------------------------------------------------------------------------
    template<>
    struct Log2<uint32_t> 
    {
      static
      inline
      uint32_t calculate( uint32_t value ) 
      {
        uint32_t msg_pos = 32 - __builtin_clz(value) ;
        return ( value == (((uint32_t)1) << msg_pos ) )
               ? msg_pos 
               : msg_pos - 1 ;
      }
    } ;

    //--------------------------------------------------------------------------------------------
    template<> 
    struct Log2<uint16_t> 
    {
      static
      inline
      uint16_t calculate( uint16_t value ) 
      { return Log2<uint32_t>::calculate( static_cast<uint32_t>(value) ) ; 
      }
    } ;

    //--------------------------------------------------------------------------------------------
    template<> 
    struct Log2<uint8_t> 
    {
      static
      inline
      uint8_t calculate( uint8_t value ) 
      { return static_cast<uint8_t>( Log2<uint32_t>::calculate( static_cast<uint32_t>(value) ) ) ; 
      }
    } ;

    //--------------------------------------------------------------------------------------------
    //
    // Note : For negative signed values, these logarithm generators
    //        return the log of the corresponding positive value.
    //
    //--------------------------------------------------------------------------------------------
    template<> 
    struct Log2<int64_t> 
    {
      static
      inline
      int64_t calculate( int64_t value ) 
      { if( value < 0 ) 
          value = -value ; 
        return Log2<uint64_t>::calculate( static_cast<uint64_t>(value) ) ; 
      }
    } ;

    //--------------------------------------------------------------------------------------------
    template<> 
    struct Log2<int32_t> 
    {
      static
      inline
      int32_t calculate( int32_t value ) 
      { if( value < 0 ) 
          value = -value ;
        return Log2<uint32_t>::calculate( static_cast<uint32_t>(value) ) ; 
      }
    } ;

    template<> 
    struct Log2<int16_t> 
    {
      static
      inline
      int16_t calculate( int16_t value ) 
      { if( value < 0 ) 
          value = static_cast<int16_t>(-value) ;
        return Log2<uint16_t>::calculate( static_cast<uint16_t>(value) ) ; 
      }
    } ;

    //
    // Log base 10 lookups
    // 
    static const uint64_t Pow10Table[] = 
    { 1 
    , 10
    , 100
    , 1000
    , 10000
    , 100000             
    , 1000000
    , 10000000
    , 100000000
    , 1000000000
    , 10000000000
    , 100000000000
    , 1000000000000
    , 10000000000000
    , 100000000000000
    , 1000000000000000
    , 10000000000000000
    , 100000000000000000
    , 1000000000000000000
    , 10000000000000000000ULL
    } ;

    //--------------------------------------------------------------------------------------------
    static const int16_t Log10_Mult  = 1233 ; 
    static const int16_t Log10_Shift = 12 ; 
      
    //--------------------------------------------------------------------------------------------
    template< typename T > struct Log10 {} ;
    
    //--------------------------------------------------------------------------------------------
    template<> 
    struct Log10<uint64_t> 
    {
      static
      inline
      uint64_t calculate( uint64_t value ) 
      {
        uint64_t result = (Log2<uint64_t>::calculate(value) + 1) * Log10_Mult >> Log10_Shift ;
        return ( value < Pow10Table[result] ) ? result - 1 : result ; 
      }
    } ;

    //--------------------------------------------------------------------------------------------
    template<> 
    struct Log10<uint32_t> 
    {
      static
      inline
      uint32_t calculate( uint32_t value ) 
      {
        uint32_t result = (Log2<uint32_t>::calculate(value) + 1) * Log10_Mult >> Log10_Shift ;
        return ( value < static_cast<uint32_t>(Pow10Table[result] )) ? result - 1 : result ; 
      }
    } ;

    //--------------------------------------------------------------------------------------------
    template<> 
    struct Log10< uint16_t > 
    {
      static
      inline
      uint16_t calculate( uint16_t value ) 
      {
        return static_cast<uint8_t>( Log10<uint32_t>::calculate( value ) ) ;
      }
    } ;

    //--------------------------------------------------------------------------------------------
    template<> 
    struct Log10< uint8_t > 
    {
      static
      inline
      uint8_t calculate( uint8_t value ) 
      {
        return Log10<uint32_t>::calculate( value ) ;
      }
    } ;

    //--------------------------------------------------------------------------------------------
    template<> 
    struct Log10< int64_t > 
    {
      static
      inline int64_t calculate( int64_t value ) 
      {
        int64_t result = (Log2<uint64_t>::calculate(value)+1) * Log10_Mult >> Log10_Shift ;
        return ( value < static_cast<int64_t>(Pow10Table[result] )) ? result - 1 : result ; 
      }
    } ;

    //--------------------------------------------------------------------------------------------
    template<> 
    struct Log10< int32_t > 
    {
      static
      inline
      int32_t calculate( int32_t value ) 
      {
        int32_t result = (Log2<uint32_t>::calculate(value)+1) * Log10_Mult >> Log10_Shift ;
        return ( value < static_cast<int32_t>(Pow10Table[result] )) ? result - 1 : result ; 
      }
    } ;

    //--------------------------------------------------------------------------------------------
    template<> 
    struct Log10< int16_t > 
    {
      static
      inline int16_t calculate( int16_t value ) 
      {
        return Log10<uint32_t>::calculate( value ) ;
      }
    } ;

    //--------------------------------------------------------------------------------------------
    template<> 
    struct Log10< int8_t > 
    {
      static
      inline
      int8_t calculate( int8_t value ) 
      {
        return Log10<uint32_t>::calculate( value ) ;
      }
    } ;
  }

  //----------------------------------------------------------------------------------------------
  template<typename T>
  T 
  fast_log2( T value )
  { return detail::Log2<T>::calculate( value ) ;
  }

  //----------------------------------------------------------------------------------------------
  template<typename T>
  T 
  fast_log10( T value )
  { return detail::Log10<T>::calculate( value ) ;
  }

}}
#endif
