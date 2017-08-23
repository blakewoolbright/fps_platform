from Type import Type
import Parse

class Variable :
  
  def __init__( self, cpp_decl ) :
    self._type = Type( cpp_decl )
    self._name = self.__parse_name( cpp_decl ) 
    
    # print 'self._name: ' + str(self._name)
    
    if self._name == '' or self._name == self._type.typeroot :
      raise Exception( "Variable: No variable name found in '%s'"%cpp_decl )
  
  def type( self ) : 
    return self._type
  
  def name( self ) :
    return self._name 

  def serialize( self, indent=0 ) :
    return self._type.declare( self.name(), indent )   

  def __parse_name( self, cpp_decl ) :
    cpp_decl = Parse.strip_declaration( cpp_decl ) 

    # Skip array [] suffix
    if self._type.is_array() :
      tmp_pos = cpp_decl.rfind( '[' ) 
      if tmp_pos < 0 :
        raise Exception( "Variable.__parse_name: No '[' found for array '%s'"%cpp_decl )

      cpp_decl = cpp_decl[ 0 : tmp_pos ] 
    elif self._type.is_pointer() :
      tmp_pos = cpp_decl.rfind( '*' ) 
      if tmp_pos < 0 :
        raise Exception( "Variable.__parse_name: No '*' found in pointer '%s'"%cpp_decl )
      cpp_decl = cpp_decl[ tmp_pos : ].strip( '*' )
    elif self._type.is_reference() :
      tmp_pos = cpp_decl.rfind( '&' ) 
      if tmp_pos < 0 :
        raise Exception( "Variable.__parse_name: No '&' found in reference '%s'"%cpp_decl )
      cpp_decl = cpp_decl[ tmp_pos : ].strip( '&' )
    
    parts = cpp_decl.split( ' ' )
    return parts[-1].strip()
    
  def __str__( self ) :
    return "Variable : { " + self.serialize() + " }"
