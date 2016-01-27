#ifndef FPS__CONTAINER__OPTIONS__H
#define FPS__CONTAINER__OPTIONS__H

#include "fps_ntp/fps_ntp.h"

namespace fps {
namespace container {
namespace opt {

  //
  // Common named template parameters supported by most containers.
  //
  FPS_Declare_NTP_Value( Capacity,         uint32_t ) ;
  FPS_Declare_NTP_Value( Max_Capacity,     uint32_t ) ;
  FPS_Declare_NTP_Value( Default_Capacity, uint32_t ) ;
  FPS_Declare_NTP_Value( Reverse,          bool ) ;
  FPS_Declare_NTP_Value( Distinct,         bool ) ;
  FPS_Declare_NTP_Value( Destruct,         bool ) ;
  FPS_Declare_NTP_Value( Construct,        bool ) ;

}}}

#endif
