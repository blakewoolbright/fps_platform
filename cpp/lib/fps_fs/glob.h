#ifndef FPS__FS__GLOB__H
#define FPS__FS__GLOB__H

#include "fps_fs/path.h"
#include <boost/iterator/iterator_facade.hpp>
#include <string>
#include <glob.h>

namespace fps {
namespace fs  {

  //----------------------------------------------------------------------------------------------------
  class Glob
  {
  private :
    std::string   expr_ ;
    glob_t        glob_ ;
    uint32_t      flags_ ;
    int32_t       error_id_ ;
  
    Glob( const Glob & rhs ) ;

  public :
    //---------------------------------------------------------------------------
    static const uint32_t Default_Flags = GLOB_MARK | GLOB_BRACE | GLOB_TILDE ;

    //---------------------------------------------------------------------------
    struct iterator
      : public boost::iterator_facade< iterator
                                     , const char *
                                     , boost::bidirectional_traversal_tag
                                     , const char *
                                     >
    {
    public :
      friend class boost::iterator_core_access ;
    
    private :
      char ** ptr_ ;

    public :
      //-------------------------------------------------------------------------
      inline          iterator() : ptr_( NULL ) {}
      inline explicit iterator( char ** ptr ) : ptr_( ptr ) {}

    private :
      //-------------------------------------------------------------------------
      inline void increment() { ++ptr_ ; }
      inline void decrement() { --ptr_ ; }

      //-------------------------------------------------------------------------
      inline 
      bool 
      equal( const iterator & rhs ) const 
      { 
        return ptr_==rhs.ptr_ ; 
      }

      //-------------------------------------------------------------------------
      inline 
      const char * 
      dereference() const 
      { 
        return *ptr_ ; 
      }
    } ;

  public :

    //------------------------------------------------------------------------------------
    Glob() ;
    explicit Glob( const std::string & expr ) ;
    explicit Glob( uint32_t flags ) ;
    explicit Glob( const std::string & expr, uint32_t flags ) ;
    ~Glob() ;

    //------------------------------------------------------------------------------------
    inline const std::string & expression() const { return expr_  ; }
    inline uint32_t            flags()      const { return flags_ ; }
    inline int32_t             error_id()   const { return error_id_ ; }

    //------------------------------------------------------------------------------------
    // Sync current glob configuration w/ file system (ie. load matches).
    //------------------------------------------------------------------------------------
    bool sync() ;
    
    //------------------------------------------------------------------------------------
    // Assign a new expression & flagset to this glob instance and sync().
    //------------------------------------------------------------------------------------
    bool assign( const std::string & expr, uint32_t flags ) ;

    //------------------------------------------------------------------------------------
    // Assign a new expression, but keep the existing flags.
    //------------------------------------------------------------------------------------
    inline bool assign( const std::string & expr ) { return assign( expr, flags_ ) ; }

    //------------------------------------------------------------------------------------
    // Assign new flags, but keep the existing expression.
    //------------------------------------------------------------------------------------
    inline bool assign( uint32_t new_flags ) { return assign( expr_, new_flags ) ; }

    //------------------------------------------------------------------------------------
    inline iterator begin() const { return iterator( glob_.gl_pathv ) ; }
    inline iterator end()   const { return iterator( glob_.gl_pathv + glob_.gl_pathc ) ; }

    //------------------------------------------------------------------------------------
    inline bool     valid()    const { return !expr_.empty() && glob_.gl_pathc!=0 ; }
    inline bool     empty()    const { return glob_.gl_pathc==0 ; }
    inline uint32_t size()     const { return glob_.gl_pathc ; }
  } ;

}}

#endif
