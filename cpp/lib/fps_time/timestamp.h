#ifndef FPS__TIME__TIMESTAMP__H
#define FPS__TIME__TIMESTAMP__H 

#include "fps_time/clock.h"
#include "fps_time/constants.h"

#include <cstring>
#include <string>

namespace fps  {
namespace time {

  class Datetime ;

  //---------------------------------------------------------------------------------------------------------
  // Timestamp
  // Wrapper for nanoseconds since unix epoch.
  //---------------------------------------------------------------------------------------------------------
  class Timestamp 
  {
    uint64_t value_ ;

  public :

    //-------------------------------------------------------------------------------------------------------
    inline Timestamp() : value_( 0 ) {}

    //-------------------------------------------------------------------------------------------------------
    explicit 
    inline 
    Timestamp( uint64_t nanos ) 
      : value_( nanos ) 
    {}

    //-------------------------------------------------------------------------------------------------------
    Timestamp( const Datetime & src ) ; 

    //-------------------------------------------------------------------------------------------------------
    inline Timestamp & operator= ( const Timestamp & rhs )       { value_ = rhs.value_ ; return *this ; }
    inline bool        operator> ( const Timestamp & rhs ) const { return value_ >  rhs.value_; }
    inline bool        operator>=( const Timestamp & rhs ) const { return value_ >= rhs.value_; }
    inline bool        operator< ( const Timestamp & rhs ) const { return value_ <  rhs.value_; }
    inline bool        operator<=( const Timestamp & rhs ) const { return value_ <= rhs.value_; }
    inline bool        operator==( const Timestamp & rhs ) const { return value_ == rhs.value_; }
    inline bool        operator!=( const Timestamp & rhs ) const { return value_ != rhs.value_; }

    //-------------------------------------------------------------------------------------------------------
    inline uint64_t epoch_seconds() const { return value_ / time::Nanos_Per_Second ; }
    inline uint64_t epoch_millis () const { return value_ / time::Nanos_Per_Milli ; }
    inline uint64_t epoch_micros () const { return value_ / time::Nanos_Per_Micro ; }
    inline uint64_t epoch_nanos  () const { return value_ ; }
    Timestamp       midnight()      const ;

    //---------------------------------------------------------------------------------------------------------------
    // TODO: Should these be implemented via operator+/-?  Should the concept of a time delta be strongly typed?
    //---------------------------------------------------------------------------------------------------------------
    inline Timestamp add_days   ( int64_t val ) const { return Timestamp( value_ + (val* time::Nanos_Per_Day) ) ; }
    inline Timestamp add_seconds( int64_t val ) const { return Timestamp( value_ + (val* time::Nanos_Per_Second)) ; }
    inline Timestamp add_nanos  ( int64_t val ) const { return Timestamp( value_ + val ) ; }

    //-------------------------------------------------------------------------------------------------------
    template<time::Format F=time::TS_Nano> std::string to_string() const ;

    //-------------------------------------------------------------------------------------------------------
    // template<time::Format T> static Timestamp from_string( const char * ) ;
    // template<time::Format T> static Timestamp from_string( const fps::String & ) ;

    //-------------------------------------------------------------------------------------------------------
    static inline Timestamp now() { return Timestamp( Clock::now() ) ; }
  } ;

}}

#endif

