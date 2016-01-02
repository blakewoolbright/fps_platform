#ifndef FPS__IPC__SHM_FIFO__H
#define FPS__IPC__SHM_FIFO__H

#include <type_traits>
#include <cstdlib>
#include <stdint.h>
#include <atomic>

#include "fps_ipc/constants.h"
#include "fps_ipc/shm_region.h"

namespace fps {
namespace ipc {

  namespace detail 
  {
    /*
    //--------------------------------------------------------------------------------
    template<typename T>
    class
    RingBuffer_Base
    {
    private :
      uint32_t r_idx_ alignas( constants::Cache_Line_Size ) ;
      uint32_t w_idx_ alignas( constants::Cache_Line_Size ) ;
      
      RingBuffer_Base( const RingBuffer_Base & ) = delete ;
      RingBuffer_Base( RingBuffer_Base      && ) = delete ;

    } ;
    */
  }

  //----------------------------------------------------------------------------------
  // TODO: Add configuration options for the following :
  //       1) Compile-time fixed buffer size.
  //       2) Overwrite unread records rather than blocking.
  //----------------------------------------------------------------------------------
  template<typename T>
  class
  RingBuffer_Base
  {
  private :
    //--------------------------------------------------------------------------------
    typedef std::size_t            size_type ;
    typedef std::atomic<size_type> atomic_idx_t ;

    //--------------------------------------------------------------------------------
    atomic_idx_t r_idx_ alignas( constants::Cache_Line_Size ) ;
    atomic_idx_t w_idx_ alignas( constants::Cache_Line_Size ) ;
    
    //--------------------------------------------------------------------------------
    RingBuffer_Base( const RingBuffer_Base & ) = delete ;
    RingBuffer_Base( RingBuffer_Base      && ) = delete ;

  protected :

    //--------------------------------------------------------------------------------
    inline
    static 
    size_type
    readable_count( size_type r_idx, size_type w_idx, size_type capacity )
    {
      return ( w_idx >= r_idx )
             ? w_idx - r_idx
             : w_idx + capacity - r_idx 
             ;
    }

    //--------------------------------------------------------------------------------
    inline
    static 
    size_type 
    writable_count( size_type r_idx, size_type w_idx, size_type capacity )
    {
      size_type rv = r_idx - w_idx - 1 ;
      if( w_idx >= r_idx )
        rv += capacity ;
      return rv ;
    }

    //--------------------------------------------------------------------------------
    inline 
    static
    bool
    empty( size_type r_idx, size_type w_idx ) 
    { return r_idx == w_idx ;
    }

    //--------------------------------------------------------------------------------
    inline
    static 
    size_type 
    advance_index( size_type from, size_type capacity )
    {
      size_type rv = from + 1 ;
      // TODO: Add unlikely macro to the exit condition of this while loop.
      while( rv >= capacity ) { rv -= capacity ; }
      return rv ;
    }

    //--------------------------------------------------------------------------------
    // Note: Writes to write index (w_idx_) 
    //--------------------------------------------------------------------------------
    inline
    bool 
    push( const T & src, T * dest, size_type capacity )
    {
      // Load current write index and compare to read index to determine 
      // whether the queue is full. 
      const size_type w_lst = w_idx_.load( std::memory_order_relaxed ) ; 
      const size_type w_nxt = advance_index( w_lst, capacity ) ;

      const size_type r_cur = r_idx_.load( std::memory_order_acquire ) ;
      if( r_cur == w_nxt ) 
          return false ; 

      // Use placement new to copy construct in place.
      new ( dest + w_nxt ) T( src ) ; 

      // Update write index
      w_idx_.store( w_nxt, std::memory_order_release ) ;

      return true ;
    }

    //--------------------------------------------------------------------------------
    // Note: Writes to read index (r_idx_) 
    //--------------------------------------------------------------------------------
    inline
    bool 
    pop( T & dest, T * src, size_t capacity )
    {
      const size_t w_cur = w_idx_.load( std::memory_order_acquire ) ;
      const size_t r_cur = r_idx_.load( std::memory_order_relaxed ) ; 
      if( empty( w_cur, r_cur ) )
        return false ;

      dest = src[ r_cur ] ;
      src[ r_cur ].~T() ; // TODO: Only call if dtor exists

      size_type r_nxt = advance_index( r_cur, capacity ) ;
      r_idx_.store( r_nxt, std::memory_order_release ) ;
      return true ;
    }

  protected :
    //--------------------------------------------------------------------------------
    inline RingBuffer_Base() { init() ; } 

    //--------------------------------------------------------------------------------
    // Note: Not thread safe!  This should be called by the writer during 
    //       initialization.
    //--------------------------------------------------------------------------------
    inline
    void 
    init()
    {
      w_idx_.store( 0, std::memory_order_relaxed ) ;
      r_idx_.store( 0, std::memory_order_release ) ;
    }

    //--------------------------------------------------------------------------------
    inline
    bool 
    empty()
    {
      return empty( w_idx_.load( std::memory_order_relaxed )  
                  , r_idx_.load( std::memory_order_relaxed )
                  ) ;
    }

    //--------------------------------------------------------------------------------
    // Return true if we're using lock-free atomics
    //--------------------------------------------------------------------------------
    inline
    bool 
    is_lock_free() const
    {
      return w_idx_.is_lock_free() && r_idx_.is_lock_free() ;
    }
  } ;

  //-------------------------------------------------------------------------------------------------
  template<typename T, std::size_t T_Capacity>
  class
  RingBuffer_Fixed 
    : public RingBuffer_Base<T>
  {
  public :
    //-----------------------------------------------------------------------------------------------
    static const std::size_t Capacity = T_Capacity + 1 ;
    static const bool        Is_Fixed = true ;

    //-----------------------------------------------------------------------------------------------
    typedef RingBuffer_Base<T> base_t ;
    typedef typename std::aligned_storage< sizeof( T ) * Capacity, alignof( T ) >::type storage_t ;

  private :
    //-----------------------------------------------------------------------------------------------
    storage_t storage_ ;
    
    //-----------------------------------------------------------------------------------------------
    inline T * data() { return reinterpret_cast<T*>( &storage_ ) ; }

  public :
    
    //-----------------------------------------------------------------------------------------------
    inline
    RingBuffer_Fixed() 
      : base_t() 
    {}

    //-----------------------------------------------------------------------------------------------
    inline 
    bool 
    push( const T & src ) 
    { 
      return base_t::push( src, data(), Capacity ) ;
    }
  
    //-----------------------------------------------------------------------------------------------
    inline 
    bool
    pop( T & dest ) 
    { 
      return base_t::pop( dest, data(), Capacity ) ;
    }
  
  } ;

}}

#endif
