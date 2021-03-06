#ifndef FPS__IPC__SWMR_RING_BUFFER__H
#define FPS__IPC__SWMR_RING_BUFFER__H

#include "fps_system/fps_system.h"  // For cache line size
#include "fps_util/fps_util.h"      // For fps_likely/unlikely
#include "fps_ipc/swmr_sequence_lock.h"
#include <type_traits>

//
// Single-Writer/Multiple-Reader obstruction free (for the writer) ringbuffer 
// implementation.  
//

namespace fps {
namespace ipc {
namespace swmr {
namespace detail {

  //-----------------------------------------------------------------------------------
  template<typename T>
  class Slot 
  {
  private :
    //---------------------------------------------------------------------------------
    swmr::SequenceLock lock_  ;
    uint32_t           valid_ ;
    T                  data_  ;

    //---------------------------------------------------------------------------------
    inline void write_begin() { lock_.write_begin() ; }
    inline void write_end()   { lock_.write_end()   ; }
    
    //---------------------------------------------------------------------------------
    inline uint64_t read_begin()                 const { return lock_.read_begin() ; }
    inline bool     read_end  ( uint64_t state ) const { return lock_.read_end( state ) ; }

  public : 
    //---------------------------------------------------------------------------------
    inline Slot() : valid_( false ) {}

    //---------------------------------------------------------------------------------
    inline       T & data()       { return data_ ; }
    inline const T & data() const { return data_ ; }

    //---------------------------------------------------------------------------------
    inline 
    bool 
    read( T & dest ) const
    {
      if( !valid_ ) 
        return false ; 

      uint64_t begin_state = lock_.read_begin() ;
      dest = data_ ; 
      return lock_.read_end( begin_state ) ;
    }

    //---------------------------------------------------------------------------------
    inline 
    void 
    write( const T & value ) 
    { 
      lock_.write_begin() ;
      data_  = value ; 
      valid_ = true ;
      lock_.write_end() ;
    }
  } ;

  //--------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  class RingBuffer
  {
  public :
    static const uint32_t Capacity = T_Capacity ;
    typedef Slot<T> slot_t ;

  private :
    //------------------------------------------------------------------------
    typedef 
    typename 
    std::aligned_storage<sizeof(slot_t) * Capacity, alignof(slot_t)>::type 
    storage_t ;
    
    //------------------------------------------------------------------------
    typedef std::atomic<uint32_t> atomic_idx_t ;

    //------------------------------------------------------------------------
    atomic_idx_t w_idx_ alignas( system::cpu::Cache_Line_Size ) ;
    storage_t    storage_ ;

    //------------------------------------------------------------------------
    RingBuffer( const RingBuffer & ) = delete ;
    RingBuffer & operator=( const RingBuffer & ) = delete ;

    //------------------------------------------------------------------------
    inline 
    slot_t * 
    data() 
    { return reinterpret_cast<slot_t *>( &storage_ ) ; 
    }

    //------------------------------------------------------------------------
    inline 
    const slot_t * 
    data() const 
    { return reinterpret_cast<const slot_t *>( &storage_ ) ; 
    }
    
    //------------------------------------------------------------------------
    // TODO: Add std::enable_if< std::is_default_constructable<T>::value > 
    //       check that skips construction if unavailable.
    //------------------------------------------------------------------------
    void init() ;

  public :
    //------------------------------------------------------------------------
    inline RingBuffer() ;

    //------------------------------------------------------------------------
    inline void write( const T & value ) ;

    //------------------------------------------------------------------------
    inline bool read( uint32_t idx, T & dest ) const ;

    //------------------------------------------------------------------------
    inline uint32_t advance( uint32_t idx ) const ;

    //------------------------------------------------------------------------
    // Note : Debug Only 
    //------------------------------------------------------------------------
    inline 
    uint32_t 
    write_index() const
    { uint32_t rv = w_idx_.load( std::memory_order_relaxed ) ;
      return rv ;
    }
  } ;

  //--------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  RingBuffer<T,T_Capacity>::
  RingBuffer() 
  {
    init() ;
  } 

  //--------------------------------------------------------------------------
  // TODO: std::enable_if< std::is_default_constructable<T>::value > 
  //--------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  void
  RingBuffer<T,T_Capacity>::
  init() 
  { 
    w_idx_.store( 0, std::memory_order_relaxed ) ;
    for( uint32_t idx = 0 ; idx < Capacity ; ++idx ) 
      new ( data() + w_idx_ ) slot_t() ;
  }

  //--------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  void
  RingBuffer<T,T_Capacity>::
  write( const T & value ) 
  { 
    uint32_t cur_idx = w_idx_.load( std::memory_order_relaxed ) ;
    data()[ cur_idx ].write( value ) ;
  
    if( cur_idx == (Capacity - 1) ) 
      w_idx_.store( 0, std::memory_order_release ) ;
    else 
      w_idx_.store( cur_idx + 1, std::memory_order_release ) ;
  }
  
  //------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  bool
  RingBuffer<T,T_Capacity>::
  read( uint32_t r_idx, T & dest ) const
  {
    if( r_idx >= Capacity )  //TODO: Add unlikely() macro
      return false ;

    uint32_t w_idx = w_idx_.load( std::memory_order_acquire ) ;
    if( r_idx == w_idx ) 
      return false ;
    
    return data()[ r_idx ].read( dest ) ;
  }

  //------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  uint32_t 
  RingBuffer<T, T_Capacity>::
  advance( uint32_t idx ) const
  {
    ++idx ;
    while( fps_unlikely( idx >= Capacity ) ) 
      idx -= Capacity ;
    return idx ;
  }

}}}}

#endif
