#!/usr/bin/python 

import os, sys 

from fps import cppgen 

def main() :

  t1 = cppgen.Type( 'uint64_t' )   
  t2 = cppgen.Type( 'std::string' )
  t3 = cppgen.Type( 'std::map<std::string, std::vector<std::string>>' ) 

  td1 = cppgen.Typedef( t1, 'my_uint64_t' ) 
  td2 = cppgen.Typedef( t2, 'my_string_t' ) 
  td3 = cppgen.Typedef( t3, 'my_map_t' )
  
  print td1
  print td2
  print td3
 
  return 0 

if __name__ == "__main__" :
  sys.exit( main() )

