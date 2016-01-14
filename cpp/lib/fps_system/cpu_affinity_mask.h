#ifndef FPS__SYSTEM__CPU__AFFINITY_MASK__H
#define FPS__SYSTEM__CPU__AFFINITY_MASK__H

#include <cstdint>
#include <thread>
#include <sched.h>
#include <unistd.h>
#include <cstring>

namespace fps {
namespace system {
namespace cpu {

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
    std::string to_string() const ;
    
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
  
}}}

#endif
