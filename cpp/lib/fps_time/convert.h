#ifndef FPS__TIME__CONVERT__H
#define FPS__TIME__CONVERT__H

#include "datetime.h"
#include "timestamp.h"
#include "fps_string/fps_string.h"

#define A2D( char_val ) (char_val-'0')

namespace fps {
namespace time {

  //------------------------------------------------------------------------------
  template< time::Format fmt> struct Convert  ;

  //------------------------------------------------------------------------------
  template<>
  struct Convert<TS_Nano>
  { 
    //----------------------------------------------------------------------------
    static
    inline
    std::string
    to_string( const time::Datetime & dtm )
    { 
      return string::sprintf
             ( "%02u:%02u:%02u.%09u"
             , dtm.hour()
             , dtm.minute()
             , dtm.second()
             , dtm.nanosecond() 
             ) ;
    }

    //----------------------------------------------------------------------------
    static
    inline 
    bool
    from_string( Datetime & rv, const char * str, uint32_t len )  
    { 
      if( !str || len < 18 ) 
        return false ;

      rv.set_hour       ( A2D(str[0]) * 10 + A2D(str[1]) ) ; 
      rv.set_minute     ( A2D(str[3]) * 10 + A2D(str[4]) ) ; 
      rv.set_second     ( A2D(str[6]) * 10 + A2D(str[7]) ) ; 
      rv.set_nanosecond
      ( (A2D(str[9])  * 100000000) + (A2D(str[10]) * 10000000) + (A2D(str[11]) * 1000000)
      + (A2D(str[12]) * 100000)    + (A2D(str[13]) * 10000)    + (A2D(str[14])* 1000)
      + (A2D(str[15]) * 100)       + (A2D(str[16]) * 10)       + A2D(str[17])
      ) ; 

      return true ;
    }
  } ;

  //------------------------------------------------------------------------------
  template<>
  struct Convert<TS_Micro>
  { static
    inline
    std::string
    to_string( const time::Datetime & dtm )
    { 
      return string::sprintf
             ( "%02u:%02u:%02u.%06u"
             , dtm.hour()
             , dtm.minute()
             , dtm.second()
             , dtm.microsecond() 
             ) ;
    }

    //----------------------------------------------------------------------------
    static
    inline 
    bool
    from_string( Datetime & rv, const char * str, uint32_t len )  
    { 
      if( !str || len < 15 ) 
        return false ;

      rv.set_hour       ( A2D(str[0]) * 10 + A2D(str[1]) ) ; 
      rv.set_minute     ( A2D(str[3]) * 10 + A2D(str[4]) ) ; 
      rv.set_second     ( A2D(str[6]) * 10 + A2D(str[7]) ) ; 
      rv.set_microsecond
      ( (A2D(str[9]) * 100000) + (A2D(str[10]) * 10000) + (A2D(str[11]) * 1000)
      + (A2D(str[12]) * 100)   + (A2D(str[13]) * 10)    + A2D(str[14])
      ) ; 

      return true ;
    }
  } ;

  //------------------------------------------------------------------------------
  template<>
  struct Convert<TS_Milli>
  { 
    //----------------------------------------------------------------------------
    static
    inline
    std::string
    to_string( const time::Datetime & dtm )
    { 
      return string::sprintf
             ( "%02u:%02u:%02u.%03u"
             , dtm.hour()
             , dtm.minute()
             , dtm.second()
             , dtm.millisecond() 
             ) ;
    }

    //----------------------------------------------------------------------------
    static
    inline 
    bool
    from_string( Datetime & rv, const char * str, uint32_t len )  
    { 
      if( !str || len < 12 ) 
        return false ;

      rv.set_hour       ( A2D(str[0]) * 10 + A2D(str[1]) ) ; 
      rv.set_minute     ( A2D(str[3]) * 10 + A2D(str[4]) ) ; 
      rv.set_second     ( A2D(str[6]) * 10 + A2D(str[7]) ) ; 
      rv.set_millisecond( (A2D(str[9]) * 100) + (A2D(str[10]) * 10) + A2D(str[11]) ) ;
      return true ;
    }
  } ;

