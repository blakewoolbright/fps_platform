#ifndef FPS__IPC__MAPPED_MEMORY__H
#define FPS__IPC__MAPPED_MEMORY__H

#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#include "fps_ipc/shared_memory.h"
#include "fps_ipc/ipc_util.h"
#include "fps_except/fps_except.h"

namespace fps {
namespace ipc { 

  //-------------------------------------------------------------------------------------
  struct MappedMemory
  {
  private :
    //-------------------------------------------------------------------------------------------
    void       * begin_ ;
    std::size_t  size_  ;
    std::size_t  offset_ ;

    //-------------------------------------------------------------------------------------------
    static
    void * 
    map_memory( int32_t fd, uint32_t flags, uint32_t offset, uint32_t size ) ;

    //-------------------------------------------------------------------------------------------
    static 
    bool
    unmap_memory( void * ptr, uint32_t size ) ;

  public :

    //-------------------------------------------------------------------------------------------
    MappedMemory( const SharedMemory & shm, uint32_t flags ) ;

    //-------------------------------------------------------------------------------------------
    MappedMemory( const SharedMemory & shm
                , uint32_t             flags
                , uint32_t             size
                , uint32_t             offset
                ) ;

    //-------------------------------------------------------------------------------------------
    inline
    MappedMemory( const MappedMemory & rhs ) 
      : begin_ ( rhs.begin_ ) 
      , size_  ( rhs.size_  )
      , offset_( rhs.offset_ )
    {}

    //-------------------------------------------------------------------------------------------
    inline ~MappedMemory() {}

    //-------------------------------------------------------------------------------------------
    inline
    MappedMemory &
    operator=( const MappedMemory & rhs ) 
    { 
      begin_  = rhs.begin_ ;
      size_   = rhs.size_  ;
      offset_ = rhs.offset_ ;
      return *this ;
    }

    //-------------------------------------------------------------------------------------------
    bool open( const SharedMemory & shm, uint32_t flags ) ;
    bool open( const SharedMemory & shm, uint32_t flags, uint32_t size, uint32_t offset ) ;

    //-------------------------------------------------------------------------------------------
    // The close() function unmaps this memory region and resets all member variables.
    // It returns true on success, false on failure.  
    //-------------------------------------------------------------------------------------------
    bool close() ;

    //-------------------------------------------------------------------------------------------
    inline bool         empty()  const { return begin_ == NULL || size_ == 0 ; }
    inline const void * begin()  const { return begin_  ; }
    inline std::size_t  size()   const { return size_   ; }
    inline std::size_t  offset() const { return offset_ ; }

    //-------------------------------------------------------------------------------------------
    template<typename T>
    T *  
    alloc() 
    { 
      return ( empty() ) 
             ? NULL 
             : new ( begin_ ) T() 
             ;
    }

    //-------------------------------------------------------------------------------------------
    template<typename T, typename... Args>
    T *  
    alloc( Args &&... args ) 
    { 
      return ( empty() ) 
             ? NULL 
             : new ( begin_ ) T( std::forward<Args>( args )... ) 
             ;
    }

  } ;

}}

#endif
