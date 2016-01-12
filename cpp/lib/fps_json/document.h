#ifndef FPS__JSON__DOCUMENT__H
#define FPS__JSON__DOCUMENT__H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <type_traits>

#include "fps_json/value.h"

namespace fps  {
namespace json { 

  //----------------------------------------------------------------------------
  // TODO: Rethink this design... It feels forced.  The terminology is from 
  //       the jsoncpp library, and the implementation is boost property tree...
  //
  // TODO: Is boost::property_tree the best choice here?  I like it's design, 
  //       but the fact that it isn't strongly typed makes it unsuitable for
  //       the schema implementation being prototyped in the schema/ subdir.
  //----------------------------------------------------------------------------
  class Document 
  { 
  private :
    //--------------------------------------------------------------------------
    typedef detail::tree_t         tree_t ;
    typedef tree_t::iterator       iterator ;
    typedef tree_t::const_iterator const_iterator ;  

    //--------------------------------------------------------------------------
    tree_t              tree_ ;
    Value               root_ ;
    mutable std::string err_text_ ;

  public :
    //--------------------------------------------------------------------------
    Document() ; 
    ~Document() ;
  
    //--------------------------------------------------------------------------
    inline const std::string & error_msg() const { return err_text_ ; }
    inline bool                empty()     const { return root_.empty() ; }
  
    //--------------------------------------------------------------------------
    inline const Value & root() const { return root_ ; }
    inline Value       & root()       { return root_ ; }

    //--------------------------------------------------------------------------
    inline 
    Value 
    find( const std::string & key ) const 
    { return root_.find( key ) ;
    }

    //--------------------------------------------------------------------------
    std::string serialize() const ;
    bool        deserialize( const std::string & json_data ) ;

    //--------------------------------------------------------------------------
    bool load( const std::string & input_file ) ;
    bool save( const std::string & output_file ) const ;
  } ;
  
}}

#endif

