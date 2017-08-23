#!/usr/bin/python 

import os, sys 

from fps import cppgen 

def main() :

  v = cppgen.Variable( 'const Object * obj' ) 
  t = cppgen.Type    ( 'const char *' )
  print "[ %s -> %s ]"%( v.type().typename(), t.typename() )
  cast = cppgen.Cast( v, t )
  print cast.to_string( 2 ) 
  print ''

  v = cppgen.Variable( 'const Base    & base' ) 
  t = cppgen.Type    ( 'const Derived & derived' )
  print "[ %s -> %s ]"%( v.type().typename(), t.typename() )
  cast = cppgen.Cast( v, t )
  print cast.to_string( 2 ) 
  print ''

  v = cppgen.Variable( 'const char * ptr' ) 
  t = cppgen.Type    ( 'uint64_t' )
  print "[ %s -> %s ]"%( v.type().typename(), t.typename() )
  cast = cppgen.Cast( v, t )
  print cast.to_string( 2 ) 
  print ''

  v = cppgen.Variable( 'uint32_t value32' ) 
  t = cppgen.Type    ( 'uint64_t' )
  print "[ %s -> %s ]"%( v.type().typename(), t.typename() )
  cast = cppgen.Cast( v, t )
  print cast.to_string( 2 ) 
  print ''

  v = cppgen.Variable( 'uint32_t array[32] ' ) 
  t = cppgen.Type    ( 'const uint32_t *' ) # TODO: validate variable size
  print "[ %s -> %s ]"%( v.type().typename(), t.typename() )
  cast = cppgen.Cast( v, t )
  print cast.to_string( 2 ) 
  print ''
 
  return 0 

if __name__ == "__main__" :
  sys.exit( main() )

