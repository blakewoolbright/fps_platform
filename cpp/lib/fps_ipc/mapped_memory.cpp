#include "fps_ipc/mapped_memory.h"

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
      throw except::RuntimeError( "ipc::MappedMemory :: Refusing to map region of size=0" ) ;
  
    if( !shm.is_open() ) 
      throw except::RuntimeError( "ipc::MappedMemory :: Shared memory segment not open" ) ;

    if( !open( shm, flags ) ) 
    { 
      int32_t last_error = errno ;
      throw except::RuntimeError
            ( "ipc::MappedMemory :: Failed to map memory ( fd: %d, size: %u, errno: %d )"
            , shm.fd()
            , shm.size()
            , last_error 
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
  {
    if( size == 0 ) 
      throw except::RuntimeError( "ipc::MappedMemory :: Refusing to map region of size=0" ) ;

    if( size > shm.size() ) 
    { throw except::RuntimeError
            ( "ipc::MappedMemory :: Requested size %u exceeds available size %lu"
            , size
            , shm.size() 
            ) ;
    }

    if( !shm.is_open() || shm.fd() < 0 ) 
      throw except::RuntimeError( "ipc::MappedMemory :: Target ipc::SharedMemory instance is closed" ) ;

    if( !open( shm, flags, size, offset ) ) 
    { int32_t last_error = errno ;
      throw except::RuntimeError
            ( "ipc::MappedMemory :: Failed to map memory ( fd: %d, size: %u, errno: %d )"
            , shm.fd()
            , shm.size()
            , last_error 
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
    int32_t mmap_flags = PROT_READ ;
    if( flags & access::Read_Write ) 
      mmap_flags |= PROT_WRITE ;
  
    void * rv = ::mmap( NULL, size, mmap_flags, MAP_SHARED, fd, offset ) ;
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
    return (0 == ::munmap( ptr, size )) ;
  }

  //-------------------------------------------------------------------------------------------
  bool
  MappedMemory::
  open( const SharedMemory & shm, uint32_t flags )
  { 
    begin_ = MappedMemory::map_memory( shm.fd(), flags, shm.size(), 0 ) ;
    if( begin_ == NULL ) 
      return false ;
  
    size_   = shm.size() ;
    offset_ = 0 ;
    return true ;
  }

  //-------------------------------------------------------------------------------------------
  bool
  MappedMemory::
  open( const SharedMemory & shm, uint32_t flags, uint32_t size, uint32_t offset )
  { 
    begin_ = MappedMemory::map_memory( shm.fd(), flags, size, offset ) ;
    if( begin_ == NULL ) 
      return false ;
  
    size_   = size ;
    offset_ = offset ;

    return true ;
  }
  
  //-------------------------------------------------------------------------------------------
  bool 
  MappedMemory::
  close() 
  { 
    if( begin_ == NULL ) 
      return true ;
  
    if( !MappedMemory::unmap_memory( begin_, size_ ) ) 
      return false ;

    begin_ = NULL ;
    size_  = 0 ;
    return true ;
  }

}}
