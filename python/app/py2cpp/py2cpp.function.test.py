#!/usr/bin/python 

import os, sys 

from fps import py2cpp  

def main() :
  
  main = py2cpp .Function( 'int', 'main', [ 'int argc', 'char * argv[]' ] )
  print str(main)
 
  return 0 

if __name__ == "__main__" :
  sys.exit( main() )

