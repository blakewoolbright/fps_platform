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

  //---------------------------------------------------------------------------------------------
  struct MappedMemory
  {
  private :
    //-------------------------------------------------------------------------------------------
    void       * begin_ ;
    uint32_t     size_  ;
    uint32_t     offset_ ;
    uint32_t     error_ ;

    //-------------------------------------------------------------------------------------------
    static
    void * 
    map_memory( int32_t fd, uint32_t flags, uint32_t size, uint32_t offset ) ;

    //-------------------------------------------------------------------------------------------
    static 
    bool
    unmap_memory( void * ptr, uint32_t size ) ;

    //-------------------------------------------------------------------------------------------
    MappedMemory( const MappedMemory & ) = delete ;

    //-------------------------------------------------------------------------------------------
    MappedMemory & operator=( const MappedMemory & ) = delete ;

  public :
    //-------------------------------------------------------------------------------------------
    inline 
    MappedMemory() 
      : begin_ ( NULL ) 
      , size_  ( 0 )
      , offset_( 0 )
      , error_ ( 0 )
    {}

    //-------------------------------------------------------------------------------------------
    MappedMemory( const SharedMemory & shm, uint32_t flags ) ;

    //-------------------------------------------------------------------------------------------
    MappedMemory( const SharedMemory & shm
                , uint32_t             flags
                , uint32_t             size
                , uint32_t             offset
                ) ;

    //-------------------------------------------------------------------------------------------
    inline ~MappedMemory() { close() ; }

    //-------------------------------------------------------------------------------------------
    // TODO : For the second 'open' signature, make it less likely that someone might 
    //        pass the size via the 'offset' argument or vice-versa.
    //
    // Map the indicated shared memory segment into local address space.  Return true on success,
    // or false on failure.  On failure, the internal error_ member will be populated with the
    // relevant system errno value describing the problem.  Users may check this value via the 
    // 'last_error()' function.
    //-------------------------------------------------------------------------------------------
    bool open( const SharedMemory & shm, uint32_t flags ) ;
    bool open( const SharedMemory & shm, uint32_t flags, uint32_t size, uint32_t offset ) ;

    //-------------------------------------------------------------------------------------------
    // The close() function unmaps this memory region and resets all member variables.
    // It returns true on success, false on failure.  
    //-------------------------------------------------------------------------------------------
    bool close() ;

    //-------------------------------------------------------------------------------------------
    inline bool         is_open()    const { return begin_ == NULL || size_ == 0 ; }
    inline const void * begin()      const { return begin_  ; }
    inline uint32_t     size()       const { return size_   ; }
    inline uint32_t     offset()     const { return offset_ ; }
    inline uint32_t     last_error() const { return error_ ; }

    //-------------------------------------------------------------------------------------------
    template<typename T>
    T * 
    cast() 
    { return reinterpret_cast<T *>( begin_ ) ;
    }

    //-------------------------------------------------------------------------------------------
    template<typename T>
    const T * 
    cast() const
    { return reinterpret_cast<const T *>( begin_ ) ;
    }
  
    //-------------------------------------------------------------------------------------------
    template<typename T>
    T *  
    construct() 
    { 
      return ( is_open() || sizeof( T ) > size() ) 
             ? NULL 
             : new ( begin_ ) T
             ;
    }

    //-------------------------------------------------------------------------------------------
    template<typename T, typename... Args>
    T *  
    construct( Args &&... args ) 
    { 
      return ( is_open() || sizeof( T ) > size() ) 
             ? NULL 
             : new ( begin_ ) T( std::forward<Args>( args )... ) 
             ;
    }

  } ;

}}

#endif
