from Type import Type, Conversion
from Variable import Variable

class Function :

  def __init__( self, return_type, name, args=[], body = None ) :
    if return_type is None or (isinstance( return_type, str ) and str == '') :
      raise Exception( "Function: Constructed with empty/null return type" ) 
    
    if not isinstance( name, str ) or len(name) == 0 :
      raise Exception( "Function: Construction with empty/null name" ) 
    
    if isinstance( return_type, str ) :
      return_type = Type( return_type ) 

    self.__clear() 
    self._name = name 
    self._rt   = return_type 
    self._args = args 
    if body :
      self._body = body.split( '\n' ) 
    else :
      self._body = [] 

  def __clear( self ) :
    self._return = None 
    self._name   = None
    self._body   = []
    self._args   = []

  def return_type( self ) :
    return self._rt 
  
  def name( self ) :
    return self._name 
  
  def args( self ) :
    return self._args 
    
  def __str__( self ) :
    return 'Function : { name: %s, rt: %s, args: %s }'\
           %( self.name(), self.return_type(), str(self.args()) )
  
