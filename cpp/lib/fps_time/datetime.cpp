#include "fps_time/datetime.h"
#include "fps_time/timestamp.h"

namespace fps  {
namespace time {

  //----------------------------------------------------------------------------------------
  Datetime::Datetime( const Timestamp & ts ) 
    : nanos_( 0 )
  {
    time_t epoch_secs( ts.epoch_seconds() ) ;
    ::localtime_r( &epoch_secs, &raw_ ) ;

    nanos_ = ts.epoch_nanos() - (ts.epoch_seconds() * time::Nanos_Per_Second) ;
  }

  //----------------------------------------------------------------------------------------
  Datetime::Datetime( uint64_t epoch_nanos )
    : nanos_( 0 ) 
  {
    time_t epoch_seconds = epoch_nanos / time::Nanos_Per_Second ;
    ::localtime_r( &epoch_seconds, &raw_ );
    nanos_ = epoch_nanos - (epoch_seconds * time::Nanos_Per_Second) ;
  }

  //--------------------------------------------------------------------------------------
  Datetime Datetime::now() { return Datetime( Timestamp::now() ) ; } 

  //--------------------------------------------------------------------------------------
  Datetime 
  Datetime::midnight( Timestamp ts ) 
  {
    Datetime rv( ts ) ;
    rv.raw_.tm_sec  = 0;
    rv.raw_.tm_min  = 0;
    rv.raw_.tm_hour = 0;
    rv.nanos_       = 0 ;
    return rv ;
  }

}}


