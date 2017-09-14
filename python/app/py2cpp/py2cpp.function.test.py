#!/usr/bin/python 

import os, sys 

from fps import py2cpp  

def main() :

  func = py2cpp.Function( 'int', 'main'
                        , [ 'int argc', 'char * argv[]' ]
                        , body='return EXIT_SUCCESS ;' 
                        )
    
  print "[ Default Indention ]" 
  lines = [ py2cpp.Options.Indent_Width * ' ' + x for x in func.serialize() ]
  print '\n'.join( lines )
  print ""

  print "[ 2x Default Indention ]" 
  lines  = func.serialize( py2cpp.Options.Indent_Width * 2 ) 
  lines  = [ (py2cpp.Options.Indent_Width * ' ') + x for x in lines ] 
  print '\n'.join( lines )
  print ""

  print "[ Tabbed Indention ]" 
  py2cpp.Options.Indent_Width = 1
  py2cpp.Options.Indent_Char  = '\t'
  lines  = [ (py2cpp.Options.Indent_Width * '\t') + x for x in func.serialize() ] 
  print '\n'.join( lines )
  print ""
 
  return 0 

if __name__ == "__main__" :
  sys.exit( main() )

