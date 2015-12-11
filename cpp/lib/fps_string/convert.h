#ifndef FPS__STRING__CONVERT__H
#define FPS__STRING__CONVERT__H

#include <stdint.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cctype>

#include "fps_math/logarithm.h"
#include "fps_string/format.h"

//---------------------------------------------------------------------------
namespace fps     {
namespace string  {

  //-----------------------------------------------------------------------
  inline
  std::string 
  lower( const std::string & dest ) 
  { 
    std::string rv( dest ) ;
    std::transform( rv.begin(), rv.end(), rv.begin(), ::tolower ) ;
    return rv ;
  }

  //-----------------------------------------------------------------------
  inline
  std::string 
  upper( const std::string & dest ) 
  { 
    std::string rv( dest ) ;
    std::transform( rv.begin(), rv.end(), rv.begin(), ::toupper ) ;
    return rv ;
  }

  //-----------------------------------------------------------------------
  inline
  std::string & 
  to_upper( std::string & dest ) 
  { 
    std::transform( dest.begin(), dest.end(), dest.begin(), ::toupper ) ;
    return dest ;
  }

  //-----------------------------------------------------------------------
  inline
  std::string & 
  to_lower( std::string & dest ) 
  { 
    std::transform( dest.begin(), dest.end(), dest.begin(), ::tolower ) ;
    return dest ;
  }


  //-------------------------------------------------------------------------
  namespace convert 
  {
    //-------------------------------------------------------------------------
    // String to floating point 
    //-------------------------------------------------------------------------
    inline long double to_long_double( const char * buf, uint32_t buf_len ) ;
    inline double      to_double     ( const char * buf, uint32_t buf_len ) ;
    inline float       to_float      ( const char * buf, uint32_t buf_len ) ;

    //-------------------------------------------------------------------------
    // String to integer 
    //-------------------------------------------------------------------------
    template<typename T> T to_unsigned( const char * buf, uint32_t buf_len ) ;
    template<typename T> T to_signed  ( const char * buf, uint32_t buf_len ) ;


    //-------------------------------------------------------------------------
    // Conversion struct that can be specialized for specific type conversions.
    //-------------------------------------------------------------------------
    namespace detail { template<typename T> struct Convert ; }

    //-------------------------------------------------------------------------
    // Convienence function that selects the appropriate detail::Convert::to
    // specialization for the template argument type.
    //-------------------------------------------------------------------------
    template<typename T>
    T 
    to( const char * buf, uint32_t buf_len ) 
    { 
      return detail::Convert<T>::to( buf, buf_len ) ;
    }

    //-------------------------------------------------------------------------
    // Convienence function that selects the appropriate detail::Convert::from
    // specialization for the template argument type.
    //-------------------------------------------------------------------------
    template<typename T>
    std::string
    from( T value ) 
    { 
      return detail::Convert<T>::from( value ) ;
    }

    //-------------------------------------------------------------------------
    long double 
    to_long_double( const char * buf, uint32_t buf_len ) 
    { 
      return ( buf_len == 0 || buf == NULL ) 
             ? static_cast<long double>( 0 ) 
             : ::strtold( buf, NULL ) 
             ; 
    }

    //-------------------------------------------------------------------------
    double 
    to_double( const char * buf, uint32_t buf_len ) 
    { 
      return ( buf_len == 0 || buf == NULL ) 
             ? static_cast<double>( 0 )
             : ::strtod( buf, NULL ) 
             ; 
    }

    //-------------------------------------------------------------------------
    float 
    to_float( const char * buf, uint32_t buf_len ) 
    { 
      return ( buf_len == 0 || buf == NULL ) 
             ? static_cast<float>( 0 )
             : ::strtof( buf, NULL ) 
             ; 
    }

    //----------------------------------------------------------------------------------------
    // Convert an ascii encoded unsigned integer to binary representation.
    // Skips leading whitespace, aborts at first non-digit character after 
    // leading whitespace or at end of string.
    //----------------------------------------------------------------------------------------
    template<typename T>
    T
    to_unsigned( const char * buf, uint32_t buf_len ) 
    {
      const char * buf_itr = buf ;
      const char * buf_end = buf + buf_len ;
      T            rv( 0 ) ;

      while( buf_itr < buf_end && ::isspace( *buf_itr ) )
        ++buf_itr ;

      while( buf_itr < buf_end && ::isdigit( *buf_itr ) )
        rv = (rv * 10) + (*buf_itr++ - '0') ;
      
      return rv ;
    }

    //----------------------------------------------------------------------------------------
    template<typename T>
    T
    to_unsigned( const char * buf ) 
    { 
      return to_unsigned<T>( buf, ::strlen( buf ) ) ; 
    }


