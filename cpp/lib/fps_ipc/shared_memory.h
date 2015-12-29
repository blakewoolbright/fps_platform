#ifndef FPS__IPC__SHARED_MEMORY__H
#define FPS__IPC__SHARED_MEMORY__H

#include "fps_string/fps_string.h"
#include "fps_system/fps_system.h"

#include <sys/mman.h>
#include <fcntl.h>    

namespace fps {
namespace ipc {

  //-------------------------------------------------------------------------------------------
  static const int32_t Read_Only   = O_RDONLY ;
  static const int32_t Read_Write  = O_RDWR   ;
  static const int32_t Create      = O_CREAT  ;
  static const int32_t Truncate    = O_TRUNC  ;
  static const int32_t Exclusive   = O_EXCL   ;

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

  public : 
    //----------------------------------------------------------------------------------------
    SharedMemory() ;
    ~SharedMemory() ;

    //----------------------------------------------------------------------------------------
    bool open( const std::string & name, int32_t flags ) ;
    bool close() ;

    //----------------------------------------------------------------------------------------
    bool resize( uint32_t new_size ) ;

    //----------------------------------------------------------------------------------------
    inline std::string name()        const { return name_   ; }
    inline uint32_t    size()        const { return size_   ; }
    inline int32_t     last_error()  const { return error_  ; }
    inline bool        is_open()     const { return fd_ > 0 ; }
    inline bool        is_readable() const { return is_open() ; }
    inline bool        is_writable() const { return is_open() && (flags_ | Read_Write) ; }
  } ;

}}

#endif

