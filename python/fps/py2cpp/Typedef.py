from Type import Type

class Typedef :
  def __init__( self, aliased_type, alias_name ) :

    if type(aliased_type) is str :
      self._aliased_type= Type( aliased_type ) 
    else :
      self._aliased_type = aliased_type 
    self._type = Type( alias_name )
  
  def typename( self ) :
    return self._type.typename()

  def typeroot( self ) :
    return self._type.typeroot()

  def type( self ) :
    return self._type 

  def aliased_type( self ) :
    return self._aliased_type

  def aliased_typename( self ) :
    return self._aliased_type.typename()
  
  def serialize( self ) :
    return [ 'typedef %s %s ;'%( self.aliased_typename(), self.typename() ) ]
  
  def to_string( self, indent=0 ) :
    indent=indent * ' '
    return "%sTypedef : { name: '%s', aliased_type='%s' }" \
           %( indent, self.typename(), self.aliased_typename() )
  
  def __str__( self ) : 
    return self.to_string( 0 ) 
  
  

