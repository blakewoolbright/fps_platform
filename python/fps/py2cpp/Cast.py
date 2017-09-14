from Type import Type, Metrics 
from Variable import Variable
import Options

class Cast :

  def __init__( self, src_var, dst_type, cast_arg=None ) :
    '''src_var  : Input Variable/Type for this cast
       dst_type : Destination Type for this cast
       cast_arg : Argument to actual cast operation (to use instead of src_var.name())
    '''

    if cast_arg and not isinstance( cast_arg, str ) :
      raise Exception( "Cast: Constructed with non-null, non-string 'cast_arg' argument" )

    if isinstance( src_var, str ) :
      src_var = Variable( src_var ) 
    
    if isinstance( dst_type, str ) :
      dst_type = Type( dst_type ) 

    if not isinstance( dst_type, Type ) :
      raise Exception( "Cast: Unsupported argument type for 'dst_type' ('%s')"%(type(dst_type).__name__) )
    
    if isinstance( src_var, Type ) and cast_arg :
      self._var = Variable( src_var.declare( cast_arg ) )
    elif isinstance( src_var, Variable ) :
      self._var = src_var 
    else :
      raise Exception( "Cast: Unsupported 'src_var' argument type (%s)"%(type(src_var).__name__) )
    
    self._dst       = dst_type
    self._prefix    = ''
    self._cast      = ''

    # Now compose the actual cast operation
    self.__compose() 

    # print str(self._cast)

  def __explicit_cast( self ) :
    '''Explicit cast'''
    self._cast = '(%s)%s'%( self._dst.typename(), self._var.name() )

  def __static_cast( self ) :
    '''Static cast'''
    self._cast = 'static_cast<%s>( %s )'%( self._dst.typename(), self._var.name() )

  def __array_to_pointer_cast( self ) :
    '''Array to pointer cast'''
    self._cast = 'reinterpret_cast<%s>( &(%s[0]) )'%( self._dst.typename(), self._var.name() )

  def __alias_cast( self ) :
    '''Pointer/reference to various types'''

    # Pointer to Pointer and Reference to Reference
    if( (self._var.type().is_pointer() and self._dst.is_pointer()) or
        (self._var.type().is_reference() and self._dst.is_reference())
      ) :

      cast_type  = self._dst.typename() 
      self._cast = 'reinterpret_cast<%s>( %s )'%(cast_type, self._var.name()) 
      return 
   
    # Pointer to Reference
    if self._var.type().is_reference() and self._dst.is_pointer() :
      
      # Use reinterpret cast to convert between different typeroots
      if self._var.type().typeroot() != self._dst.typeroot() :
        cast_type  = self._dst.typename() 
        self._cast = 'reinterpret_cast<%s>( &%s )'%(cast_type, self._var.name())
      else :
        self._cast = '*' + self._var.name() 

      return

    # Pointer to primitive casts are ok if the result type is large
    # enough to hold a pointer
    if self._var.type().is_pointer() and self._dst.is_primitive() :
      if self._dst.sizeof() < self._var.type().sizeof() :
        raise Exception\
              ( "Cast.__alias_cast: Illegal cast from pointer to primitive (%s -> %s)"
                %( self._var.type().typename(), self._dst.typename() )
              )

      self._cast = 'reinterpret_cast<%s>( %s )'%(self._dst.typename(), self._var.name()) 
      return 

    raise Exception\
          ( "Cast.__alias_cast: Unsupported cast from '%s' to '%s'"
            %( self._var.type().typename(), self._dst.typename() )
          )

  def __compose( self ) :
    '''Compose cast operation base on src/dst types'''

    # 1) Handle casts between pointer/reference types via reinterpret_cast
    if( self._var.type().is_alias() and (self._dst.is_alias() or self._dst.is_primitive()) ) :
      self.__alias_cast() 
    # 2) Handle casts of arrays to pointers 
    elif( self._var.type().is_array() and self._dst.is_pointer() ):
      self.__array_to_pointer_cast() 
    # 3) Handle static casts
    elif not (self._var.type().is_alias() and self._dst.is_alias()) :
      self.__static_cast()
    # 4) Default to explicit cast
    else :
      self.__explicit_cast()
    return self

  def to_string( self, indent=0 ) :
    return (indent * Options.Indent_Char) \
           + "Cast : { src: '%s', dst: '%s' } -> '%s'" \
             %( self._var.serialize(), self._dst.typename(), self._cast )
  
  def __str__( self ) :
    return self.to_string( 0 )
        
      
    
    
    


      
    
