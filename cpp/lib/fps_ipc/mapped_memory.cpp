#include "fps_ipc/mapped_memory.h"
#include <errno.h>

namespace fps {
namespace ipc {

  //-------------------------------------------------------------------------------------------
  MappedMemory::
  MappedMemory( const SharedMemory & shm, uint32_t flags )
    : begin_ ( NULL )
    , size_  ( 0 )
    , offset_( 0 )
  {
    if( shm.size() == 0 ) 
    { error_ = EINVAL ;
      throw except::RuntimeError( "ipc::MappedMemory :: Refusing to map region of size=0" ) ; 
    }
  
    if( !shm.is_open() ) 
    { error_ = EBADF ;
      throw except::RuntimeError( "ipc::MappedMemory :: Shared memory segment not open" ) ;
    }

    if( !open( shm, flags ) ) 
    { 
      error_ = errno ;
      throw except::RuntimeError
            ( "ipc::MappedMemory :: Failed to map memory ( fd: %d, size: %u, errno: %d )"
            , shm.fd()
            , shm.size()
            , error_  
            ) ;
    }
  }

  //-------------------------------------------------------------------------------------------
  MappedMemory::
  MappedMemory( const SharedMemory & shm
              , uint32_t             flags 
              , uint32_t             size
              , uint32_t             offset
              ) 
    : begin_ ( NULL )
    , size_  ( 0 )
    , offset_( 0 )
    , error_ ( 0 )
  {
    if( size == 0 ) 
    { error_ = EINVAL ;
      throw except::RuntimeError( "ipc::MappedMemory :: Refusing to map region of size=0" ) ;
    }

    if( size > shm.size() || offset > size ) 
    { error_ = EINVAL ;
      throw except::RuntimeError
            ( "ipc::MappedMemory :: Requested size %u exceeds available size %lu"
            , size
            , shm.size() 
            ) ;
    }

    if( !shm.is_open() || shm.fd() < 0 ) 
    { error_ = EBADF ;
      throw except::RuntimeError( "ipc::MappedMemory :: Target ipc::SharedMemory instance is closed" ) ;
    }

    if( !open( shm, flags, size, offset ) ) 
    { error_ = errno ;
      throw except::RuntimeError
            ( "ipc::MappedMemory :: Failed to map memory ( fd: %d, size: %u, errno: %d )"
            , shm.fd()
            , shm.size()
            , error_ 
            ) ;
    }
  }


  //-------------------------------------------------------------------------------------------
  void * 
  MappedMemory::
  map_memory( int32_t     fd
            , uint32_t    flags
            , uint32_t    size 
            , uint32_t    offset 
            ) 
  {
    int32_t mmap_prot = PROT_READ ;
    if( flags & access::Read_Write ) 
      mmap_prot |= PROT_WRITE ;
  
    void * rv = ::mmap( NULL, size, mmap_prot, MAP_SHARED, fd, offset ) ;
    return ( rv == MAP_FAILED ) 
           ? NULL 
           : rv 
           ;
  }

  //-------------------------------------------------------------------------------------------
  bool
  MappedMemory::
  unmap_memory( void * ptr, uint32_t size ) 
  {
    return ( 0 == ::munmap( ptr, size ) ) ;
  }

  //-------------------------------------------------------------------------------------------
  bool
  MappedMemory::
  open( const SharedMemory & shm, uint32_t flags )
  { 
    error_ = 0 ;
    uint32_t shm_size = shm.size() ;
    begin_ = MappedMemory::map_memory( shm.fd(), flags, shm_size, 0 ) ;
    if( begin_ == NULL ) 
    { error_ = errno ;
      return false ;
    }
  
    size_   = shm_size ;
    offset_ = 0 ;
    return true ;
  }

  //-------------------------------------------------------------------------------------------
  bool
  MappedMemory::
  open( const SharedMemory & shm, uint32_t flags, uint32_t size, uint32_t offset )
  { 
    error_ = 0 ;

    begin_ = MappedMemory::map_memory( shm.fd(), flags, size, offset ) ;
    if( begin_ == NULL ) 
    { error_ = errno ;
      return false ;
    }
  
    size_   = size ;
    offset_ = offset ;
    return true ;
  }
  
  //-------------------------------------------------------------------------------------------
  bool 
  MappedMemory::
  close() 
  { 
    error_ = 0 ;
    if( begin_ == NULL ) 
      return true ;
  
    if( !MappedMemory::unmap_memory( begin_, size_ ) ) 
    { error_ = errno ;
      return false ;  
    }

    begin_ = NULL ;
    size_  = 0 ;
    return true ;
  }

}}
