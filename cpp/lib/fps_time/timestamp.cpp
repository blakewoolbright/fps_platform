#include "fps_time/timestamp.h"
#include "fps_time/datetime.h"

namespace fps  {
namespace time {

  //----------------------------------------------------------------------------------------
  Timestamp::Timestamp( const Datetime & src ) 
    : value_( 0 ) 
  { 
    uint64_t epoch_secs = ::mktime( const_cast<struct tm*>( &(src.as_tm_struct()) ) ) ;
    value_ = (epoch_secs * time::Nanos_Per_Second) + src.nanosecond() ;
  }

  //----------------------------------------------------------------------------------------
  Timestamp 
  Timestamp::midnight() const  
  { 
    return Timestamp( Datetime::midnight( *this ) ) ;
  }

}}

