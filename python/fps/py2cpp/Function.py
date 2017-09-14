import Options
from Type import Type, Conversion
from Variable import Variable

class Function :

  def __init__( self, return_type, name, args=[], attributes=[], body = None ) :
    if return_type is None or (isinstance( return_type, str ) and str == '') :
      raise Exception( "Function: Constructed with empty/null return type" ) 
    
    if not isinstance( name, str ) or len(name) == 0 :
      raise Exception( "Function: Constructed with empty/null 'name' argument" ) 
    
    # If return type is a string, convert to Type instance
    if isinstance( return_type, str ) :
      return_type = Type( return_type ) 

    # Set defaults 
    self.clear() 
    
    # Set function basics
    self._name      = name 
    self._rt        = return_type 

    # Set function attributes
    self._is_static = True if 'static' in attributes else False 
    self._is_inline = True if 'inline' in attributes else False
    self._is_const  = True if (not self._is_static and 'const' in attributes) else False 
    self._is_proto  = True if body is None or body is '' else False

    # Set function arguments
    self._args = []
    for arg in args :
      if isinstance( arg, str ) :
        arg = Variable( arg ) 
      if not isinstance( arg, Variable ) :
        raise Exception( "Malformed argument to Function constructor '%s'", str(arg) )
      self._args.append( arg )  
    
    # Set function body
    if body :
      self.set_body( body ) 
    
    # Sanity check instance
    self.sanity_check() 

  def clear( self ) :
    self._return = None 
    self._name   = None
    self._body   = []
    self._args   = []

  def sanity_check( self ) :
    if self.is_static() and self.is_const() :
      raise Exception( "Function.sanity_check() : Invalid attributes (static and const)" )
    
    if self.name() is None or self.name() == "" :
      raise Exception( "Function.sanity_check() : No name assigned to instance" )
    
    if not isinstance( self.return_type(), Type ) :
      raise Exception( "Function.sanity_check() : Malformed return type (not Type instance)" )

  def set_body( self, body=None ) : 
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

  def return_type ( self ) : return self._rt 
  def name        ( self ) : return self._name 
  def args        ( self ) : return self._args 
  def body        ( self ) : return self._body
  def is_inline   ( self ) : return self._is_inline 
  def is_static   ( self ) : return self._is_static
  def is_const    ( self ) : return self._is_const
  def is_prototype( self ) : return self._is_prototype
    
  def __str__( self ) :
    args = [ arg.serialize() for arg in self._args ]
    args = ', '.join( args )
    return 'Function : { name: %s, rt: %s, args: [ %s ], const: %s, inline: %s, proto: %s }'\
           %( self.name(), self.return_type(), args, self._is_const, self._is_inline, self._is_proto )

  def serialize( self, indent=2 ) :
    
    f_indent  = ' ' * indent 
    f_name    = self.name() 

    f_prefix  = ''
    f_prefix += 'inline ' if self.is_inline() else '' 
    f_prefix += 'static ' if self.is_static() else ''
    if len( f_prefix ) : f_prefix += ' ' 
    
    f_args    = []
    for arg in self.args() :
      f_args.append( arg.serialize() ) 
    f_args = '' if not f_args or len(f_args) == 0 else ', '.join( f_args ) 

    out = ''
    if not self.body() or (len( self.body() ) == 0) :
      out  = f_prefix + self.return_type().typename() + ' ' + f_name + '( ' + f_args + ' )' 
      if self.is_const() :
        out += ' const' 
      out += ' ;'
      return [ out ]

    else :
      out += f_prefix + self.return_type().typename() + ' ' + f_name + '( ' +  f_args + ' )'
      
      if self.is_const() :
        out += ' const' 
      
      cpp  = [ out, '{' ]
      cpp.extend( [ ((f_indent * 2) + x) for x in self.body() ] )
      cpp.append( '}' )
      
      return cpp
        
      
        
      

    
    
    
  
    

    
  
