import Parse, Options

#
# The Metrics class stores name and size information for basic c++ types
#
class Metrics :
  """
  SYNOPSIS :
    The 'py2cpp.Metrics' class stores the name and size(in bytes) of a given type.
  """ 

  def __init__( self, name, size ) :
    """
    SYNOPSIS :
      Metrics.__init__() constructs a new Metrics object given a 
      name and a size argument.

    ARGUMENTS
      name : Name of referenced datatype.
      size : Size in bytes of this datatype when compiled (x64).
    """
    self._name = name
    self._size = int(size)
    self._is_signed = not (name.startswith( 'uint' ) or name.startswith( 'unsigned' ))

  def name( self ) :
    '''Return the c++ name of this type'''
    return self._name

  def size( self ) :
    '''Return the compiled size (in bytes) of this type as an integer.'''
    return self._size

  def is_signed( self ) :
    '''Return True if this is an signed type, False otherwise'''
    return self._is_unsigned

  def __str__( self ) :
    '''Automatic string conversion support (for use w/ print, etc)'''
    return "Metrics: { name='%s', size=%d }"%(self._name, self._size)

#
# The 'Primitives' dict stores Type.Metrics for c++ primitive types.
#
Primitives = \
{ 'char'     : Metrics( 'char',         1 )
, 'int8_t'   : Metrics( 'int8_t',       1 )
, 'uint8_t'  : Metrics( 'uint8_t',      1 )
, 'int16_t'  : Metrics( 'int16_t',      2 )
, 'uint16_t' : Metrics( 'uint16_t',     2 )
, 'int32_t'  : Metrics( 'int32_t',      4 )
, 'uint32_t' : Metrics( 'uint32_t',     4 )
, 'float'    : Metrics( 'float',        4 )
, 'int64_t'  : Metrics( 'int64_t',      8 )
, 'uint64_t' : Metrics( 'uint64_t',     8 )
, 'double'   : Metrics( 'double',       8 )
, 'enum'     : Metrics( 'enum',         4 )
}

