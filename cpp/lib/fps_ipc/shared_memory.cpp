#include "fps_ipc/shared_memory.h" 
#include "fps_string/fps_string.h"
#include "fps_except/fps_except.h"
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
  SharedMemory::SharedMemory() 
    : fd_   ( -1 )
    , size_ ( 0 )
    , flags_( 0 )
    , error_( 0 )
  {
  }

  //-------------------------------------------------------------------------------------------
  SharedMemory::~SharedMemory() 
  { 
    close( false ) ;
  }

  //-------------------------------------------------------------------------------------------
  // Sanitize user provided shm file names.
  //-------------------------------------------------------------------------------------------
  bool
  SharedMemory::sanitize_name()
  {
    // Remove whitespace.
    string::strip( name_ ) ;

    // Remove acceptable directory prefix.
    std::string sys_shm_dir( "/dev/shm/" ) ;
    if( string::starts_with( name_, sys_shm_dir ) ) 
      name_.erase( 0, (sys_shm_dir.length() - 1) ) ;

    // Count filesystem delimiters.
    uint32_t slash_count = 0 ;
    for( auto itr = name_.begin() ; itr != name_.end() ; ++itr ) 
    { if( *itr == '/' )  
        ++slash_count ;
    }
    
    if( slash_count > 1 ) 
    { error_ = EINVAL ;
      return false ;
    }
    
    if( slash_count == 0 ) 
      name_.insert( name_.begin(), '/' ) ;
    
    return true ;
  }

  //-------------------------------------------------------------------------------------------
  bool 
  SharedMemory::load_size_from_filesystem() 
  {
    if( name_.empty() ) 
    { 
      error_ = EINVAL ;
      return false ;
    }

    // The fps::fs::Path class has a size() member that can tell us how big our
    // shm segment is currently.
    fs::Path shm_path = this->path() ;
  
    // Make sure the path object constructed properly or fail
    if( !shm_path.valid() )
    { 
      error_ = ( shm_path.last_error() == 0 ) 
             ? shm_path.last_error() 
             : EINVAL 
             ;

      return false ;
    }

    // Make sure this shm segment actually exists.
    if( !shm_path.exists() ) 
    {
      error_ = EBADF ;
      return false ;
    }
    
    // Update our size_ member
    size_ = shm_path.size() ;
    return true ;
  }

  //-------------------------------------------------------------------------------------------
  bool
  SharedMemory::open( const std::string & name, uint32_t access_flags )
  {
    if( is_open() ) 
      close( false ) ;

    error_ = 0 ;
    size_  = 0 ;
    flags_ = 0 ;
    name_  = name ;

    //
    // Verify that "name" argument meets basic requirements, and normalize it.
    // 
    if( !sanitize_name() ) 
    { 
      error_ = EINVAL ;
      name_.clear() ;
      return false ;
    }

    //
    // Build flagset that will be passed to shm_open() 
    //
    if( access_flags & access::Read_Write ) 
      flags_ |= O_RDWR ;
    if( access_flags & access::Create ) 
      flags_ |= O_CREAT ;
    if( access_flags & access::Exclusive ) 
      flags_ |= O_EXCL ;

    //
    // Attempt to open the desired shared memory file
    //
    fd_    = ::shm_open( name_.c_str(), flags_, 0666 ) ;
    if( fd_ < 0 ) 
    { 
      error_ = errno ;
      fd_    = -1 ;
      return false ;
    }

    //
    // Query the filesystem to determine the initial shm segment size.
    // 
    if( !load_size_from_filesystem() ) 
    { 
      int32_t last_err = error_ ;
      close( false ) ;
      error_ = last_err ;
      return false ;
    }

    return true ;
  }

  //-------------------------------------------------------------------------------------------
  bool
  SharedMemory::close( bool remove_from_fs )
  {
    bool rv = true ;
    if( is_open() ) 
    { 
      // Close underlying file handle
      if( ::close( fd_ ) != 0 ) 
      { 
        if( error_ == 0 ) 
        { 
          error_ = errno ;
          rv     = false ;
        }
      }
      else
      { 
        if( remove_from_fs )
          path().rm() ;
      }
    }

    // Clear members
    size_   =  0 ;
    flags_  =  0 ;
    fd_     = -1 ;
    error_  = 0 ;

    return rv ;
  }

  //-------------------------------------------------------------------------------------------
  bool 
  SharedMemory::resize( uint32_t bytes ) 
  {
    error_ = 0 ;

    if( !is_open() ) 
    { 
      error_ = EBADF ;
      return false ;
    }

    if( !is_writable() ) 
    { 
      error_ = EACCES ;
      return false ;
    }

    //
    // Resize underlying shm file via ::ftruncate() system function.
    // Note: ftruncate() is called in a loop so we can retry if it fails 
    //       and sets errno to EINTR/EAGAIN.
    //
    for( uint32_t idx = 0 ; idx < 5 ; ++idx )
    {
      error_     = 0 ;
      int result = ::ftruncate( fd_, bytes ) ;
      if( result == 0 ) 
      { 
        size_ = bytes ;
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
