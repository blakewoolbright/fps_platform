#ifndef FPS__UTIL__BUILD_INFO__H
#define FPS__UTIL__BUILD_INFO__H

#include "fps_string/fps_string.h"
#include "fps_json/fps_json.h"

namespace fps  {
namespace util {
  
  //-----------------------------------------------------------------
  class BuildInfo
  { 
  public :
    #if defined( FPS__ENABLE_BUILD_INFO ) 
    static const bool Is_Available = true ;
    #else
    static const bool Is_Available = false ;
    #endif

  private :
    json::Document  doc_  ;
    std::string     data_ ;

    std::string     tag_ ;
    std::string     timestamp_ ;
    std::string     user_ ;
    std::string     host_ ;
    std::string     type_ ;
    std::string     linkage_ ;
    std::string     compiler_ ;

    //--------------------------------------------------------------
    bool load() ;

  public :
    //--------------------------------------------------------------
    BuildInfo()  ;
    ~BuildInfo() ;

    //--------------------------------------------------------------
    inline bool empty() const { return doc_.empty() ; }
    inline bool valid() const { return !doc_.empty() && !tag_.empty() ; }

    //--------------------------------------------------------------
    inline const std::string & to_json()   const { return data_ ; }

    //--------------------------------------------------------------
    inline const std::string & tag()       const { return tag_ ; }
    inline const std::string & timestamp() const { return timestamp_ ; }
    inline const std::string & user()      const { return user_ ; }
    inline const std::string & host()      const { return host_ ; }
    inline const std::string & type()      const { return type_ ; }
    inline const std::string & linkage()   const { return linkage_ ; }
    inline const std::string & compiler()  const { return compiler_ ; }
  } ;
}}

#endif
