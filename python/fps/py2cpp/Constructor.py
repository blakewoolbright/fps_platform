import Options
from Type import Type, Conversion
from Variable import Variable

class Constructor :

  Attribute_Tags = [ 'inline', 'explicit', 'virtual', 'abstract' ]
  """Valid attribute tag definitions"""

  def __init__( self, name, args=[], attrs=[], initializers=[], body=None ) :
    
    if not isinstance( name, str ) or len(name) == 0 :
      raise Exception( "Constructor: Initialized with empty/null 'name' argument" ) 

    # If attrs is a string, convert to list
    if isinstance( attrs, str ) :
      attrs = [ attrs ] 
    
    # Initialize w/ defaults
    self.clear() 
    self._name  = name 
    self._attrs = [ x.lower().strip() for x in attrs ]

    # Init attributes
    self._is_explicit = True if 'explicit' in self._attrs else False 
    self._is_virtual  = True if 'virtual'  in self._attrs else False
    self._is_inline   = True if 'inline'   in self._attrs else False
    self._is_abstract = True if 'abstract' in self._attrs else False
    
    # Set function arguments
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
    self._return      = None 
    self._name        = None
    self._body        = []
    self._args        = []
    self._attrs       = []
    self._is_explicit = False
    self._is_virtual  = False
    self._is_inline   = False
    self._is_abstract = False

  def sanity_check( self ) :
    """Sanity check instance"""
    
    if self.name() is None or self.name() == "" :
      raise Exception( "Function.sanity_check() : No name assigned to instance" )
    
    for attr in self._attrs :
      if not attr in self.Attribute_Tags :
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
  def name        ( self ) : return self._name 
  def args        ( self ) : return self._args 
  def body        ( self ) : return self._body
  def is_inline   ( self ) : return self._is_inline 
  def is_const    ( self ) : return self._is_const
  def is_virtual  ( self ) : return self._is_virtual
  def is_abstract ( self ) : return self._is_abstract
  def is_prototype( self ) : return self._is_proto
  def is_ctor     ( self ) : return False 
  def is_function ( self ) : return True  

  def serialize( self, indent=0, proto=False ) :
    """
    Serialize to c++ with the given indentation.  
    If 'proto' arg is False, then the full function definition will be
    serialized (if possible).  If 'proto' is True, then only 
    a function prototype will be serialized.
    """
    indent    = Options.Indent_Width if indent == 0 else indent
    f_name    = self.name() 
    f_prefix  = ''
    f_prefix += 'inline ' if self.is_inline() else '' 
    f_prefix += 'virtual ' if self.is_virtual() else ''

    f_args = []
    for arg in self.args() :
      f_args.append( arg.serialize() ) 
    f_args = '' if not f_args or len(f_args) == 0 else ', '.join( f_args ) 

    out = ''
    if proto or not self.body() :
      f_suffix = ' = 0' if self.is_abstract() else '' 
      out = f_prefix + f_name + '(' 
      if len( f_args ) :
        out += ' ' + f_args + ' '
      out += ')' +  f_suffix + ' ;'
      out = [ out ]

    else :
      f_indent = Options.Indent_Char * indent 
      out += f_prefix + f_name + '('
      if len( f_args ) :
        out += ' ' + f_args + ' '
      out += ')'
      
      cpp = [ out, '{' ]
      cpp.extend( [ (f_indent + x) for x in self.body() ] )
      cpp.append( '}' )
      out = cpp
      
    return out 



