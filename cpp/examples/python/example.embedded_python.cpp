#include <Python.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>

int main( int argc, char * argv[] ) 
{
  uint32_t verbose   = 0 ;
  bool     show_help = false ; 
  for( int idx = 1 ; idx < argc ; ++idx ) 
  {
    std::string arg( argv[idx] ) ;
    if( arg == "-h" || arg == "--help" ) 
    {
      show_help = true ;
      continue ;
    }

    if( arg == "-v" || arg == "--verbose" )
    {
      ++verbose ;
      continue ;
    }
  }

  if( show_help || (argc < 3) )
  {
    std::cout 
      << std::endl 
      << "  SYNOPSIS" << std::endl 
      << "           This program loads <external python file> in an embedded interpreter, " << std::endl
      << "           runs <external function/command>, and displays the output." << std::endl
      << std::endl
      << "  USAGE" << std::endl
      << "           " << argv[0] << " <external python file> <external function/command>" << std::endl 
      << std::endl
      << "  OPTIONS" << std::endl 
      << "           -h/--help    Display this message" << std::endl 
      << "           -v/--verbose Increase verbosity" << std::endl 
      << std::endl ;
    
    return show_help ? EXIT_SUCCESS : EXIT_FAILURE ;
  }

  return EXIT_SUCCESS ;
}

