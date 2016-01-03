#ifndef FPS__IPC__MAPPED_MEMORY__H
#define FPS__IPC__MAPPED_MEMORY__H

#include <errno.h>
#include <unistd.h>
#include <stdint.h>

namespace fps {
namespace ipc { 

  //-------------------------------------------------------------------------------------
  struct MappedMemory
  {
  private :
    //-----------------------------------------------------------------------------------
    void       * begin_ ;
    std::size_t  size_  ;

  public :
    //-----------------------------------------------------------------------------------
    inline
    MappedMemory() 
      : begin_( NULL ) 
      , size_ ( 0 )
    {}
      
    //-----------------------------------------------------------------------------------
    inline
    MappedMemory( void * begin, std::size_t size ) 
      : begin_( begin )
      , size_ ( size )
    {}

    //-----------------------------------------------------------------------------------
    inline
    MappedMemory( const MappedMemory & rhs ) 
      : begin_( rhs.begin_ ) 
      , size_ ( rhs.size_  )
    {}

    //-----------------------------------------------------------------------------------
    inline
    MappedMemory &
    operator=( const MappedMemory & rhs ) 
    { begin_ = rhs.begin_ ;
      size_  = rhs.size_  ;
      return *this ;
    }

    //-----------------------------------------------------------------------------------
    inline 
    void
    clear() 
    { begin_ = NULL ;
      size_  = 0 ;
    }

    //-----------------------------------------------------------------------------------
    inline bool         empty() const { return begin_ == NULL || size_ == 0 ; }
    inline const void * begin() const { return begin_ ; }
    inline std::size_t  size()  const { return size_ ; }

    //-----------------------------------------------------------------------------------
    template<typename T>
    T *  
    construct() 
    { 
      return ( empty() ) 
             ? NULL 
             : new ( begin_ ) T() 
             ;
    }

    //-----------------------------------------------------------------------------------
    template<typename T, typename... Args>
    T *  
    construct( Args &&... args ) 
    { 
      return ( empty() ) 
             ? NULL 
             : new ( begin_ ) T( std::forward<Args>( args )... ) 
             ;
    }
  } ;

}}

#endif
