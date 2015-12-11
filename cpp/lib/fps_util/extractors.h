#ifndef FPS__UTIL__EXTRACT__H
#define FPS__UTIL__EXTRACT__H

#include <stdint.h>

namespace fps  {
namespace extract {

  //----------------------------------------------------------------------------------
  struct ByNewline 
  {
    static
    const char *
    get( const char * begin, uint32_t len )
    {
      if( begin == NULL || len == 0 )
        return NULL ;

      const char * end = begin + len ;
      while( begin != end )
      {
        if( *begin == '\n' )
          return begin + 1 ;
        ++begin ;
      }
      return NULL ;
    }
  } ;

  //----------------------------------------------------------------------------------
  template<typename T>
  struct ByType
  {
    static
    const char *
    get( const char * begin, uint32_t len )
    {
      return ( begin == NULL || len == 0 || len < sizeof( T ) )
             ? NULL
             : begin + sizeof( T ) + 1
             ;
    }
  } ;

  //----------------------------------------------------------------------------------
  struct BySize
  {
  private :
    uint32_t sz_ ;
    BySize() ;

  public :

    //--------------------------------------------------------
    inline BySize( uint32_t sz ) : sz_( sz ) {}

    //--------------------------------------------------------
    inline
    const char *
    get( const char * begin, uint32_t len )
    {
      return ( begin == NULL || len == 0 || len < sz_ )
             ? NULL
             : begin + sz_ + 1
             ;
    }
  } ;

}}

#endif
