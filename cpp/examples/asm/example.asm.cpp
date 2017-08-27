#include <cstdlib>
#include <iostream>

int main( int argc, char * argv[] ) 
{
  int32_t foo = 10 ; 
  int32_t bar = 15 ;

  __asm__ __volatile__
  ( "addl  %%ebx,%%eax"
  : "=a"(foo)
  : "a"(foo), "b"(bar)
  ) ;

  printf( "foo+bar=%d\n", foo ) ;

  return 0 ;
}