  //------------------------------------------------------------------------------
  template<>
  struct Convert<TS_Second>
  { 
    //----------------------------------------------------------------------------
    static
    inline
    std::string
    to_string( const time::Datetime & dtm )
    { 
      return string::sprintf
             ( "%02u:%02u:%02u"
             , dtm.hour()
             , dtm.minute()
             , dtm.second()
             ) ;
    }

    //----------------------------------------------------------------------------
    static
    inline 
    bool
    from_string( Datetime & rv, const char * str, uint32_t len )  
    { 
      if( !str || len < 8 ) 
        return false ;

      rv.set_hour  ( A2D(str[0]) * 10 + A2D(str[1]) ) ; 
      rv.set_minute( A2D(str[3]) * 10 + A2D(str[4]) ) ; 
      rv.set_second( A2D(str[6]) * 10 + A2D(str[7]) ) ; 
      return true ;
    }
  } ;

  //------------------------------------------------------------------------------
  template<>
  struct Convert<DT_Nano>
  { 
    //----------------------------------------------------------------------------
    static
    inline
    std::string
    to_string( const time::Datetime & dtm )
    { 
      return string::sprintf( "%04u-%02u-%02u %02u:%02u:%02u.%09u"
                            , dtm.year()
                            , dtm.month()
                            , dtm.month_day()
                            , dtm.hour()
                            , dtm.minute()
                            , dtm.second()
                            , dtm.nanosecond() 
                            ) ;
    }

    //----------------------------------------------------------------------------
    static
    inline 
    bool     
    from_string( Datetime & rv, const char * str, uint32_t len )  
    { 
      if( !str || len < 29 ) 
        return false ;

      rv.set_year  ( A2D(str[0])*1000 + A2D(str[1])*100 + A2D(str[2])*10 + A2D(str[3]) ) ;
      rv.set_month ( A2D(str[5])  * 10 + A2D(str[6]) ) ;
      rv.set_mday  ( A2D(str[8])  * 10 + A2D(str[9]) ) ;
      rv.set_hour  ( A2D(str[11]) * 10 + A2D(str[12]) ) ; 
      rv.set_minute( A2D(str[14]) * 10 + A2D(str[15]) ) ; 
      rv.set_second( A2D(str[17]) * 10 + A2D(str[18]) ) ; 
      rv.set_nanosecond( (A2D(str[20]) * 100000000) 
                       + (A2D(str[21]) * 10000000) 
                       + (A2D(str[22]) * 1000000) 
                       + (A2D(str[23]) * 100000) 
                       + (A2D(str[24]) * 10000) 
                       + (A2D(str[25]) * 1000) 
                       + (A2D(str[26]) * 100) 
                       + (A2D(str[27]) * 10) 
                       +  A2D(str[28]) 
                       ) ; 
      return true ;
    }
  } ;

  //------------------------------------------------------------------------------
  template<>
  struct Convert<DT_Micro>
  { 
    //----------------------------------------------------------------------------
    static
    inline
    std::string
    to_string( const time::Datetime & dtm )
    { 
      return string::sprintf( "%04u-%02u-%02u %02u:%02u:%02u.%06u"
                            , dtm.year()
                            , dtm.month()
                            , dtm.month_day()
                            , dtm.hour()
                            , dtm.minute()
                            , dtm.second()
                            , dtm.microsecond() 
                            ) ;
    }

