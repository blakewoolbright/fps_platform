#!/usr/bin/python 

import os, sys 

from fps import cppgen 

def main() :
  
  main = cppgen.Function( 'int', 'main', [ 'int argc', 'char * argv[]' ] )
  print str(main)
 
  return 0 

if __name__ == "__main__" :
  sys.exit( main() )

