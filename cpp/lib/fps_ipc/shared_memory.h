#ifndef FPS__IPC__SHARED_MEMORY__H
#define FPS__IPC__SHARED_MEMORY__H

#include "fps_string/fps_string.h"
#include "fps_system/fps_system.h"
#include "fps_fs/path.h"
#include "fps_ipc/ipc_util.h"

#include <sys/mman.h>
#include <fcntl.h>    

namespace fps {
namespace ipc {

  //-------------------------------------------------------------------------------------------
  class SharedMemory
  {
  private :
    //----------------------------------------------------------------------------------------
    int32_t     fd_    ;  // File descriptor for shared memory region.
    uint32_t    size_  ;  // Size in bytes of shared memory region.
    int32_t     flags_ ;  // Open flags passed to shm_open.
    int32_t     error_ ;  // The value of errno after the last operation.
    std::string name_  ;  // Name of filesystem node associated w/ this shm region.
   
    //----------------------------------------------------------------------------------------
    SharedMemory( const SharedMemory & ) ;
    SharedMemory( SharedMemory && ) ;
    SharedMemory & operator=( const SharedMemory & ) ;

    //----------------------------------------------------------------------------------------
    bool sanitize_name() ;

    //----------------------------------------------------------------------------------------
    bool load_size_from_filesystem() ;

  public : 
    //----------------------------------------------------------------------------------------
    SharedMemory() ;
    ~SharedMemory() ;

    //----------------------------------------------------------------------------------------
    // Open or Create a shm segment w/ the indicated name and access rights.  
    //
    // The "flags" argument should be the bitwise OR of one or more flag values 
    // defined in the ipc::access namespace ( ipc_util.h ).  At the moment, the following 
    // flags are supported :
    //   access::Read_Only   ( Read access )
    //   access::Read_Write  ( Read and write access )
    //   access::Create      ( Create the segment if it doesn't exist )
    //   access::Exclusive   ( Fail if the segment already exists and access::Create was specified )
    //
    // When a segment is created, the filesystem permissions are default to 0666 (octal).
    //
    // Return true on success, false on failure.  On failure, the associated "errno" value
    // can be retrieved via the last_error() member function to assist w/ debugging.
    //----------------------------------------------------------------------------------------
    bool open( const std::string & name, uint32_t flags ) ;

    //----------------------------------------------------------------------------------------
    // Try to open an existing shm segment w/ the indicated name w/ the indicated access
    // flags.  
    //
    // The "flags" member should be the result of OR'ing together one or more of the 
    // flag definitions in the ipc::access namespace ( ipc_util.h ).
    //
    // On success, return true.  On failure, return false.  
    //----------------------------------------------------------------------------------------
    // bool open_or_fail( const std::string & name, uint32_t flags ) ;

    //----------------------------------------------------------------------------------------
    bool close() ;

    //----------------------------------------------------------------------------------------
    // Reserve/resize this shm segment to ensure it can hold the indicated number of bytes.
    // Return true on success, false on failure.  
    //----------------------------------------------------------------------------------------
    bool resize( uint32_t bytes ) ;

    //----------------------------------------------------------------------------------------
    template<typename T>
    bool
    resize() 
    {   
      return resize( static_cast<uint32_t>( sizeof( T ) ) ) ; 
    }
  
    //----------------------------------------------------------------------------------------
    inline fs::Path    path()        const { return fs::Path( "/dev/shm", name_ ) ; } 
    inline int32_t     fd()          const { return fd_     ; }
    inline std::string name()        const { return name_   ; }
    inline uint32_t    size()        const { return size_   ; }
    inline int32_t     last_error()  const { return error_  ; }
    inline bool        is_open()     const { return fd_ > 0 ; }
    inline bool        is_readable() const { return is_open() ; }
    inline bool        is_writable() const { return is_open() && (flags_ & O_RDWR ) ; }
  } ;

}}

#endif

