#include <iostream>
#include <cstdlib>
#include <string>
#include <cstdint>
#include <vector>
#include <unistd.h>

//
// Validate fib request ( non-zero positive integers )
//
bool validate_arg( std::string arg ) 
{
  for( uint32_t idx = 0 ; idx < arg.length() ; ++idx ) 
  {
    if( !std::isdigit( static_cast<uint8_t>( arg[idx] ) ) ) 
    {
      std::cout 
        << "ERROR: Non-numeric character at position " << idx 
        << " in argument '" << arg << "'" 
        << std::endl ;
      return false ;
    }
  }
  
  return true ;
}

//
// Iterative/Recursive addition counters
//
uint32_t Adds_Iterative = 0 ;
uint32_t Adds_Recursive = 0 ;
  
// Test input values
std::vector<uint32_t> Test_Inputs  = { 0, 1, 2, 3, 4, 5, 6, 15, 30, 47} ;

// Expected output values
std::vector<uint32_t> Test_Outputs = { 0, 1, 1, 2, 3, 5, 8, 610, 832040, 2971215073 } ;

//
// Iterative fib calculation
//
uint32_t fib_iterative( uint32_t arg ) 
{
  // Degenerate cases  
  if( arg == 0 ) return 0 ;
  if( arg == 1 ) return 1 ;

  // Iterative calculation via for loop
  uint32_t fp2  = 0 ;
  uint32_t fp1  = 1 ;
  uint32_t fcur = 1 ;
  for( uint32_t idx = 1 ; idx < arg ; ++idx ) 
  {
    fcur = fp1 + fp2 ;
    ++Adds_Iterative ;
    
    fp2 = fp1 ;
    fp1 = fcur ;
  }
  
  return fcur ;
}

//
// Test for iterative fib implementation
//
bool test_fib_iterative()
{
  std::cout << "[ Test fib_iterative() ]" << std::endl ;
  
  // Test loop
  bool rv = true ;
  for( uint32_t idx = 0 ; idx < Test_Inputs.size() ; ++idx ) 
  {
    uint32_t fib_rv = fib_iterative( Test_Inputs[idx] ) ;
    if( fib_rv != Test_Outputs[ idx ] ) 
      rv = false ;

    std::cout 
      << "  fib_iterative( " << Test_Inputs[idx] << " ) => " << fib_rv 
      << " (adds: " << Adds_Iterative << ")" 
      << std::endl ;
    
    Adds_Iterative = 0 ;
  }
  if( rv )
    std::cout << "[ Pass ]" << std::endl << std::endl ;
  else 
    std::cout << "[ FAIL ]" << std::endl << std::endl ;
    
  return rv ;
}

//
// Recursive fib calculation
//
uint32_t fib_recursive( uint32_t arg )
{
  // Degenerate cases
  if( arg == 0 ) return 0 ;
  if( arg == 1 ) return 1 ;
  
  // Calculate via recursion
  uint32_t rv = fib_recursive( arg-1 ) + fib_recursive( arg-2 ) ;
  ++Adds_Recursive ;
  return rv ;
}

//
// Test for recursive fib implementation
//
bool test_fib_recursive()
{
  std::cout << "[ Test fib_recursive() ]" << std::endl ;

  // Test loop
  bool rv = true ;
  for( uint32_t idx = 0 ; idx < Test_Inputs.size() ; ++idx ) 
  {
    uint32_t fib_rv = fib_recursive( Test_Inputs[idx] ) ;
    if( fib_rv != Test_Outputs[ idx ] ) 
      rv = false ;

    std::cout 
      << "  fib_recursive( " << Test_Inputs[idx] << " ) => " << fib_rv 
      << "  (adds: " << Adds_Recursive << ")" 
      << std::endl ;

    Adds_Recursive = 0 ;
  }
  if( rv )
    std::cout << "[ Pass ]" << std::endl << std::endl ;
  else 
    std::cout << "[ FAIL ]" << std::endl << std::endl ;
    
  return rv ;
}

//
// Program entry point
//
int 
main( int argc, char * argv[] )
{
  if( (argc != 2) || (std::string(argv[1]) == "-h" || std::string( argv[1] ) == "--help" ) )
  {
    std::cout 
      << std::endl 
      << "USAGE" << std::endl 
      << "        " << argv[0] << " <desired fib #>  (print requested fib # to console)" << std::endl 
      << "        " << argv[0] << " test (run internal tests)" << std::endl 
      << "EXAMPLE" << std::endl
      << "        $> " << argv[0] << " 2" << std::endl
      << "        " << fib_iterative( 2 ) << std::endl 
      << std::endl ;
    
    return (argc != 2) ? EXIT_FAILURE : EXIT_SUCCESS ;
  }

  // Internal tests
  if( std::string( argv[1] ) == "test" ) 
  {
    if( !test_fib_iterative() ) 
      return EXIT_FAILURE ;

    if( !test_fib_recursive() ) 
      return EXIT_FAILURE ;
  
    return EXIT_SUCCESS ;
  }

  // Validate argument
  if( !validate_arg( argv[1] ) ) 
  {
    return EXIT_FAILURE ;
  }

  // Calculate and display result  
  uint32_t fib_req = std::strtoul( argv[1], nullptr, 10 ) ;
  if( fib_req > 48 ) 
  {
    std::cout 
      << "ERROR: The maximum fib series member representable via uint32_t is 48" << std::endl ;
    return EXIT_FAILURE ;
  }

  uint32_t fib_iterate = fib_iterative( fib_req ) ;
  uint32_t fib_recurse = fib_recursive( fib_req ) ;
  std::cout 
    << "iterative: " << fib_iterate << " (adds: " << Adds_Iterative << ")" << std::endl 
    << "recursive: " << fib_recurse << " (adds: " << Adds_Recursive << ")" << std::endl
    << std::endl ;

  return EXIT_SUCCESS ;
}

