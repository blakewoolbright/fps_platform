#ifndef FPS__IPC__IPC_UTIL__H
#define FPS__IPC__IPC_UTIL__H

namespace fps {
namespace ipc {
namespace detail {

  //--------------------------------------------------------------------------------
  /*
  template<typename T, uint64_t T_Size>
  union PadType 
  {
    static const uint32_t Total_Size = sizeof(T) + (T_Size-(sizeof(T) % T_Size)) ;
    typedef T value_t ;

    value_t value_ ;
    char    pad_[ Total_Size ] ;
  } ;
  */

  //--------------------------------------------------------------------------------
  template<typename T>
  struct 
  alignas( 64 )
  Align64
  {
  public :
    typedef T value_t ;

  private :
    value_t value_ ;

  public :
    inline 
    const value_t & 
    get() const 
    { return value_ ; 
    }

    inline
    void 
    put( const value_t & v ) 
    { value_ = v ;
    }
  } ;
  
  //--------------------------------------------------------------------------------
  template<typename T>
  struct 
  alignas( 32 )
  Align32
  {
  public :
    typedef T value_t ;

  private :
    value_t value_ ;

  public :
    inline 
    const value_t & 
    get() const 
    { return value_ ; 
    }

    inline
    void 
    put( const value_t & v ) 
    { value_ = v ;
    }
  } ;
  
}}}

#endif
