
def strip_declaration( var_decl ) :
  """Strips whitespace, trailing ';', and adds expected space padding to variable/type declaration"""
  
  # Strip leading/trailing whitespace 
  var_decl = var_decl.replace( '\t', ' ' ) 
  var_decl = var_decl.replace( '\n', ' ' ) 
  var_decl = var_decl.strip()
  
  # String ';' delimiter
  var_decl = var_decl.rstrip( ';' )

  # Pad '*' and '&' characters so they are delimited by spaces
  var_decl = var_decl.replace( '*', ' * ' ).replace( '&', ' & ' ) 
  var_decl = var_decl.replace( '  ', ' ' ) 

  return var_decl.strip()

def normalize_declaration( type_decl ) :
  """Given a c++ type/variable declaration, return the equivalent in normalized form"""
  
  # Strip unnecessary characters and get declaration into shape for next step
  type_decl = strip_declaration( type_decl ) 

  # Make sure one space prefixes and suffixes declaration
  type_decl = ' ' + type_decl + ' '

  # Replace various multi-word types w/ single word equivalent
  type_decl = type_decl.replace( " long long ",      " long " ) ;
  type_decl = type_decl.replace( " unsigned long ",  " uint64_t " )
  type_decl = type_decl.replace( " long unsigned ",  " uint64_t " )
  type_decl = type_decl.replace( " unsigned int ",   " uint32_t " ) 
  type_decl = type_decl.replace( " int unsigned ",   " uint32_t " ) 
  type_decl = type_decl.replace( " unsigned short ", " uint16_t " )
  type_decl = type_decl.replace( " short unsigned ", " uint16_t " )
  type_decl = type_decl.replace( " unsigned char ",  " uint8_t " ) 
  type_decl = type_decl.replace( " char unsigned ",  " uint8_t " ) 
  type_decl = type_decl.replace( " signed long ",    " int64_t " ) 
  type_decl = type_decl.replace( " long signed ",    " int64_t " ) 
  type_decl = type_decl.replace( " signed int ",     " int32_t " ) 
  type_decl = type_decl.replace( " int signed ",     " int32_t " ) 
  type_decl = type_decl.replace( " signed char ",    " int8_t " ) 
  type_decl = type_decl.replace( " char signed ",    " int8_t " ) 
  type_decl = type_decl.replace( " long ",           " int64_t " )
  type_decl = type_decl.replace( " unsigned ",       " uint32_t " ) 
  type_decl = type_decl.replace( " signed ",         " int32_t " ) 
  type_decl = type_decl.replace( " ulong",           " uint64_t " ) 
  type_decl = type_decl.replace( " ushort",          " uint16_t " ) 
  type_decl = type_decl.replace( " byte",            " uint8_t " ) 

  # Eliminate extra whitespace
  type_decl = type_decl.replace( '  ', ' ' ) 

  return type_decl