    //----------------------------------------------------------------------------
    static
    inline 
    bool     
    from_string( Datetime & rv, const char * str, uint32_t len )  
    { 
      if( !str || len < 26 ) 
        return false ;

      rv.set_year  ( A2D(str[0])*1000 + A2D(str[1])*100 + A2D(str[2])*10 + A2D(str[3]) ) ;
      rv.set_month ( A2D(str[5])  * 10 + A2D(str[6]) ) ;
      rv.set_mday  ( A2D(str[8])  * 10 + A2D(str[9]) ) ;
      rv.set_hour  ( A2D(str[11]) * 10 + A2D(str[12]) ) ; 
      rv.set_minute( A2D(str[14]) * 10 + A2D(str[15]) ) ; 
      rv.set_second( A2D(str[17]) * 10 + A2D(str[18]) ) ; 
      rv.set_microsecond( (A2D(str[20]) * 100000) 
                        + (A2D(str[21]) * 10000) 
                        + (A2D(str[22]) * 1000) 
                        + (A2D(str[23]) * 100) 
                        + (A2D(str[24]) * 10) 
                        +  A2D(str[25]) 
                        ) ; 
      return true ;
    }
  } ;

  //------------------------------------------------------------------------------
  template<>
  struct Convert<DT_Milli>
  { 
    //----------------------------------------------------------------------------
    static
    inline
    std::string
    to_string( const time::Datetime & dtm )
    { 
      return string::sprintf( "%04u-%02u-%02u %02u:%02u:%02u.%03u"
                            , dtm.year()
                            , dtm.month()
                            , dtm.month_day()
                            , dtm.hour()
                            , dtm.minute()
                            , dtm.second()
                            , dtm.millisecond() 
                            ) ;
    }

    //----------------------------------------------------------------------------
    static
    inline 
    bool 
    from_string( Datetime & rv, const char * str, uint32_t len )  
    { 
      if( !str || len < 23 ) 
        return false ;

      rv.set_year  ( A2D(str[0])*1000 + A2D(str[1])*100 + A2D(str[2])*10 + A2D(str[3]) ) ;
      rv.set_month ( A2D(str[5])  * 10 + A2D(str[6]) ) ;
      rv.set_mday  ( A2D(str[8])  * 10 + A2D(str[9]) ) ;
      rv.set_hour  ( A2D(str[11]) * 10 + A2D(str[12]) ) ; 
      rv.set_minute( A2D(str[14]) * 10 + A2D(str[15]) ) ; 
      rv.set_second( A2D(str[17]) * 10 + A2D(str[18]) ) ; 
      rv.set_millisecond( A2D(str[20]) * 100 + A2D(str[21]) * 10 + A2D(str[22]) ) ; 

      return true ;
    }
  } ;

  //------------------------------------------------------------------------------
  template<>
  struct Convert<DT_Second>
  { 
    //----------------------------------------------------------------------------
    static
    inline
    std::string
    to_string( const time::Datetime & dtm )
    { 
      return string::sprintf( "%04u-%02u-%02u %02u:%02u:%02u"
                            , dtm.year()
                            , dtm.month()
                            , dtm.month_day()
                            , dtm.hour()
                            , dtm.minute()
                            , dtm.second()
                            ) ;
    }

    //----------------------------------------------------------------------------
    static
    inline 
    bool 
    from_string( Datetime & rv, const char * str, uint32_t len )  
    { 
      if( !str || len < 19 ) 
        return false ;

      rv.set_year  ( A2D(str[0])*1000 + A2D(str[1])*100 + A2D(str[2])*10 + A2D(str[3]) ) ;
      rv.set_month ( A2D(str[5])  * 10 + A2D(str[6]) ) ;
      rv.set_mday  ( A2D(str[8])  * 10 + A2D(str[9]) ) ;
      rv.set_hour  ( A2D(str[11]) * 10 + A2D(str[12]) ) ; 
      rv.set_minute( A2D(str[14]) * 10 + A2D(str[15]) ) ; 
      rv.set_second( A2D(str[17]) * 10 + A2D(str[18]) ) ; 
       
      return true ;
    }
  } ;

  //------------------------------------------------------------------------------
  template<>
  struct Convert<HHMMSS_n>
  { 
    //----------------------------------------------------------------------------
    static
    inline
    std::string
    to_string( const time::Datetime & dtm )
    { 
      return string::sprintf( "%02u%02u%02u%09u"
                            , dtm.hour()
                            , dtm.minute()
                            , dtm.second()
                            , dtm.nanosecond()
                            ) ;
    }

