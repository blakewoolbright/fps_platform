#!/usr/bin/python 

import os, sys 

from fps import py2cpp  

def main() :

  t1 = py2cpp .Type( 'uint64_t' )   
  t2 = py2cpp .Type( 'std::string' )
  t3 = py2cpp .Type( 'std::map<std::string, std::vector<std::string>>' ) 

  td1 = py2cpp .Typedef( t1, 'my_uint64_t' ) 
  td2 = py2cpp .Typedef( t2, 'my_string_t' ) 
  td3 = py2cpp .Typedef( t3, 'my_map_t' )
  
  print td1
  print td2
  print td3
 
  return 0 

if __name__ == "__main__" :
  sys.exit( main() )

