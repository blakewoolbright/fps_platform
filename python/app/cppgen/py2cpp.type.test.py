#!/usr/bin/python 

import os, sys 

from fps import cppgen 

def main() :
  decl_list = [ ('int32_t var ;', 'int32_t')
              , ('uint32_t var ;', 'uint32_t') 
              , (' const uint32_t var ;', 'uint32_t')
              , ('\tconst char * const var ;', 'char')
              , ('const\tchar ** var ;', 'char')
              , ('int64_t & var ;\n', 'int64_t')
              , ('uint8_t var[16] ;', 'uint8_t')
              , ('std::vector<std::string>', 'std::vector<std::string>')
              , ('std::vector< std::string,\tabcd  >', 'std::vector<std::string, abcd>') 
              , ('long long', 'int64_t')
              , ('unsigned long & var ;', 'uint64_t')
              , ('UserStruct', 'UserStruct')
              ] 
  
  
  for entry in decl_list :
    print "-----------------------------------------------------"
    var_decl      = entry[0] 
    expected_root = entry[1]
    test_type     = cppgen.Type( var_decl, verbose=True ) 
    if test_type.typeroot() != expected_root :
      raise Exception( "Expected typeroot of '%s' for '%s'"%( expected_root, var_decl ) )
 
  return 0 

if __name__ == "__main__" :
  sys.exit( main() )