    //----------------------------------------------------------------------------
    static
    inline 
    bool 
    from_string( Datetime & rv, const char * str, uint32_t len )  
    { 
      if( !str || len < 15 ) 
        return false ;
      
      rv.set_hour       ( A2D(str[0]) * 10 + A2D(str[1]) ) ; 
      rv.set_minute     ( A2D(str[2]) * 10 + A2D(str[3]) ) ; 
      rv.set_second     ( A2D(str[4]) * 10 + A2D(str[5]) ) ; 
      rv.set_nanosecond
      ( (A2D(str[6])  * 100000000) + (A2D(str[7])  * 10000000) + (A2D(str[8]) * 1000000)
      + (A2D(str[9])  * 100000)    + (A2D(str[10]) * 10000)    + (A2D(str[11])* 1000)
      + (A2D(str[12]) * 100)       + (A2D(str[13]) * 10)       + A2D(str[14])
      ) ; 

      return true ;
    }
  } ;

  //------------------------------------------------------------------------------
  template<>
  struct Convert<HHMMSS_u>
  { 
    //----------------------------------------------------------------------------
    static
    inline
    std::string
    to_string( const time::Datetime & dtm )
    { 
      return string::sprintf( "%02u%02u%02u%06u"
                            , dtm.hour()
                            , dtm.minute()
                            , dtm.second()
                            , dtm.microsecond()
                            ) ;
    }

    //----------------------------------------------------------------------------
    static
    inline 
    bool 
    from_string( Datetime & rv, const char * str, uint32_t len )  
    { 
      if( !str || len < 12 ) 
        return false ;
      rv.set_hour       ( A2D(str[0]) * 10 + A2D(str[1]) ) ; 
      rv.set_minute     ( A2D(str[2]) * 10 + A2D(str[3]) ) ; 
      rv.set_second     ( A2D(str[4]) * 10 + A2D(str[5]) ) ; 
      rv.set_microsecond
      ( (A2D(str[6]) * 100000) + (A2D(str[7]) * 10000) + (A2D(str[8]) * 1000)
      + (A2D(str[9]) * 100)    + (A2D(str[10]) * 10)    + A2D(str[11])
      ) ; 

      return true ;
    }
  } ;

  //------------------------------------------------------------------------------
  template<>
  struct Convert<HHMMSS_m>
  { 
    //----------------------------------------------------------------------------
    static
    inline
    std::string
    to_string( const time::Datetime & dtm )
    { 
      return string::sprintf( "%02u%02u%02u%03u"
                            , dtm.hour()
                            , dtm.minute()
                            , dtm.second()
                            , dtm.millisecond()
                            ) ;
    }

    //----------------------------------------------------------------------------
    static
    inline 
    bool 
    from_string( Datetime & rv, const char * str, uint32_t len )  
    { 
      if( !str || len < 9 ) 
        return false ;

      rv.set_hour       ( A2D(str[0]) * 10 + A2D(str[1]) ) ; 
      rv.set_minute     ( A2D(str[2]) * 10 + A2D(str[3]) ) ; 
      rv.set_second     ( A2D(str[4]) * 10 + A2D(str[5]) ) ; 
      rv.set_millisecond( (A2D(str[6]) * 100) + (A2D(str[7]) * 10) + A2D(str[8]) ) ;

      return true ;
    }
  } ;

  //------------------------------------------------------------------------------
  template<>
  struct Convert<HHMMSS>
  { 
    //----------------------------------------------------------------------------
    static
    inline
    std::string
    to_string( const time::Datetime & dtm )
    { 
      return string::sprintf( "%02u%02u%02u"
                            , dtm.hour()
                            , dtm.minute()
                            , dtm.second()
                            ) ;
    }

