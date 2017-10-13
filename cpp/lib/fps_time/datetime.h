#ifndef FPS__TIME__DATETIME__H
#define FPS__TIME__DATETIME__H

#include "fps_time/clock.h"
#include "fps_time/constants.h"
#include <cstring>
#include <string>

namespace fps  {
namespace time {

  //----------------------------------------------------------------------------------------
  class Timestamp ;

  //----------------------------------------------------------------------------------------
  class Datetime
  {
  private :
    friend class fps::time::Timestamp ;

    struct tm raw_ ; 
    uint32_t  nanos_   ;     

  public :
    //--------------------------------------------------------------------------------------
    inline Datetime() : nanos_( 0 ) { clear() ; }

    //--------------------------------------------------------------------------------------
    inline 
    Datetime( const Datetime & rhs ) 
      : nanos_( rhs.nanos_ ) 
    { 
      std::memcpy( &raw_, &(rhs.raw_), sizeof( struct tm ) ) ;
    }

    //--------------------------------------------------------------------------------------
    Datetime( const Timestamp & ts ) ;
    explicit Datetime( uint64_t epoch_nanos ) ;

    //--------------------------------------------------------------------------------------
    inline 
    Datetime & 
    operator=( const Datetime & rhs ) 
    {
      std::memcpy( &raw_, &(rhs.raw_), sizeof( struct tm ) ) ;  
      nanos_ = rhs.nanos_ ;
      return *this ;
    }

    //--------------------------------------------------------------------------------------
    inline 
    bool
    operator==( const Datetime & rhs ) const
    {
      return  year()       == rhs.year() 
           && month()      == rhs.month() 
           && month_day()  == rhs.month_day() 
           && hour()       == rhs.hour() 
           && minute()     == rhs.minute() 
           && second()     == rhs.second() 
           && nanosecond() == rhs.nanosecond() 
              ;
    }

    //--------------------------------------------------------------------------------------
    inline bool operator!=( const Datetime & rhs ) const { return !this->operator==( rhs ) ; }

    //--------------------------------------------------------------------------------------
    inline uint32_t nanosecond()  const { return nanos_ ; }
    inline uint32_t microsecond() const { return nanos_ / Nanos_Per_Micro ; }
    inline uint32_t millisecond() const { return nanos_ / Nanos_Per_Milli ; }
    inline uint32_t second()      const { return raw_.tm_sec; }
    inline uint32_t minute()      const { return raw_.tm_min; }
    inline uint32_t hour()        const { return raw_.tm_hour; }
    inline uint32_t month()       const { return raw_.tm_mon + 1 ; }
    inline uint32_t month_day()   const { return raw_.tm_mday; }
    inline uint32_t week_day()    const { return raw_.tm_wday; }
    inline uint32_t year_day()    const { return raw_.tm_yday; }
    inline uint32_t year()        const { return raw_.tm_year + 1900; }
    inline bool     is_dst()      const { return static_cast<bool>( raw_.tm_isdst ) ; } 

    //--------------------------------------------------------------------------------------
    inline void set_is_dst     (  int32_t val ) { raw_.tm_isdst = (val > 0) ? 1 : (val < 0) ? -1 : 0 ; } 
    inline void set_year       ( uint32_t val ) { raw_.tm_year = (val - 1900) ; }
    inline void set_year_day   ( uint32_t val ) { raw_.tm_yday = val ; }
    inline void set_yday       ( uint32_t val ) { set_year_day( val ) ; }
    inline void set_month      ( uint32_t val ) { raw_.tm_mon  = (val - 1); }
    inline void set_month_day  ( uint32_t val ) { raw_.tm_mday = val ; }
    inline void set_mday       ( uint32_t val ) { set_month_day( val ) ; }
    inline void set_week_day   ( uint32_t val ) { raw_.tm_wday = val ; }
    inline void set_wday       ( uint32_t val ) { set_week_day( val ) ; }
    inline void set_hour       ( uint32_t val ) { raw_.tm_hour = val ; }
    inline void set_minute     ( uint32_t val ) { raw_.tm_min  = val ; }
    inline void set_second     ( uint32_t val ) { raw_.tm_sec  = val ; }
    inline void set_millisecond( uint32_t val ) { nanos_ = val / time::Nanos_Per_Milli ; } 
    inline void set_microsecond( uint32_t val ) { nanos_ = val / time::Nanos_Per_Micro ; } 
    inline void set_nanosecond ( uint32_t val ) { nanos_ = val ; } 
    

    //--------------------------------------------------------------------------------------
    inline 
    void 
    clear() 
    { 
      std::memset( this, 0, sizeof( *this ) ) ;
      raw_.tm_isdst = -1 ;
    }

    //--------------------------------------------------------------------------------------
    inline 
    bool 
    empty() const 
    { 
      return (nanos_ == 0) 
          && (raw_.tm_year==0)
          && (raw_.tm_mday==0)
          && (raw_.tm_mon ==0)   
          && (raw_.tm_hour==0)   
          && (raw_.tm_min ==0)   
          && (raw_.tm_sec ==0)   
             ; 
    }

    //--------------------------------------------------------------------------------------
    inline const struct tm & as_tm_struct() const { return raw_ ; }
    inline struct tm       & as_tm_struct()       { return raw_ ; }

    //--------------------------------------------------------------------------------------
    template<time::Format F=time::DT_Nano> 
    std::string 
    to_string() const ;

    //--------------------------------------------------------------------------------------
    template<time::Format T> 
    Datetime &
    from_string( const char * str, uint32_t len ) ;

    //--------------------------------------------------------------------------------------
    template<time::Format T> 
    Datetime &
    from_string( const std::string & str ) 
    { 
      return from_string<T>( str.c_str(), str.length() ) ;
    }

    //--------------------------------------------------------------------------------------
    template<time::Format T> 
    Datetime &
    from_string( const char * str )
    { 
      return from_string<T>( str, (str ? std::strlen( str ) : 0 ) ) ;
    }

    //--------------------------------------------------------------------------------------
    inline 
    Datetime 
    midnight() const 
    {
      Datetime rv( *this ) ;
      rv.raw_.tm_sec  = 0;
      rv.raw_.tm_min  = 0;
      rv.raw_.tm_hour = 0;
      rv.nanos_       = 0 ;
      return rv ;
    }

    //--------------------------------------------------------------------------------------
    static Datetime now() ;
    static Datetime midnight( Timestamp ts ) ;
  } ;
}}

#endif

