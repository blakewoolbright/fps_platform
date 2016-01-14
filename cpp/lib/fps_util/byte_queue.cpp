#include "byte_queue.h"

namespace fps  {
namespace util {

  //--------------------------------------------------------------------------------------
  ByteQueue::ByteQueue( uint32_t capacity ) 
    : data_    ( NULL ) 
    , capacity_( 0 ) 
    , r_ptr_   ( NULL )
    , r_size_  ( 0 ) 
    , w_size_  ( 0 )
  {}

  //--------------------------------------------------------------------------------------
  ByteQueue::~ByteQueue()
  {
    delete [] data_ ;
    data_   = NULL ;
    w_size_ = 0 ;
    r_size_ = 0 ;
    r_ptr_  = NULL ;
  }

  //--------------------------------------------------------------------------------------
  void 
  ByteQueue::reallocate( uint32_t new_capacity ) 
  { 
    if( new_capacity < Minimum_Capacity ) 
      new_capacity = Minimum_Capacity ;

    if( new_capacity < r_size_ ) 
      new_capacity = r_size_ ;

    char * old_data = data_ ;
    data_ = new char[ new_capacity ] ;

    if( r_size_ ) 
      std::memcpy( data_, r_ptr_, r_size_ ) ;

    r_ptr_  = data_ ;
    w_size_ = capacity_ - r_size_ ;
    delete [] old_data ;
  }

  //--------------------------------------------------------------------------------------
  void 
  ByteQueue::reclaim() 
  { 
    if( r_ptr_ == data_ || r_size_ == 0 ) 
      return ;

    ::memmove( data_, r_ptr_, r_size_ ) ;
    r_ptr_  = data_ ;
    w_size_ = capacity_ - r_size_ ;
  }

  //--------------------------------------------------------------------------------------
  void 
  ByteQueue::reserve( uint32_t min_w_bytes )
  {
    // If we have at least min_w_bytes available for writing, we don't need to do anything.
    if( w_size_ >= min_w_bytes ) 
      return ;

    // Figure out how many bytes are available for writing...
    uint32_t free_w_bytes = ( capacity_ - r_size_ ) ;

    // Either reclaim the necessary space from the start of the underlying
    // buffer, or reallocate the buffer and copy the current unread content to the
    // new location.
    return ( free_w_bytes >= min_w_bytes ) 
           ? reclaim()                              
           : reallocate( capacity_ + min_w_bytes ) 
           ;
    

  }
  
}}
