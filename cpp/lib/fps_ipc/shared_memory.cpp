#include "fps_ipc/shared_memory.h" 

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>

namespace fps {
namespace ipc {

  //-------------------------------------------------------------------------------------------
  SharedMemory::SharedMemory() 
    : fd_   ( -1 )
    , size_ ( 0 )
    , flags_( 0 )
    , error_( 0 )
    , ptr_  ( NULL ) 
  {
  }

  //-------------------------------------------------------------------------------------------
  SharedMemory::~SharedMemory() 
  { 
    close() ;
  }

  //-------------------------------------------------------------------------------------------
  bool
  SharedMemory::open( const std::string & name, int32_t flags )
  {
    if( is_open() ) 
      close() ;
    
    error_ = 0 ;

    return false ;
  }

  //-------------------------------------------------------------------------------------------
  bool
  SharedMemory::close()
  {
    bool rv = true ;
    error_  = 0 ;
    if( is_open() ) 
    { 
      // ::munmap() 
      if( ::close( fd_ ) != 0 ) 
      { error_ = errno ;
        rv     = false ;
      }
    }

    ptr_   = NULL ;
    size_  = 0 ;
    flags_ = 0 ;

    return rv ;
  }

  //-------------------------------------------------------------------------------------------
  bool 
  SharedMemory::resize( uint32_t new_size ) 
  {
    error_ = 0 ;
    return false ;
  }
}}