    //----------------------------------------------------------------------------------------
    // Convert an asci encoded signed integer to binary representation.
    // Skips leading whitespace, aborts at first non-digit character after leading whitespace 
    // and option sign prefix "-" or at end of string.
    //----------------------------------------------------------------------------------------
    template <typename T>
    T
    to_signed( const char * buf, uint32_t buf_len )
    { 
      const char * buf_itr = buf ;
      const char * buf_end = buf + buf_len ;
      T            mult( 1 ) ;

      while( buf_itr < buf_end && ::isspace( *buf_itr ) )
        ++buf_itr ;

      if( buf_itr < buf_end && *buf_itr == '-' ) 
      { 
        mult = -1 ;
        ++buf_itr ;
      }

      return mult 
           * static_cast<T>( to_unsigned<T>( buf_itr, static_cast<uint32_t>( buf_end - buf_itr ) ) ) ;
    }

    //----------------------------------------------------------------------------------------
    template <typename T>
    T 
    to_signed( const char * buf ) 
    { return to_signed<T>( buf, ::strlen( buf ) ) ; 
    }

    //----------------------------------------------------------------------------------------
    // Write the ascii representation of the unsigned value parameter into a std::string
    //----------------------------------------------------------------------------------------
    template<typename T>
    std::string 
    from_unsigned( T value )
    {
      std::string dest ;
      if( value == 0 )
      { 
        dest.push_back( '0' ) ;
        return dest ;
      }

      uint32_t digit_idx = math::fast_log10< T >( value ) ;
      dest.resize( digit_idx + 1 ) ;
      do 
      {
        T temp            = (value/10) * 10 ;
        dest[ digit_idx ] = static_cast<char>('0' + (value - temp)) ;
        value             = value / 10 ;
      } while( digit_idx-- ) ;

      return dest ;
    }

    //----------------------------------------------------------------------------------------
    template< typename T >
    std::string 
    from_signed( T value )
    {
      std::string dest ;

      // Check for zero case early because fast_log10( 0 ) triggers a crash
      if( value == 0 )
      { dest.push_back( '0' ) ;
        return dest ;
      }

      bool     is_neg = ( value < 0 ) ;
      uint32_t stop_idx    = 0 ;
      if( is_neg )  
      {
        value    = -value ;
        stop_idx = 1 ;
        dest.push_back( '-' ) ;
      }

      uint32_t digit_idx = math::fast_log10<T>( value ) + stop_idx ;
      dest.resize( digit_idx + 1 ) ;
      do 
      {
        T temp            = (value/10) * 10 ;
        dest[ digit_idx ] = static_cast<char>( '0' + (value - temp) ) ;
        value             = value / 10 ;
      } while( digit_idx-- != stop_idx ) ;

      return dest ;
    }

    //----------------------------------------------------------------------------------------
    template<typename T>
    int32_t 
    write_unsigned( T value, char * dest, uint32_t dest_len )
    {
      if( dest == NULL || dest_len == 0 ) 
        return -1 ;

      // Check for zero case early because fast_log10( 0 ) triggers a crash
      if( value == 0 )
      { dest[ 0 ] = '0' ;
        return 1 ; 
      }

      uint32_t digit_idx = (uint32_t)math::fast_log10< T >( value ) ;
      uint32_t req_len   = digit_idx + 1 ;
      if( req_len > dest_len ) 
        return 0 ;

      do 
      {
        T temp            = (value/10) * 10 ;
        dest[ digit_idx ] = static_cast<char>('0' + (value - temp)) ;
        value             = value / 10 ;
      } while( digit_idx-- ) ;

      return static_cast<int32_t>( req_len ) ;
    }

    //----------------------------------------------------------------------------------------
    template<typename T>
    int32_t
    write_signed( T value, char * dest, uint32_t dest_len )
    {
      if( dest == NULL || dest_len == 0 ) 
        return -1 ;

      // Check for zero case early because fast_log10( 0 ) triggers a crash
      if( value == 0 )
      { dest[ 0 ] = '0' ;
        return 1 ;
      }

      bool     is_neg   = ( value < 0 ) ;
      uint32_t stop_idx = 0 ;
      if( is_neg )  
      { value    = -value ;
        stop_idx = 1 ;
      }

      //
      // Find length of output string using fast log10 implementation.
      //
      uint32_t digit_idx = math::fast_log10<T>( value ) + stop_idx ;
      uint32_t req_len   = digit_idx + 1 ;
      if( req_len > dest_len ) 
        return 0 ;

      do 
      {
        T temp            = (value/10) * 10 ;
        dest[ digit_idx ] = static_cast<char>( '0' + (value - temp) ) ;
        value             = value / 10 ;
      } while( digit_idx-- != stop_idx ) ;

      if( is_neg ) 
        dest[ 0 ] = '-' ;

      return static_cast<int32_t>( req_len ) ;
    }

    //-------------------------------------------------------------------------
    // 
    namespace detail 
    { 
      //-----------------------------------------------------------------------
      template<>
      struct Convert<int8_t>
      { typedef int8_t value_t ;

        //---------------------------------------------------------------------
        static 
        value_t 
        to( const char * buf, uint32_t len ) 
        { 
          return to_signed<value_t>( buf, len ) ; 
        }

