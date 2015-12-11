#ifndef FPS__JSON__VALUE__H
#define FPS__JSON__VALUE__H

#include "fps_json/detail/iterator.h"

namespace fps  {
namespace json { 

  //----------------------------------------------------------------------------------------------------
  class Value 
  {
  private :
    typedef detail::tree_t tree_t ;

    std::string      name_ ;
    mutable tree_t * node_ ;

  public  :
    //--------------------------------------------------------------------------------------------------
    typedef detail::iterator_base<json::Value>       iterator ;
    typedef detail::iterator_base<json::Value const> const_iterator ;

    //--------------------------------------------------------------------------------------------------
    inline Value() : node_( NULL ) {}

    //--------------------------------------------------------------------------------------------------
    inline explicit Value( tree_t & root ) : node_( &root ) {}

    //--------------------------------------------------------------------------------------------------
    inline 
    Value( const std::string & name, tree_t & root ) 
      : name_( name ) 
      , node_( &root ) 
    {}

    //--------------------------------------------------------------------------------------------------
    inline 
    Value( const std::string & name, const tree_t & root ) 
      : name_( name ) 
      , node_( const_cast<tree_t *>( &root ) ) 
    {}

    //--------------------------------------------------------------------------------------------------
    inline 
    Value( const Value & rhs ) 
      : name_( rhs.name_ ) 
      , node_( rhs.node_ ) 
    {}

    //--------------------------------------------------------------------------------------------------
    inline std::string name() const { return name_         ; }
    inline std::string data() const { return node_->data() ; }

    //--------------------------------------------------------------------------------------------------
    inline iterator begin() { return iterator( node_->begin() ) ; }
    inline iterator end  () { return iterator( node_->end  () ) ; }

    //--------------------------------------------------------------------------------------------------
    inline const_iterator begin() const { return const_iterator( const_cast<tree_t *>( node_ )->begin() ) ; }
    inline const_iterator end  () const { return const_iterator( const_cast<tree_t *>( node_ )->end() ) ; }

    //--------------------------------------------------------------------------------------------------
    inline uint32_t size()    const { return node_ ? node_->size()  : 0 ; }
    inline bool     empty()   const { return node_ && node_->empty() && node_->data().empty() ; }
    inline bool     valid()   const { return node_ != NULL ; }
    inline bool     is_leaf() const { return node_ ? !node_->empty() : false ; }

    //--------------------------------------------------------------------------------------------------
    // Return the number children sharing the name identified by 'key'.
    //--------------------------------------------------------------------------------------------------
    inline
    std::size_t
    count_children( const std::string & key ) 
    { return node_->count( key ) ;
    }

    //--------------------------------------------------------------------------------------------------
    inline
    Value & 
    operator=( const Value & rhs ) 
    { 
      name_ = rhs.name_ ;
      node_ = rhs.node_ ;
      return *this ;
    }

    //--------------------------------------------------------------------------------------------------
    // TODO: Are these even a good idea?
    //--------------------------------------------------------------------------------------------------
    /*
    inline bool operator==( const Value & rhs ) const ;
    inline bool operator!=( const Value & rhs ) const ;
    */

    //--------------------------------------------------------------------------------------------------
    inline
    void 
    clear() 
    {
      name_.clear() ;
      node_ = NULL ;
    }

    //--------------------------------------------------------------------------------------------------
    inline
    Value &  
    assign( const std::string & name, tree_t & root ) 
    { 
      name_ = name ;
      node_ = &root ;
      return *this ; 
    }

    //--------------------------------------------------------------------------------------------------
    inline
    Value &  
    assign( const std::string & name, const tree_t & root ) 
    { 
      name_ = name ;
      node_ = &const_cast<tree_t &>( root ) ;
      return *this ; 
    }

    //--------------------------------------------------------------------------------------------------
    // Find a child node by name
    //--------------------------------------------------------------------------------------------------
    Value find( const std::string & key ) const ;

    //--------------------------------------------------------------------------------------------------
    // Serialize this value instance to a JSON encoded string
    //--------------------------------------------------------------------------------------------------
    std::string serialize() const ;

    //--------------------------------------------------------------------------------------------------
    // Deserialize a JSON encoded string into this value instance.
    //--------------------------------------------------------------------------------------------------
    void deserialize( const std::string & json_input ) ;
  
    //--------------------------------------------------------------------------------------------------
    // Fetch current value of node converted to the template argument type.
    //--------------------------------------------------------------------------------------------------
    template<typename T = std::string>
    T
    to() const
    { 
      return node_->get_value<T>() ;
    }

    //--------------------------------------------------------------------------------------------------
    template<typename T = std::string>
    T 
    get( const std::string & key, const T & default_value ) const
    { 
      return node_->get<T>( key, default_value ) ;
    }

    //--------------------------------------------------------------------------------------------------
    template<typename T>
    bool
    set( const T & new_value )
    { 
      try 
      { node_->put_value<T>( new_value ) ;
        return true ;
      }
      catch( const std::runtime_error & exc ) 
      { return false ;
      }
    }

    //--------------------------------------------------------------------------------------------------
    template<typename T>
    bool 
    set( const std::string & key, const T & value ) 
    { 
      try 
      { node_->put<T>( key, value ) ; 
        return true ;
      }
      catch( const std::exception & exc ) 
      { return false ;
      }
    }

    //--------------------------------------------------------------------------------------------------
    bool 
    set( const std::string & key, const char * value ) 
    { return set( key, std::string( value ) ) ;
    }

  } ;
}}

#endif
