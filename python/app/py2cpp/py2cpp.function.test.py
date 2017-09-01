#!/usr/bin/python 

import os, sys 

from fps import py2cpp  

def main() :
  
  main = py2cpp .Function( 'int', 'main', [ 'int argc', 'char * argv[]' ] )
  print str(main)
  print '\n'.join( main.serialize() )
    
  main.set_body( 'return EXIT_SUCCESS ;' )
  print str(main)
  print '\n'.join( main.serialize() )
 
  return 0 

if __name__ == "__main__" :
  sys.exit( main() )

