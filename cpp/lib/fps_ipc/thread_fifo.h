#ifndef FPS__IPC__THREAD_FIFO__H
#define FPS__IPC__THREAD_FIFO__H

#include <type_traits>
#include <cstdlib>
#include "fps_ipc/constants.h"

namespace fps {
namespace ipc {

//--------------------------------------------------------------------------------
// TODO: Test the following
// 
//       Should each array slot occupy an entire cache line?
//       (See: <system-hdrs>/adm-i386/cache.h - Macro: L1_CACHE_BYTES)
//
//--------------------------------------------------------------------------------
template<typename T>
class ThreadFifo
{
public :
  typedef T value_t ;
  static const std::size_t Alignment = constants::Cache_Line_Size ;

private :
  //------------------------------------------------------------------------------
  // Note: The 'alignas' specifiers below guarantee that the size_, r_idx, and 
  //       w_idx_ members will reside in distinct cache lines.
  //------------------------------------------------------------------------------
  uint64_t          capacity_ alignas( Alignment ) ;
  volatile uint64_t size_     alignas( Alignment ) ;
  uint64_t          r_idx_    alignas( Alignment ) ;
  uint64_t          w_idx_    alignas( Alignment ) ;
  T               * data_ ;

  ThreadFifo( const ThreadFifo & ) ;
  ThreadFifo & operator=( const ThreadFifo & ) ;

public :
  //------------------------------------------------------------------------------
  inline ThreadFifo() ;
  inline ThreadFifo( uint64_t capacity ) ;
  inline ~ThreadFifo() ;

  //------------------------------------------------------------------------------
  bool clear() ;
  bool reserve( uint64_t capacity ) ;

  //------------------------------------------------------------------------------
  inline bool     valid()    const { return capacity_ > 0 ; }
  inline uint64_t capacity() const { return capacity_ ; }
  inline uint64_t size()     const { return size_ ; }

  //------------------------------------------------------------------------------
  inline bool write( const T & value ) ;
  inline bool read ( T & result ) ;
} ;

//------------------------------------------------------------------------------
template<typename T>
ThreadFifo<T>::ThreadFifo() 
  : capacity_( 0 ) 
  , size_    ( 0 )
  , r_idx_   ( 0 ) 
  , w_idx_   ( 0 )
  , data_    ( NULL )
{}

//------------------------------------------------------------------------------
template<typename T>
ThreadFifo<T>::ThreadFifo( uint64_t capacity ) 
  : capacity_( 0 ) 
  , size_    ( 0 )
  , r_idx_   ( 0 ) 
  , w_idx_   ( 0 )
  , data_    ( NULL )
{
  reserve( capacity ) ;
}

//------------------------------------------------------------------------------
template<typename T>
ThreadFifo<T>::~ThreadFifo() 
{ 
  clear() ;
}

//------------------------------------------------------------------------------
template<typename T>
bool
ThreadFifo<T>::clear()
{
  if( capacity_ == 0 || data_ == NULL ) 
    return true ;

  for( uint32_t idx = 0 ; idx < capacity_ ; ++idx ) 
    data_[ idx ].~T() ;
  
  free( data_ ) ;
  data_     = NULL ;
  size_     = 0 ;
  capacity_ = 0 ;
  w_idx_    = 0 ;
  r_idx_    = 0 ;
}

//------------------------------------------------------------------------------
template<typename T>
bool
ThreadFifo<T>::reserve( uint64_t capacity ) 
{
  if( valid() || size() > 0 ) 
    return false ;

  int32_t rv = ::posix_memalign( reinterpret_cast<void**>( &data_ )
                               , Alignment
                               , capacity * sizeof( T ) 
                               ) ;
  if( rv ) 
    return false ;

  capacity_ = capacity ;
  for( uint32_t idx = 0 ; idx < capacity_ ; ++idx ) 
    new ( &data_[ idx ] ) T() ;

  w_idx_ = 0 ;
  r_idx_ = 0 ;
  size_  = 0 ;

  return true ;
}

//------------------------------------------------------------------------------
template<typename T>
bool
ThreadFifo<T>::write( const T & value ) 
{ 
  // Make sure the buffer isn't full
  if( size_ == capacity_ ) 
    return false ;

  // Write data to buffer
  data_[ w_idx_ ] = value ;
 
  // Increase fifo size by 1
  __sync_fetch_and_add( &size_, 1 ) ; 

  // Increment & normalize write index 
  // TODO: Figure out how to ditch the conditional
  if( w_idx_ == (capacity_-1) ) 
    w_idx_ = 0 ;
  else 
    ++w_idx_ ;
  
  return true ;
}

//------------------------------------------------------------------------------
template<typename T>
bool
ThreadFifo<T>::read( T & result ) 
{
  // Make sure there's something to read
  if( size_ == 0 ) 
    return false ;

  // Read data element
  result = data_[ r_idx_ ] ;
  
  // Reduce fifo size by 1
  __sync_fetch_and_sub( &size_, 1 ) ;

  // Increment & normalize read index 
  // TODO: Figure out how to ditch the conditional
  if( r_idx_ == (capacity_-1) ) 
    r_idx_ = 0 ;
  else 
    ++r_idx_ ;

  return true ;
}

}}

#endif
