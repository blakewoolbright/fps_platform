#include "build_info.h"
#include <sstream>

#if defined( FPS__ENABLE_BUILD_INFO ) 
extern uint64_t _binary_fps_build_info_size ;
extern uint64_t _binary_fps_build_info_start ;
#endif 

namespace fps  {  
namespace util {

  //-----------------------------------------------------------------
  BuildInfo::BuildInfo() 
  {
    if( Is_Available ) 
      load() ;
  }

  //-----------------------------------------------------------------
  BuildInfo::~BuildInfo() {}

  //-----------------------------------------------------------------
  bool 
  BuildInfo::load() 
  {
    const char * bi_begin = NULL ;
    uint64_t     bi_size = 0 ;
    data_.clear() ;

    #if defined( FPS__ENABLE_BUILD_INFO ) 
    bi_size  = (uint64_t)    &_binary_fps_build_info_size ;
    bi_begin = (const char *)&_binary_fps_build_info_start ;
    #endif

    if( bi_begin != NULL && bi_size > 0 ) 
      data_.assign( bi_begin, bi_size ) ;

    if( !doc_.deserialize( data_ ) ) 
      return false ;

    json::Value node = doc_.find( "build_info.tag" ) ;
    if( !node.empty() ) 
      tag_ = node.data() ;
  
    node = doc_.find( "build_info.timestamp" ) ;
    if( !node.empty() ) 
      timestamp_ = node.data() ;

    node = doc_.find( "build_info.user" ) ;
    if( !node.empty() ) 
      user_ = node.data() ;

    node = doc_.find( "build_info.type" ) ;
    if( !node.empty() ) 
      type_ = node.data() ;

    node = doc_.find( "build_info.host" ) ;
    if( !node.empty() ) 
      host_ = node.data() ;

    node = doc_.find( "build_info.compiler" ) ;
    if( !node.empty() ) 
      compiler_= node.data() ;

    node = doc_.find( "build_info.linkage" ) ;
    if( !node.empty() ) 
      linkage_ = node.data() ;

    return !tag_.empty() ;
  }
}}

