#ifndef FPS__SYSTEM__SYSTEM__H
#define FPS__SYSTEM__SYSTEM__H

#include <thread>
#include <sched.h>
#include <unistd.h>
#include <stdint.h>

namespace fps {
namespace system {

  /* TODO:

  current user
  current group
  installed ram
  X installed cpu cores 
  network interfaces
  
  */

  //------------------------------------------------------------------------
  inline
  uint32_t
  cpu_core_count() 
  { 
    return std::thread::hardware_concurrency() ;
    // return ::sysconf( _SC_NPROCESSORS_ONLN ) ;
  }

  //------------------------------------------------------------------------
  struct AffinityMask
  {
  private :
    mutable ::cpu_set_t mask_ ;

  public :
    //----------------------------------------------------------------------
    inline AffinityMask() { reset() ; }

    //----------------------------------------------------------------------
    inline 
    AffinityMask( uint32_t core_id ) 
    { 
      reset() ;
      set( core_id ) ;
    }

    //----------------------------------------------------------------------
    const ::cpu_set_t & cpu_set() const { return mask_ ; }
    ::cpu_set_t       & cpu_set()       { return mask_ ; }

    //----------------------------------------------------------------------
    inline 
    bool 
    is_set( uint32_t core_id ) const 
    { return CPU_ISSET( core_id, &mask_ ) ;
    }

    //----------------------------------------------------------------------
    inline
    bool 
    set( uint32_t core_id ) 
    { 
      if( core_id > CPU_SETSIZE ) 
        return false ;

      CPU_SET( core_id, &mask_ ) ;
      return true ;
    }

    //----------------------------------------------------------------------
    inline
    bool
    clr( uint32_t core_id ) 
    {
      if( core_id > CPU_SETSIZE ) 
        return false ;

      CPU_CLR( core_id, &mask_ ) ;
      return true ;
    }
    
    //----------------------------------------------------------------------
    inline uint32_t size()  const { return CPU_COUNT( &mask_ ) ; }
    inline bool     empty() const { return size() == 0 ; }

    //----------------------------------------------------------------------
    inline void reset() { CPU_ZERO( &mask_ ) ; }

    //----------------------------------------------------------------------
    inline
    std::string
    to_string() const 
    {
      std::string rv ;
      uint32_t tgt_count = size() ; 
      uint32_t cur_count = 0 ;
      for( uint32_t idx = 0 
         ; idx < CPU_SETSIZE && cur_count < tgt_count 
         ; ++idx 
         ) 
      { if( !CPU_ISSET( idx, &mask_ ) ) 
          continue ;

        string::append( rv, "%u,", idx ) ;
        ++cur_count ;
      }

      if( !rv.empty() ) 
        rv.pop_back() ;
      
      return rv ;
    }
    
    //----------------------------------------------------------------------
    inline 
    AffinityMask & 
    operator=( const AffinityMask & rhs ) 
    { std::memcpy( &mask_, &(rhs.mask_), sizeof( mask_ ) ) ;
      return *this ;
    }

    //----------------------------------------------------------------------
    inline 
    bool
    operator==( const AffinityMask & rhs ) const 
    { return CPU_EQUAL( &mask_, &(rhs.mask_) ) ;
    }

    //----------------------------------------------------------------------
    inline 
    bool 
    operator!=( const AffinityMask & rhs ) const 
    { return !this->operator==( rhs ) ;
    }

  } ;

  //------------------------------------------------------------------------
  inline 
  bool
  set_affinity( const AffinityMask & mask ) 
  {
    int32_t rv = ::pthread_setaffinity_np( ::pthread_self()
                                         , sizeof( cpu_set_t )
                                         , &(mask.cpu_set())
                                         ) ;
    return ( rv == 0 ) ;
  }

  //------------------------------------------------------------------------
  inline
  AffinityMask 
  get_affinity() 
  {
    AffinityMask rv ;
    if( 0 != ::pthread_getaffinity_np( ::pthread_self()
                                     , sizeof( cpu_set_t ) 
                                     , &(rv.cpu_set())
                                     ) 
      ) 
    { rv.reset() ; 
    }

    return rv ;
  }
  
}}

#endif