    //----------------------------------------------------------------------------
    static
    inline 
    bool 
    from_string( Datetime & rv, const char * str, uint32_t len )  
    { 
      if( !str || len < 6 ) 
        return false ;

      rv.set_hour  ( A2D(str[0]) * 10 + A2D(str[1]) ) ; 
      rv.set_minute( A2D(str[2]) * 10 + A2D(str[3]) ) ; 
      rv.set_second( A2D(str[4]) * 10 + A2D(str[5]) ) ; 
      
      return true ;
    }
  } ;

  //------------------------------------------------------------------------------
  template<>
  struct Convert<YYYYMMDDHHMMSS_n>
  { 
    //----------------------------------------------------------------------------
    static
    inline
    std::string
    to_string( const time::Datetime & dtm )
    { 
      return string::sprintf( "%04u%02u%02u%02u%02u%02u%09u"
                            , dtm.year()
                            , dtm.month()
                            , dtm.month_day()
                            , dtm.hour()
                            , dtm.minute()
                            , dtm.second()
                            , dtm.nanosecond()
                            ) ;
    }

    //----------------------------------------------------------------------------
    static
    inline 
    bool 
    from_string( Datetime & rv, const char * str, uint32_t len )  
    { 
      if( !str || len < 23 ) 
        return false ;

      rv.set_year  ( A2D(str[0])*1000 + A2D(str[1])*100 + A2D(str[2])*10 + A2D(str[3]) ) ;
      rv.set_month ( A2D(str[4]) *10   + A2D(str[5]) ) ;
      rv.set_mday  ( A2D(str[6]) *10  + A2D(str[7]) ) ;
      rv.set_hour  ( A2D(str[8]) *10  + A2D(str[9]) ) ; 
      rv.set_minute( A2D(str[10]) *10 + A2D(str[11]) ) ; 
      rv.set_second( A2D(str[12]) *10 + A2D(str[13]) ) ; 
      rv.set_microsecond
      ( (A2D(str[14]) * 100000000) + (A2D(str[15]) * 10000000) + (A2D(str[16]) * 1000000)
      + (A2D(str[17]) * 100000)    + (A2D(str[18]) * 10000)    + (A2D(str[19]) * 1000)
      + (A2D(str[20]) * 100)       + (A2D(str[21]) * 10)       + A2D(str[22]) 
      ) ;

      return true ;
    }
  } ;

  //------------------------------------------------------------------------------
  template<>
  struct Convert<YYYYMMDDHHMMSS_u>
  { 
    //----------------------------------------------------------------------------
    static
    inline
    std::string
    to_string( const time::Datetime & dtm )
    { 
      return string::sprintf( "%04u%02u%02u%02u%02u%02u%06u"
                            , dtm.year()
                            , dtm.month()
                            , dtm.month_day()
                            , dtm.hour()
                            , dtm.minute()
                            , dtm.second()
                            , dtm.microsecond()
                            ) ;
    }

    //----------------------------------------------------------------------------
    static
    inline 
    bool 
    from_string( Datetime & rv, const char * str, uint32_t len )  
    { 
      if( !str || len < 20 ) 
        return false ;

      rv.set_year  ( A2D(str[0])*1000 + A2D(str[1])*100 + A2D(str[2])*10 + A2D(str[3]) ) ;
      rv.set_month ( A2D(str[4]) *10   + A2D(str[5]) ) ;
      rv.set_mday  ( A2D(str[6]) *10  + A2D(str[7]) ) ;
      rv.set_hour  ( A2D(str[8]) *10  + A2D(str[9]) ) ; 
      rv.set_minute( A2D(str[10]) *10 + A2D(str[11]) ) ; 
      rv.set_second( A2D(str[12]) *10 + A2D(str[13]) ) ; 
      rv.set_microsecond
      ( (A2D(str[14]) * 100000) + (A2D(str[15]) * 10000) + (A2D(str[16]) * 1000)
      + (A2D(str[17]) * 100)    + (A2D(str[18]) * 10)    + A2D(str[19]) 
      ) ;

      return true ;
    }
  } ;

