#ifndef FPS__CONTAINER__BYTE_QUEUE__H
#define FPS__CONTAINER__BYTE_QUEUE__H

#include <cstdint>
#include <cstring>

#include "fps_container/extractors.h"

namespace fps  {
namespace container {

  //-----------------------------------------------------------------------
  class ByteRange
  {
  private :
    const char * bytes_ ;
    uint32_t     size_  ;

  public :
    //---------------------------------------------------------------------
    inline ByteRange() : bytes_( NULL ), size_( 0 ) {}

    //---------------------------------------------------------------------
    inline 
    ByteRange( const char * ptr, uint32_t sz ) 
      : bytes_( ptr ) 
      , size_ ( sz  ) 
    {}

    //---------------------------------------------------------------------
    inline const char * begin() const { return bytes_ ; }
    inline const char * end()   const { return bytes_ + size_ ; }
    inline uint32_t     size()  const { return size_ ; }
    inline bool         empty() const { return size_ == 0 ; }

    //---------------------------------------------------------------------
    inline 
    void 
    assign( const char * ptr, uint32_t sz ) 
    { 
      bytes_ = ptr ; 
      size_  = sz  ; 
    }

    //---------------------------------------------------------------------
    inline operator bool() const { return size_ > 0 && bytes_ ; }

    //---------------------------------------------------------------------
    inline 
    bool 
    operator==( const ByteRange & rhs ) const 
    { 
      return bytes_ == rhs.bytes_ && size_ == rhs.size_ ;
    }

    //---------------------------------------------------------------------
    inline 
    bool 
    operator!=( const ByteRange & rhs ) const 
    { 
      return !this->operator==( rhs ) ;
    }
  } ;

  //--------------------------------------------------------------------------------------
  class ByteQueue
  {
  private :
    //------------------------------------------------------------------------
    static const uint32_t Default_Capacity = 4096 ;
    static const uint32_t Minimum_Capacity = 64 ;

    //------------------------------------------------------------------------
    char *   data_     ;
    uint32_t capacity_ ;
    char *   r_ptr_    ;
    uint32_t r_size_   ;
    uint32_t w_size_   ;
    
    //------------------------------------------------------------------------
    void reallocate( uint32_t new_capacity ) ;
    void reclaim() ;

  public :
    //------------------------------------------------------------------------
    explicit ByteQueue( uint32_t capacity = Default_Capacity ) ;
    ~ByteQueue() ;
    
    //------------------------------------------------------------------------
    inline const char * begin() const { return r_ptr_ ; }
    inline const char * end()   const { return begin() + r_size_ ; }
  
    //------------------------------------------------------------------------
    inline char * begin() { return r_ptr_ ; }
    inline char * end()   { return begin() + r_size_ ; }

    //------------------------------------------------------------------------
    inline uint32_t capacity()   const { return capacity_ ; } 
    inline uint32_t size()       const { return r_size_ ; }
    inline bool     empty()      const { return size() == 0 ; }

    //------------------------------------------------------------------------
    // Call reserve() to guarantee that the indicated number of bytes are 
    // available for insert() operations.  If sufficient space already 
    // exists and is contiguous in memory, nothing is done.  If sufficient 
    // non-contiguous space exists, all unread content is moved to the start
    // of the underlying buffer to make it contiguous.  If sufficient space
    // does not exist in the current buffer, a larger buffer is allocated 
    // and unread content is copied over.
    //------------------------------------------------------------------------
    void reserve( uint32_t min_w_bytes ) ;

    //------------------------------------------------------------------------
    // Call insert() to append data to the end of the queue.
    //------------------------------------------------------------------------
    inline void insert( const char * src_ptr, uint32_t src_len ) ;

    //------------------------------------------------------------------------
    template<typename T_Inserter> void insert( const T_Inserter & ) ;
    
    //------------------------------------------------------------------------
    // Call extract() to remove all data from the queue.  The return value
    // is a object of type ByteRange (see top of file).  ByteRange objects
    // can be evaluated in a boolean context to determine whether the 
    // extract() operation succeeded or failed.
    //------------------------------------------------------------------------
    inline ByteRange extract() ;
    
    //------------------------------------------------------------------------
    template<typename T> 
    ByteRange 
    extract() ;

    //------------------------------------------------------------------------
    template<typename T> 
    ByteRange 
    extract( const T & tokenizer ) ;

    //------------------------------------------------------------------------
    // Call clear to invalidate all unread data and reset size_ to zero.
    //------------------------------------------------------------------------
    inline void clear() ;
  } ;

  //-----------------------------------------------------------------------
  void 
  ByteQueue::insert( const char * src_ptr, uint32_t src_len )  
  { 
    reserve ( src_len ) ;
    ::memcpy( end(), src_ptr, src_len ) ;
    r_size_ += src_len ;
    w_size_ -= src_len ;
  }

  //-----------------------------------------------------------------------
  template<typename T_Inserter>
  void 
  ByteQueue::insert( const T_Inserter & inserter) 
  {
    reserve( inserter.max_length() ) ;
    uint32_t rv = inserter.put( end() ) ;
    if( rv > 0 ) 
    { r_size_ += rv ;
      w_size_ -= rv ;
    }
  }

  //-----------------------------------------------------------------------
  ByteRange 
  ByteQueue::extract()
  {
    ByteRange rv( r_ptr_, r_size_ ) ;
    clear() ;
    return rv ;
  }

  //-----------------------------------------------------------------------
  template<typename T_Extractor>
  ByteRange 
  ByteQueue::extract()
  {
    ByteRange rv ;

    if( empty() ) 
      return rv ;

    const char * r_end = T_Extractor::get( r_ptr_, r_size_ ) ;
    if( r_end == NULL ) 
      return rv ;

    rv.assign( r_ptr_, static_cast<uint32_t>( r_end - r_ptr_ ) ) ;
    
    if( rv.size() < r_size_ )
    { r_ptr_  += rv.size() ;
      r_size_ -= rv.size() ;
    }
    else 
      clear() ;

    return rv ;
  }

  //-----------------------------------------------------------------------
  template<typename T>
  ByteRange 
  ByteQueue::extract( const T & extractor )
  {
    ByteRange rv ;
    if( empty() ) 
      return rv ;

    const char * r_end = extractor.get( r_ptr_, r_size_ ) ;
    if( r_end == NULL ) 
      return rv ;

    rv.assign( r_ptr_, static_cast<uint32_t>( r_end - r_ptr_ ) ) ;
    
    if( rv.size() < r_size_ )
    { r_ptr_  += rv.size() ;
      r_size_ -= rv.size() ;
    }
    else 
      clear() ;

    return rv ;
  }

  //-----------------------------------------------------------------------
  void 
  ByteQueue::clear() 
  { 
    r_ptr_  = data_ ;
    r_size_ = 0 ;
    w_size_ = capacity_ ;
  }

}}

#endif

