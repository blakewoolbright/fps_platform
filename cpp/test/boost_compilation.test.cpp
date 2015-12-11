#include <iostream>
#include <boost/program_options.hpp> 

namespace po = boost::program_options ;

int main( int argc, char * argv[] )
{ 
  std::cout << "Boost Compilation Test" << std::endl << std::endl ;

  po::options_description opt_desc( "Options" ) ;
  opt_desc.add_options()
                      ( "help,h",    "Display command line help" ) 
                      ( "verbose,v", "Enable verbose output" )
                      ;

  po::variables_map vm;
  po::store ( po::parse_command_line( argc, argv, opt_desc ), vm ) ;
  po::notify( vm ) ;  

  if( vm.count( "help" ) )
  { std::cout << opt_desc << std::endl << std::endl ;
    return 0 ;
  }



  return 0 ;
}