        //---------------------------------------------------------------------
        static 
        std::string 
        from( value_t value ) 
        { 
          return from_signed( value ) ; 
        }

        //---------------------------------------------------------------------
        static 
        int32_t 
        into_buffer( value_t value, char * buf, uint32_t len ) 
        { 
          return write_signed( value, buf, len ) ;
        }

      } ;

      //----------------------------------------------------
      template<>
      struct Convert<int16_t>
      { typedef int16_t value_t ;

        //--------------------------------------------------
        static 
        value_t 
        to( const char * buf, uint32_t len )
        { 
          return to_signed<value_t>( buf, len ) ;
        }

        //--------------------------------------------------
        static
        std::string
        from( value_t value ) 
        { 
          return from_signed( value ) ;
        }
      } ;

      //----------------------------------------------------
      template<>
      struct Convert<int32_t>
      { typedef int32_t value_t ;

        //--------------------------------------------------
        static 
        value_t 
        to( const char * buf, uint32_t len )
        { 
          return to_signed<value_t>( buf, len ) ;
        }

        //--------------------------------------------------
        static
        std::string
        from( value_t value ) 
        {
          return from_signed( value ) ;
        }

        //--------------------------------------------------
        static 
        int32_t 
        into_buffer( value_t value, char * buf, uint32_t len ) 
        { 
          return write_signed( value, buf, len ) ;
        }
      } ;

      //----------------------------------------------------
      template<>
      struct Convert<int64_t>
      { typedef int64_t value_t ;

        //--------------------------------------------------
        static 
        value_t 
        to( const char * buf, uint32_t len )
        { 
          return to_signed<value_t>( buf, len ) ;
        }

        //--------------------------------------------------
        static
        std::string
        from( value_t value ) 
        { 
          return from_signed( value ) ;
        }

        //--------------------------------------------------
        static 
        int32_t 
        into_buffer( value_t value, char * buf, uint32_t len ) 
        { 
          return write_signed( value, buf, len ) ;
        }
      } ;

      //----------------------------------------------------
      template<>
      struct Convert<uint8_t>
      { typedef uint8_t value_t ;

        //--------------------------------------------------
        static 
        value_t 
        to( const char * buf, uint32_t len )
        { 
          return to_signed<value_t>( buf, len ) ;
        }

        //--------------------------------------------------
        static
        std::string
        from( value_t value ) 
        { 
          return from_unsigned( value ) ;
        }
      } ;

      //----------------------------------------------------
      template<>
      struct Convert<uint16_t>
      { typedef uint16_t value_t ;

        //--------------------------------------------------
        static 
        value_t 
        to( const char * buf, uint32_t len )
        { 
          return to_signed<value_t>( buf, len ) ;
        }

        //--------------------------------------------------
        static
        std::string
        from( value_t value ) 
        { 
          return from_unsigned( value ) ;
        }
      } ;

      //----------------------------------------------------
      template<>
      struct Convert<uint32_t>
      { typedef uint32_t value_t ;

        //--------------------------------------------------
        static 
        value_t 
        to( const char * buf, uint32_t len )
        { 
          return to_signed<value_t>( buf, len ) ;
        }

        //--------------------------------------------------
        static
        std::string
        from( value_t value ) 
        { 
          return from_unsigned( value ) ;
        }
      } ;

      //----------------------------------------------------
      template<>
      struct Convert<uint64_t>
      { typedef uint64_t value_t ;

        //--------------------------------------------------
        static 
        value_t 
        to( const char * buf, uint32_t len )
        { 
          return to_signed<value_t>( buf, len ) ;
        }

        //--------------------------------------------------
        static
        std::string
        from( value_t value ) 
        { 
          return from_unsigned( value ) ;
        }
      } ;

      //----------------------------------------------------
      template<>
      struct Convert<float>
      { typedef float value_t ;

        //--------------------------------------------------
        static 
        value_t 
        to( const char * buf, uint32_t len )
        { 
          return to_float( buf, len ) ;
        }

        //--------------------------------------------------
        static
        std::string
        from( value_t value ) 
        { 
          return string::sprintf( "%0.12f", value ) ;
        }
      } ;

      //----------------------------------------------------
      template<>
      struct Convert<double>
      { typedef double value_t ;

        //--------------------------------------------------
        static 
        value_t 
        to( const char * buf, uint32_t len )
        { 
          return to_double( buf, len ) ;
        }

        //--------------------------------------------------
        static
        std::string
        from( value_t value ) 
        { 
          return string::sprintf( "%0.12f", value ) ;
        }
      } ;

      //----------------------------------------------------
      template<>
      struct Convert<long double>
      { typedef long double value_t ;

        //--------------------------------------------------
        static 
        value_t 
        to( const char * buf, uint32_t len )
        { 
          return to_long_double( buf, len ) ;
        }

        //--------------------------------------------------
        static
        std::string
        from( value_t value ) 
        { 
          return string::sprintf( "%0.12lf", value ) ;
        }
      } ;
    } 
  }
}}
#endif
