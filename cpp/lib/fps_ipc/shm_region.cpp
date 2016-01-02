#include "fps_ipc/shm_region.h" 
#include "fps_string/fps_string.h"
#include "fps_fs/path.h"

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
  ShmRegion::ShmRegion() 
    : fd_   ( -1 )
    , size_ ( 0 )
    , flags_( 0 )
    , error_( 0 )
    , ptr_  ( NULL ) 
  {
  }

  //-------------------------------------------------------------------------------------------
  ShmRegion::~ShmRegion() 
  { 
    close() ;
  }

  //-------------------------------------------------------------------------------------------
  bool
  ShmRegion::validate_name( const std::string & name ) 
  {
    if( name.size() < 2 || name[0] != '/' ) 
      return false ;
 
    uint32_t slash_count = 0 ;
    for( auto itr = name.begin() ; itr != name.end() ; ++itr ) 
    { 
      if( *itr != '/' )  
        continue ;

      ++slash_count ;
      if( slash_count > 1 ) 
        return false ;
    }
    
    return true ;
  }

  //-------------------------------------------------------------------------------------------
  // Create a new shm segment w/ the indicated name and size (in bytes).
  // This function should be used by writer / producer implementations.  Reader / consumer 
  // implementations should just call open().
  //-------------------------------------------------------------------------------------------
  bool
  ShmRegion::try_open( const std::string & name, Access access )
  {
    close() ;
    error_ = 0 ;
    flags_ = O_RDONLY ;
    name_  = string::stripped( name ) ;
    if( !name_.empty() && name_[0] != '/' ) 
      name_.insert( name_.begin(), '/' ) ;

    if( access == Read_Write ) 
      flags_ |= O_RDWR ;

    fd_    = ::shm_open( name_.c_str(), flags_, 0666 ) ;
    if( fd_ < 0 )
    { error_ = errno ;
      fd_    = -1    ;
      return false ;
    }

    fs::Path shm_path( "/dev/shm", name_.c_str() ) ;
    if( !shm_path.exists() ) 
    { close() ;
      error_ = EBADF ;
      return false ;
    }
  
    int access_flags = PROT_READ ;
    if( access == Read_Write ) 
      access_flags |= PROT_WRITE ;

    ptr_ = ::mmap( NULL, shm_path.size(), access_flags, MAP_SHARED, fd_, 0 ) ;
    if( ptr_ == MAP_FAILED ) 
    { int32_t save_errno = errno ;
      close() ;
      error_ = save_errno ;
      return false ;
    }

    size_ = shm_path.size() ;
    return true ;
  }

  //-------------------------------------------------------------------------------------------
  // Create a new shm segment w/ the indicated name and size (in bytes).
  // This function should be used by writer / producer implementations.  Reader / consumer 
  // implementations should just call try_open().
  //-------------------------------------------------------------------------------------------
  bool
  ShmRegion::try_create( const std::string & name, uint32_t size_in_bytes )
  {
    close() ;

    error_ = 0 ;
    name_  = string::stripped( name ) ;
    if( !name_.empty() && name_[0] != '/' ) 
      name_.insert( name_.begin(), '/' ) ;
    
    if( !validate_name( name_ ) ) 
    { error_ = EINVAL ;
      return false ;
    }

    flags_ = O_CREAT | O_RDWR | O_EXCL ;
    fd_    = ::shm_open( name_.c_str(), flags_, 0666 ) ;
    if( fd_ < 0 ) 
    { error_ = errno ;
      fd_    = -1 ;
      return false ;
    }

    if( !reserve( size_in_bytes ) ) 
      return false ;
    
    ptr_ = ::mmap( NULL, size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0 ) ;
    if( ptr_ == NULL ) 
    { int32_t save_errno = errno ;
      close() ;
      error_ = save_errno ;
      return false ;
    }

    return true ;
  }

  //-------------------------------------------------------------------------------------------
  bool
  ShmRegion::close()
  {
    bool rv = true ;
    error_  = 0 ;
    if( is_open() ) 
    { 
      // Remove mmaping of shm region.
      if( ::munmap( ptr_, size_ ) != 0 ) 
      { 
        error_ = errno ;
        rv     = false ;
      }

      // Close underlying file handle
      if( ::close( fd_ ) != 0 ) 
      { 
        if( error_ == 0 ) 
        { error_ = errno ;
          rv     = false ;
        }
      }
    }

    // Clear members
    ptr_   = NULL ;
    size_  =  0 ;
    flags_ =  0 ;
    fd_    = -1 ;

    return rv ;
  }

  //-------------------------------------------------------------------------------------------
  bool 
  ShmRegion::reserve( uint32_t bytes ) 
  {
    error_ = 0 ;

    if( !is_open() ) 
    { error_ = EBADF ;
      return false ;
    }

    if( !is_writable() ) 
    { error_ = EACCES ;
      return false ;
    }

    if( bytes <= size_ ) 
      return true ;

    // Call ftruncate in a loop so we can retry on EINTR/EAGAIN
    for( uint32_t idx = 0 ; idx < 5 ; ++idx )
    {
      error_     = 0 ;
      int result = ::ftruncate( fd_, bytes ) ;
      if( result == 0 ) 
      { size_ = bytes ;
        return true ;
      }

      error_ = errno ;
      if( error_ == EINTR || error_ == EAGAIN ) 
        continue ;

      break ;
    }

    return false ;
  }
}}
