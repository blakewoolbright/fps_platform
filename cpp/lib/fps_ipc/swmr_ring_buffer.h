#ifndef FPS__IPC__SWMR_RING_BUFFER__H
#define FPS__IPC__SWMR_RING_BUFFER__H

#include "fps_ipc/constants.h"
#include "fps_ipc/swmr_spinlock.h"
#include <type_traits>


namespace fps {
namespace ipc {
namespace swmr {

  //--------------------------------------------------------------------------
  template<typename T>
  class Slot 
  {
  private :
    //------------------------------------------------------------------------
    swmr::SpinLock lock_  ;
    uint32_t       valid_ ;
    T              data_  ;

  public : 
    //------------------------------------------------------------------------
    inline Slot() : lock_(), valid_( false ), data_() {}

    //------------------------------------------------------------------------
    inline       T & data()       { return data_ ; }
    inline const T & data() const { return data_ ; }
    
    //------------------------------------------------------------------------
    inline void write_begin() { lock_.write_begin() ; }
    inline void write_end()   { lock_.write_begin() ; }
    
    //------------------------------------------------------------------------
    inline uint64_t read_begin()                 { return lock_.read_begin() ; }
    inline bool     read_end  ( uint64_t state ) { return lock_.read_begin() ; }

    //------------------------------------------------------------------------
    inline 
    bool 
    read( T & dest ) 
    {
      if( !valid_ ) 
        return false ; 

      uint64_t begin_state = lock_.read_begin() ;
      dest = data_ ; 
      return lock_.read_end( begin_state ) ;
    }

    //------------------------------------------------------------------------
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
    atomic_idx_t w_idx_ alignas( constants::Cache_Line_Size ) ;
    storage_t    storage_ ;

    //------------------------------------------------------------------------
    RingBuffer( const RingBuffer & ) = delete ;
    RingBuffer & operator=( const RingBuffer & ) = delete ;

    //------------------------------------------------------------------------
    inline slot_t * data() { return reinterpret_cast<slot_t *>( &storage_ ) ; }

  public :
    //------------------------------------------------------------------------
    inline RingBuffer() ;
    
    //------------------------------------------------------------------------
    void init() ;

    //------------------------------------------------------------------------
    inline void write( const T & value ) ;

    //------------------------------------------------------------------------
    inline bool read( uint32_t idx, T & dest ) ;

    //------------------------------------------------------------------------
    inline uint32_t advance( uint32_t idx ) const ;
  } ;

  //--------------------------------------------------------------------------
  template<typename T, uint32_t T_Capacity>
  RingBuffer<T,T_Capacity>::
  RingBuffer() 
    : w_idx_  ()
    , storage_() 
  {
  } 

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
  read( uint32_t r_idx, T & dest ) 
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
    while( idx >= Capacity ) idx -= Capacity ;
    return idx ;
  }

}}}

#endif
