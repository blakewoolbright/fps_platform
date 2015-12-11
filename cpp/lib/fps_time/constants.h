#ifndef FPS__TIME__CONSTANTS__H
#define FPS__TIME__CONSTANTS__H

#include <stdint.h>

namespace fps  {
namespace time {

  //-----------------------------------------------------------------------------------
  static const uint64_t Nanos_Per_Micro   = 1000lu ;
  static const uint64_t Nanos_Per_Milli   = 1000000lu ;
  static const uint64_t Nanos_Per_Second  = 1000000000lu ;
  static const uint64_t Nanos_Per_Minute  = Nanos_Per_Second * 60 ;
  static const uint64_t Nanos_Per_Hour    = Nanos_Per_Minute * 60 ;
  static const uint64_t Nanos_Per_Day     = Nanos_Per_Hour   * 24 ;

  //-----------------------------------------------------------------------------------
  static const uint64_t Micros_Per_Milli  = 1000lu ;
  static const uint64_t Micros_Per_Second = 1000000lu ;
  static const uint64_t Micros_Per_Minute = Micros_Per_Second * 60 ;
  static const uint64_t Micros_Per_Hour   = Micros_Per_Minute * 60 ;
  static const uint64_t Micros_Per_Day    = Micros_Per_Hour   * 24 ;

  //-----------------------------------------------------------------------------------
  static const uint64_t Millis_Per_Second = 1000lu ;
  static const uint64_t Millis_Per_Minute = Millis_Per_Second * 60 ;
  static const uint64_t Millis_Per_Hour   = Millis_Per_Minute * 60 ;
  static const uint64_t Millis_Per_Day    = Millis_Per_Hour   * 24 ;

  //-----------------------------------------------------------------------------------
  enum Format 
  { TS_Nano             // HH:MM:SS.nnnnnnnnn
  , TS_Micro            // HH:MM:SS.uuuuuu
  , TS_Milli            // HH:MM:SS.mmm
  , TS_Second           // HH:MM:SS

  , DT_Nano             // YYYY-MM-DD HH:MM:SS.nnnnnnnnn
  , DT_Micro            // YYYY-MM-DD HH:MM:SS.uuuuuu
  , DT_Milli            // YYYY-MM-DD HH:MM:SS.mmm
  , DT_Second           // YYYY-MM-DD HH:MM:SS

  , HHMMSS_n
  , HHMMSS_u
  , HHMMSS_m
  , HHMMSS
  , YYYYMMDDHHMMSS_n
  , YYYYMMDDHHMMSS_u
  , YYYYMMDDHHMMSS_m
  , YYYYMMDDHHMMSS    
  , YYYYMMDD
    
  , Epoch_Nano
  , Epoch_Micro
  , Epoch_Milli
  , Epoch_Second
  } ;

}}

#endif

