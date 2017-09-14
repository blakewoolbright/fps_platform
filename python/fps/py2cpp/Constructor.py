import Options
from Type import Type, Conversion
from Variable import Variable

class Constructor :

  Attribute_Tags = [ 'inline', 'explicit', 'virtual', 'pure' ]
  """Valid attribute tag definitions"""

  def __init__( self, name, args=[], attrs=[], initializers=[], body=None ) :
    
    if not isinstance( name, str ) or len(name) == 0 :
      raise Exception( "Constructor: Initialized with empty/null 'name' argument" ) 

    # If attrs is a string, convert to list
    if isinstance( attrs, str ) :
      attrs = [ attrs ] 
    
    # Initialize w/ defaults
    self.clear() 
    self._attrs = [ x.lower().strip() for x in attrs ]

    # Init attributes
    self._is_explicit = True if 'explicit' in self._attrs else False 
    self._is_virtual  = True if 'virtual'  in self._attrs else False
    self._is_inline   = True if 'inline'   in self._attrs else False
    self._is_pure     = True if 'pure'     in self._attrs else False
    
    # Set function arguments
    self._args = []
    for arg in args :
      if isinstance( arg, str ) :
        arg = Variable( arg ) 
      if not isinstance( arg, Variable ) :
        raise Exception( "Malformed argument to Function constructor '%s'", str(arg) )
      self._args.append( arg )  
    
    # Sanity check before returning
    self.sanity_check() 

  def clear( self ) :
    """Reset to defaults"""
    self._return  = None 
    self._name    = None
    self._body    = []
    self._args    = []
    self._attrs   = []

  def sanity_check( self ) :
    """Sanity check instance"""
    if self.is_static() and self.is_const() :
      raise Exception( "Function.sanity_check() : Invalid attribute combo (static and const)" )
    
    if self.name() is None or self.name() == "" :
      raise Exception( "Function.sanity_check() : No name assigned to instance" )
    
    if not isinstance( self.return_type(), Type ) :
      raise Exception( "Function.sanity_check() : Malformed return type (not Type instance)" )
    
    for attr in self._attrs :
      if not attr in Attribute_Tags :
        raise Exception( "Function.sanity_check() : Unknown attribute '%s'"%(attr) )

  def set_body( self, body=None ) : 
    """Assign a new body to the function"""
    if not body :
      self._body = None 
      self._is_proto = True
      return self

    if isinstance( body, str ) :
      self._body = body.split( '\n' )
    elif isinstance( body, list ) :
      self._body = body 
    else :
      raise Exception( "Function.set_body() : Unsupported argument type (Must be string or list)" )
    
    self._is_proto = False
    return self

  # Misc getter functions
  def return_type ( self ) : return self._rt 
  def name        ( self ) : return self._name 
  def args        ( self ) : return self._args 
  def body        ( self ) : return self._body
  def is_inline   ( self ) : return self._is_inline 
  def is_static   ( self ) : return self._is_static
  def is_const    ( self ) : return self._is_const
  def is_prototype( self ) : return self._is_proto
  def is_ctor     ( self ) : return False 
  def is_function ( self ) : return True  
