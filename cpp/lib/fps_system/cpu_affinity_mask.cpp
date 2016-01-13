#include <unistd.h>
#include "fps_system/cpu_affinity_mask.h"
#include "fps_string/format.h"

namespace fps {
namespace system {
namespace cpu {

  //----------------------------------------------------------------------
  std::string
  AffinityMask::to_string() const 
  {
    std::string rv ;
    uint32_t tgt_count = size() ; 
    uint32_t cur_count = 0 ;
    for( uint32_t idx = 0 
       ; idx < CPU_SETSIZE && cur_count < tgt_count 
       ; ++idx 
       ) 
    { if( !CPU_ISSET( idx, &mask_ ) ) 
        continue ;

      string::append( rv, "%u,", idx ) ;
      ++cur_count ;
    }

    if( !rv.empty() ) 
      rv.pop_back() ;
    
    return rv ;
  }

}}}