  //------------------------------------------------------------------------------
  template<>
  struct Convert<YYYYMMDDHHMMSS_m>
  { 
    //----------------------------------------------------------------------------
    static
    inline
    std::string
    to_string( const time::Datetime & dtm )
    { 
      return string::sprintf( "%04u%02u%02u%02u%02u%02u%03u"
                            , dtm.year()
                            , dtm.month()
                            , dtm.month_day()
                            , dtm.hour()
                            , dtm.minute()
                            , dtm.second()
                            , dtm.millisecond()
                            ) ;
    }

    //----------------------------------------------------------------------------
    static
    inline 
    bool 
    from_string( Datetime & rv, const char * str, uint32_t len )  
    { 
      if( !str || len < 17 ) 
        return false ;

      rv.set_year  ( A2D(str[0])*1000 + A2D(str[1])*100 + A2D(str[2])*10 + A2D(str[3]) ) ;
      rv.set_month ( A2D(str[4]) *10   + A2D(str[5]) ) ;
      rv.set_mday  ( A2D(str[6]) *10  + A2D(str[7]) ) ;
      rv.set_hour  ( A2D(str[8]) *10  + A2D(str[9]) ) ; 
      rv.set_minute( A2D(str[10]) *10 + A2D(str[11]) ) ; 
      rv.set_second( A2D(str[12]) *10 + A2D(str[13]) ) ; 
      rv.set_millisecond( (A2D(str[14]) * 100) + (A2D(str[15]) * 10) + A2D(str[16]) ) ;
      
      return true ;
    }
  } ;

  //------------------------------------------------------------------------------
  template<>
  struct Convert<YYYYMMDDHHMMSS>
  { 
    //----------------------------------------------------------------------------
    static
    inline
    std::string
    to_string( const time::Datetime & dtm )
    { 
      return string::sprintf( "%04u%02u%02u%02u%02u%02u"
                            , dtm.year()
                            , dtm.month()
                            , dtm.month_day()
                            , dtm.hour()
                            , dtm.minute()
                            , dtm.second()
                            ) ;
    }
  
    //----------------------------------------------------------------------------
    static
    inline 
    bool 
    from_string( Datetime & rv, const char * str, uint32_t len )  
    { 
      if( !str || len < 14 ) 
        return false ;

      rv.set_year  ( A2D(str[0])*1000 + A2D(str[1])*100 + A2D(str[2])*10 + A2D(str[3]) ) ;
      rv.set_month ( A2D(str[4]) *10   + A2D(str[5]) ) ;
      rv.set_mday  ( A2D(str[6]) *10  + A2D(str[7]) ) ;
      rv.set_hour  ( A2D(str[8]) *10  + A2D(str[9]) ) ; 
      rv.set_minute( A2D(str[10]) *10 + A2D(str[11]) ) ; 
      rv.set_second( A2D(str[12]) *10 + A2D(str[13]) ) ; 
      return true ;
    }
  } ;

  //--------------------------------------------------------------------------------------
  // Datetime::to_string() needs to be defined after all conversion operations.
  //--------------------------------------------------------------------------------------
  template<Format F> 
  std::string 
  Datetime::to_string() const
  { 
    return Convert<F>::to_string( *this ) ;
  }

  //--------------------------------------------------------------------------------------
  template<Format F> 
  Datetime &
  Datetime::from_string( const char * buf, uint32_t len ) 
  { 
    this->clear() ;
    Convert<F>::from_string( *this, buf, len ) ;
    return *this ;
  }

  //--------------------------------------------------------------------------------------
  // Datetime::to_string() needs to be defined after all conversion operations.
  //--------------------------------------------------------------------------------------
  template<Format F> 
  std::string 
  Timestamp::to_string() const
  { 
    return Convert<F>::to_string( Datetime( *this ) ) ;
  }
}}

#undef A2D 
#endif
