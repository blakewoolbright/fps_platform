#!/usr/bin/python 

import os, sys 

from fps import py2cpp  

def main() :

  funcs = [ py2cpp.Function( 'int', 'main', [ 'int argc', 'char * argv[]' ] )
          , py2cpp.Function( 'int', 'main', [ 'int argc', 'char * argv[]' ] )
          ]
  funcs[0].set_body( 'return EXIT_SUCCESS ;' )
    
  for func in funcs :
    print str(func)
    print '\n'.join( func.serialize() )
    print ""
 
  return 0 

if __name__ == "__main__" :
  sys.exit( main() )

