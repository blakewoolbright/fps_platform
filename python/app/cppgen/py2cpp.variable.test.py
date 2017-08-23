#!/usr/bin/python 

import os, sys 

from fps import cppgen 

def main() :
  
  vars = [ 'uint32_t v1 ;'
         , 'const char * v2 ;'
         , 'const char ** v3 ;'
         , 'const std::vector<std::string> & v4'
         , 'char v5[16] ;'
         , 'const char * const v6 ;'
         ]
         
  for var in vars :
    print "[ var = cppgen.Variable( '%s' ) ]"%(var)
    var = cppgen.Variable( var )
    print '  var.serialize()     : ' + var.serialize()
    var.type().describe()
    print ''
 
  return 0 

if __name__ == "__main__" :
  sys.exit( main() )

