#ifndef FPS__IPC__SHARED_MEMORY__H
#define FPS__IPC__SHARED_MEMORY__H

#include "fps_string/fps_string.h"
#include "fps_system/fps_system.h"

#include <sys/mman.h>
#include <fcntl.h>    

namespace fps {
namespace ipc {

  //-------------------------------------------------------------------------------------------
  /*
  static const int32_t Read_Only   = O_RDONLY ;
  static const int32_t Read_Write  = O_RDWR   ;
  static const int32_t Create      = O_CREAT  ;
  static const int32_t Truncate    = O_TRUNC  ;
  static const int32_t Exclusive   = O_EXCL   ;
  */

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
    void      * ptr_   ; 
   
    //----------------------------------------------------------------------------------------
    SharedMemory( const SharedMemory & ) ;
    SharedMemory( SharedMemory && ) ;
    SharedMemory & operator=( const SharedMemory & ) ;

    //----------------------------------------------------------------------------------------
    bool reserve( uint32_t bytes ) ;

    //----------------------------------------------------------------------------------------
    static bool validate_name( const std::string & name ) ;

  public : 
    //----------------------------------------------------------------------------------------
    SharedMemory() ;
    ~SharedMemory() ;

    //----------------------------------------------------------------------------------------
    // Create a read/write shm segment w/ the indicated name and size (in bytes).  
    //
    // Return true on success, false on failure (and sets internal error_ member).
    // This function passes the flagset O_CREAT | O_RDWR | O_EXCL to shm_open and sets 
    // the file level permissions to 0666 (octal).  The function will fail if any of the 
    // following is true :
    // 1) The 'name' argument is empty.
    // 2) Another shm segment w/ the same name already exists.
    // 3) The shm segment was created, but resizing failed.
    // 4) The segment was not successfully mmap()'d.
    //
    // This function should be called by producer/writer processes during initialization.
    //----------------------------------------------------------------------------------------
    bool create( const std::string & name, uint32_t size_in_bytes ) ;

    //----------------------------------------------------------------------------------------
    bool open  ( const std::string & name ) ;

    //----------------------------------------------------------------------------------------
    bool close() ;
  
    // NOTE: Should I add a way for users to call msync() 

    //----------------------------------------------------------------------------------------
    inline void * data() const { return ptr_    ; }

    //----------------------------------------------------------------------------------------
    inline std::string name()        const { return name_   ; }
    inline uint32_t    size()        const { return size_   ; }
    inline int32_t     last_error()  const { return error_  ; }
    inline bool        is_open()     const { return fd_ > 0 ; }
    inline bool        is_readable() const { return is_open() ; }
    inline bool        is_writable() const { return is_open() && (flags_ & O_RDWR ) ; }
  } ;

}}

#endif

