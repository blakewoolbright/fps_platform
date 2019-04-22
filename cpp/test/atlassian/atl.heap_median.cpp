#include <iostream> 
#include <queue>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdint>

class IHeap 
{
public :
  std::priority_queue<int32_t> l_ ; 
  std::priority_queue<int32_t, std::vector<int32_t>, std::greater<int32_t> > h_ ; 

  IHeap() {}

  int32_t median() const 
  { 
    if( l_.size() == h_.size() ) 
    {
      if( (l_.empty() && h_.empty()) ) 
        return 0 ; 
    
      if( !l_.empty() ) return l_.top() ;
      return h_.top() ;

      // : ((l_.top() + h_.top()) / 2) ;
    } 
    else if( l_.size() > h_.size() ) 
      return l_.top() ;
    else 
      return h_.top() ;
  }

  void put( int32_t val ) 
  {
    int32_t med = median() ;
    if( l_.size() == h_.size() ) 
    {
      if( val < med )
        l_.push( val ) ;
      else 
        h_.push( val ) ;
    }
    else if( l_.size() > h_.size() )
    {
      if( val < med ) 
      {
        h_.push( l_.top() ) ;
        l_.pop() ;
        l_.push( val ) ;
      }
      else 
        h_.push( val ) ;
    }
    else
    {
      if( val > med ) 
      {
        l_.push( h_.top() ) ;
        h_.pop() ;
        h_.push( val ) ;
      }
      else 
        l_.push( val ) ;
    }

  } 

} ;

int main( int argc, char * argv [] ) 
{
  IHeap ih ;
  // std::cout << "  Median: " << ih.median() << std::endl ;

  std::vector<int32_t> vec = { 1, 10, 2, 15, 22, 23, 1 } ;
  for( std::size_t idx = 0 ; idx < vec.size() ; ++idx ) 
  {
    std::cout << "[ put( " << vec[idx] << " ) ]" << std::endl ;
    ih.put( vec[idx] ) ;
    std::cout << "  Median: " << ih.median() << std::endl ;
    std::cout << "  ih.l_.size : " << ih.l_.size() << std::endl ;
    if( !ih.l_.empty() ) 
      std::cout << "  ih.l_.top  : " << ih.l_.top() << std::endl ;
  
    std::cout << "  ih.h_.size : " << ih.h_.size() << std::endl ;
    if( !ih.l_.empty() ) 
      std::cout << "  ih.h_.top  : " << ih.h_.top() << std::endl ;
  }
  
  return 0 ;
}
