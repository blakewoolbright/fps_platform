#!/usr/bin/python 

import os, sys 

from fps import py2cpp  

def main() :

  v = py2cpp .Variable( 'const Object * obj' ) 
  t = py2cpp .Type    ( 'const char *' )
  print "[ %s -> %s ]"%( v.type().typename(), t.typename() )
  cast = py2cpp .Cast( v, t )
  print cast.to_string( 2 ) 
  print ''

  v = py2cpp .Variable( 'const Base    & base' ) 
  t = py2cpp .Type    ( 'const Derived & derived' )
  print "[ %s -> %s ]"%( v.type().typename(), t.typename() )
  cast = py2cpp .Cast( v, t )
  print cast.to_string( 2 ) 
  print ''

  v = py2cpp .Variable( 'const char * ptr' ) 
  t = py2cpp .Type    ( 'uint64_t' )
  print "[ %s -> %s ]"%( v.type().typename(), t.typename() )
  cast = py2cpp .Cast( v, t )
  print cast.to_string( 2 ) 
  print ''

  v = py2cpp .Variable( 'uint32_t value32' ) 
  t = py2cpp .Type    ( 'uint64_t' )
  print "[ %s -> %s ]"%( v.type().typename(), t.typename() )
  cast = py2cpp .Cast( v, t )
  print cast.to_string( 2 ) 
  print ''

  v = py2cpp .Variable( 'uint32_t array[32] ' ) 
  t = py2cpp .Type    ( 'const uint32_t *' ) # TODO: validate variable size
  print "[ %s -> %s ]"%( v.type().typename(), t.typename() )
  cast = py2cpp .Cast( v, t )
  print cast.to_string( 2 ) 
  print ''
 
  return 0 

if __name__ == "__main__" :
  sys.exit( main() )