#
# The 'Type' class parses basic c++ type declaration statements, and stores 
# relevent information like name, size, dimension, indirection, constness, 
# static/volatile qualifiers, encapsulating namespace(s), etc.
# 
class Type :
  '''
  SYNOPSIS :
    'py2cpp.Type' parses basic c++ type declaration statements and exposes 
    the type's name, size, dimension, indirection, constness, and other 
    attributes.
  '''

  Metrics = Metrics  
  '''Alias the py2cpp.Metrics class for easy syntax.'''
  
  Primitives = Primitives 
  '''Alias the py2cpp.Primitives dict for easy syntax.'''
  
  def __init__( self, cpp_decl ) :
    """
    SYNOPSIS :
      py2cpp.Type constructor - Creates a new Type object from a 
      c++ type declaration 
      statement.

    ARGUMENTS
      decl    : C++ type declaration
    """
    
    # Call __parse to populate internal variables
    self.__parse( cpp_decl )

  def __clear( self ) :
    '''
    SYNOPSIS 
      The Type.__clear() function resets all member variables to defaults.

    PRIVATE 
    '''
    self._decl_prefix    = ''
    self._decl_suffix    = ''
    self._typeroot       = None
    self._sizeof         = None
    self._is_primitive   = False
    self._is_pointer     = False
    self._is_reference   = False
    self._is_enum        = False
    self._is_struct      = False
    self._is_template    = False # For template types
    self._is_const       = False # For constant data 
    self._is_const_alias = False # For constant aliases to non-const data (eg. char * const)
    self._is_const_expr  = False # For constant expressions
    self._is_mutable     = False # For variables explicitly qualified as mutable
    self._is_volatile    = False # For variables explicitly qualified as volatile
    self._is_static      = False # For variables explicitly qualified as static
    self._array_size     = 0
    self._indirections   = 0     # Stores the number of indirections associated a pointers/reference
    self._namespaces     = []
  
  def typeroot( self ) : 
    '''Returns the root type (unmodified)'''
    return self._typeroot 
  
  def typename( self ) : 
    '''Returns the full type name for this type'''
    if len( self._decl_suffix ) :
      return self._decl_prefix + self._decl_suffix
    else :
      return self._decl_prefix 
  
  def decl_prefix( self ) :
    '''Returns c++ declaration prefix for this type'''
    return self._decl_prefix 

  def decl_suffix( self ) :
    '''Returns c++ declaration suffix for this type'''
    return self._decl_suffix 
    
  def declare( self, var_name, indent=0 ) :
    '''Return c++ declaration for variable of this type named "var_name"'''
    if not isinstance( var_name, str ) :
      raise Exception( "Type.declare: Invoked with non-string 'var_name' argument" )

    if not var_name:
      raise Exception( "Type.declare: Invoked with empty 'var_name' argument" )

    rv = self.serialize( indent ) + ' ' + var_name
    if self._decl_suffix :
      rv += self._decl_suffix 
    return rv

  def sizeof( self ) :
    '''Returns the compiled size of this type in bytes.'''
    return self._sizeof
  
  def is_const( self ) : 
    '''Returns True for const types'''
    return self._is_const 

  def is_const_expr( self ) : 
    '''Returns True for const types'''

  def is_primitive( self ) :
    '''Returns True for primitive types.'''
    return self._is_primitive

  def is_pointer( self ) :
    '''Returns True for pointer (*) types.'''
    return self._is_pointer

  def is_reference( self ) :
    '''Returns True for reference (&) types.'''
    return self._is_reference
  
  def indirections( self ) :
    '''Returns the number of pointer/reference indirections'''
    return self._indirections 

  def is_static( self ) :
    '''Returns True if this type was declared 'static'.'''
    return self._is_static

  def is_enum( self ) :
    '''Returns True for enumeration types.'''
    return self._is_enum

  def is_struct( self ) :
    '''Returns True for struct/class types.'''
    return self._is_struct

  def is_template( self ) :
    '''Returns True for template types.'''
    return self._is_template

  def is_const( self ) :
    '''Returns True if this type was declared 'const'.'''
    return self._is_const

  def is_alias( self ) :
    '''Returns True for pointer and reference types and False for arrays/etc.'''
    return self._is_pointer or self._is_reference

  def is_const_alias( self ) :
    '''Returns True for const pointer and reference types that refer to non-const data.'''
    return self._is_const_alias

  def is_mutable( self ) :
    '''Returns True if this type was declared 'mutable'.'''
    return self._is_mutable

  def is_volatile( self ) :
    '''Returns True if this type was declared 'volatile'.'''
    return self._is_volatile

  def is_array( self ) :
    '''Returns True for array types.'''
    return self._array_size != 0

  def array_size( self ) :
    '''Returns the associated size if this type is an array, or -1 for scalars.'''
    return self._array_size
  
  def indirection( self ) :
    '''Returns the number of times a pointer type must be dereferenced to retrieve the associated value.'''

  def namespaces( self ) :
    '''Return a list namespaces that encapsulate this type.'''
    return self._namespaces
  
  def __compose_typeroot( self, cpp_decl ) :
    '''
    Given a c++ type declaration, return the list of words that make up the root 
    type.  The root type can then be examined to determine attributes like compiled size.

    For example: 
      Given: 
        'static constexpr uint32_t example_variable_ ;'
      
      Return 
        [ 'uint32_t' ]
    '''

    rv = [] 
    for word in cpp_decl.split() :
      # Filter out c++ type-modifier keywords, but store attributes for later.
      if   word == 'mutable'   : self._is_mutable    = True
      elif word == 'static'    : self._is_static     = True
      elif word == 'volatile'  : self._is_volatile   = True
      elif word == 'struct'    : self._is_struct     = True
      elif word == 'constexpr' : self._is_const_expr = True
      elif word == 'const'     : 
        # If the 'const' comes after we've already seen a '*' or '&' then
        # this is a constant alias (can't refer to anything else)
        if self._is_pointer or self._is_reference :
          self._is_const_alias = True 
        else :
          self._is_const = True 

      elif word == '*' : 
        if self._is_reference :
          raise Exception( 'Type.__compose_typeroot: Reference-to-pointer is unsupported (%s)'%(cpp_decl) )

        self._is_pointer    = True 
        self._indirections += 1
      elif word == '&' : 
        if self._is_pointer :
          raise Exception( 'Type.__compose_typeroot: Pointer-to-reference is unsupported (%s)'%(cpp_decl) )

        self._is_reference  = True 
        self._indirections += 1

        if self._indirections > 1 :
          raise Exception( "Type.__compose_typeroot: RValue references are unsupported (%s)"%(cpp_decl) )

      elif word == 'enum' : 
        self._is_enum      = True
        self._is_primitive = True # TODO: Are 'enum class' declarations primitive?

      else :
        # If we don't have a return value yet, identify any encapsulating namespaces
        # prefixed to the current word, and save the word as our return value.  
        # Continue parsing so we collect any suffixed keywords before returning.
        if not rv :
          # Check non-modifiers for namespace prefixes and update 
          # the list of encapsulating namespaces
          word_ns = word.split( '::' )
          if len( word_ns ) > 1 : 
            self._namespaces.extend( word_ns[ 0:-1 ] )
          rv.append( word )
        # If we get back to this case suffixed keywords are now parsed, so return
        else :
          break 
    return rv 

  def __parse( self, cpp_decl ) :
    '''
    SYNOPSIS 
      The Type.__parse() function examines a c++ type declaration statement and sets
      relevent member variables for this instance accordingly.
    
    PRIVATE 
    '''
    # Clear member variables
    self.__clear()  

    # Normalize declaration syntax
    cpp_decl = Parse.normalize_declaration( cpp_decl ) 
    
    #
    # Array Detection
    #
    # Search for an opening '[' character to identify probable arrays
    open_bracket_pos = cpp_decl.find( '[' )
    if open_bracket_pos > 0 :
      close_bracket_pos = cpp_decl.rfind( ']' )

      # Require a closing bracket 
      if close_bracket_pos == -1 :
        raise Exception( "Type.__parse: No closing bracket for array : '%s'"%(cpp_decl) )

      # Extract array size
      tmp_size = cpp_decl[ open_bracket_pos + 1: close_bracket_pos ]
      tmp_size.strip()
      if tmp_size and int(tmp_size) != 0 :
        self._array_size  = int(tmp_size)
        self._decl_suffix = '[ %d ]'%(self._array_size) 
      else :
        self._array_size = -1
        self._decl_suffix = '[]'

      # Remove bracketed array suffix before continuing
      cpp_decl = cpp_decl[ : open_bracket_pos ] + cpp_decl[ close_bracket_pos+1 : ]

    # As type parsing continues, 'keyword_stack' will have the individual keywords
    # that make up the c++ declaration appended.
    keyword_stack = []
    
    #
    # Determine whether this is a template type, and use special handling if so.
    # 
    open_bracket_pos = cpp_decl.find( '<' ) 
    template_args    = ''
    if open_bracket_pos > 0 :
      # Find rearmost closing bracket
      close_bracket_pos = cpp_decl.rfind( '>' )  
      
      # Require a closing bracket 
      if close_bracket_pos == -1 :
        raise Exception( "Type.__parse: No closing bracket for template type: '%s'"%(cpp_decl) )

      self._is_template = True 
      
      # Save template arguments 
      template_args = cpp_decl[ open_bracket_pos + 1 : close_bracket_pos ].strip()
    
      # Remove template type suffix
      cpp_decl = cpp_decl[ 0 : open_bracket_pos ]

      # Filter modifiers out of type declaration and store in a list that  
      # can be joined to get the root of this type.
      keyword_stack = self.__compose_typeroot( cpp_decl ) 

      # Restore template type suffix to last element in keyword stack
      keyword_stack[ -1 ] = '%s<%s>'%(keyword_stack[ -1 ], template_args)
    #
    # Handle Non-template types 
    #
    if not self._is_template : 

      # Filter modifiers out of type declaration and store in a list that  
      # can be joined to get the root of this type.
      keyword_stack = self.__compose_typeroot( cpp_decl ) 
  
    #
    # Join keyword stack to produce root type declaration
    #
    self._typeroot = ' '.join( keyword_stack ) 
    
    # Detect primitives
    self._is_primitive = self._typeroot in Primitives.keys() 
    
    #
    # Continue building the complete type declaration by appending
    # pointer/reference indirection indicators to keyword stack.
    # 
    if   self.is_pointer()   : keyword_stack.append( '*' * self.indirections() )
    elif self.is_reference() : keyword_stack.append( '&' * self.indirections() )

    # Add const suffix for aliases to constant data
    if self.is_const_alias() : keyword_stack.append( 'const' ) 
    
    #
    # Build list of type modifier prefixes
    #
    modifiers = []

    # Handle static/volatile/mutable
    if self._is_static and self._is_volatile :
      raise Exeception( "Type.__parse : Declaration includes both 'static' and 'volatile' keywords" ) 
    if self._is_static       : modifiers.append( 'static' ) 
    if self._is_volatile     : modifiers.append( 'volatile' )
    if self._is_mutable      : modifiers.append( 'mutable' )

    # Handle const/constexpr
    if   self._is_const_expr : modifiers.append( 'constexpr' ) 
    elif self._is_const      : modifiers.append( 'const' ) 

    # Determine type size
    if   self._is_primitive :
      self._sizeof = Primitives[ self.typeroot() ].size() 
    elif self.is_alias() :
      self._sizeof = Primitives[ 'int64_t' ].size()
    elif self._is_enum :
      self._sizeof = Primitives[ 'int32_t' ].size()
    else :
      self._sizeof = None 
    
    # Assemble the final type name 
    if modifiers :
      self._decl_prefix = ' '.join( modifiers ) + ' ' 
    self._decl_prefix += ' '.join( keyword_stack ) 
    
  def serialize( self, indent=0 ) :
    '''Serialize to c++ code (note: decl_suffix not appended here)'''
    return (Options.Indent_Char * indent) + self._decl_prefix
  
  def describe( self ) :

    print "  Type.sizeof         : %s"%(str(self._sizeof)) 
    print "  Type.is_primitive   : %s"%(str(self.is_primitive()))
    print "  Type.is_struct      : %s"%(str(self.is_struct()))
    print "  Type.is_template    : %s"%(str(self.is_template()))
    print "  Type.is_pointer     : %s"%(str(self.is_pointer()))
    print "  Type.is_reference   : %s"%(str(self.is_reference()))
    print "  Type.indirections   : %s"%(str(self.indirections()))
    print "  Type.is_alias       : %s"%(str(self.is_alias()))
    print "  Type.is_array       : %s (%s)"%(str(self.is_array()), str(self.array_size()))
    print "  Type.is_const       : %s"%(str(self.is_const())) 
    print "  Type.is_const_alias : %s"%(str(self.is_const_alias()))
    print "  Type.is_const_expr  : %s"%(str(self.is_const_expr()))
    print "  Type.is_mutable     : %s"%( str(not self.is_const()) ) 
    print "  Type.is_static      : %s"%(str(self.is_const_alias()))
    print "  Type.is_volatile    : %s"%(str(self.is_const_expr()))
    print "  Type.namespaces     : %s"%(str(self._namespaces))
    print "  Type.typeroot       : %s"%(self.typeroot())
    print "  Type.decl_prefix    : %s"%(self.decl_prefix()) 
    if self.decl_suffix() : 
      print "  Type.decl_suffix    : %s"%(self.decl_suffix()) 
    print "  Type.typename       : %s"%(self.typename())
    print "  Type.declare( var ) : %s"%(self.declare( 'var' ))
    
  
  def __str__( self ) :
    '''Automatic string conversion'''
    return 'Type : { ' + self.serialize() + ' }'

#
# The 'TypeConversion' class stores the result of a type conversion operation 
#    
class Conversion :

  def __init__( self, src_type, dst_type, conversion_op ) :
    '''src_type     : Type converted from
       dst_type     : Type converted to
       conversion_op : C++ code implementing the conversion
    '''
    if not conversion_op :
      raise Exception( "Conversion: Constructed with empty 'conversion_op' argument" ) 

    self._op = conversion_op

    if isinstance( src_type, str ) :
      self._src = Type( src_type )
    else :
      self._src = src_type 

    if isinstance( dst_type, str ) :
      self._dst = Type( dst_type )
    else :
      self._dst = dst_type 

  def src_type( self ) : 
    '''Returns input/source type as Type'''
    return self._src

  def dst_type( self ) :
    '''Returns output/destination type as Type'''
    return self._dst

  def __str__( self ) :
    return 'Conversion : { src: %s, dst: %s, op: %s }'%( str(self._src), str(self._dst), self._op)

  def serialize( self ) :
    '''Returns c++ conversion code'''
    return self._op
     
  
