#include <boost/python.hpp>

// const char * wrapped_function01() 
const char * hi_kat() 
{
  // return "example.boost_python : wrapped_function01" ;
  return "hi kat!!!  -blake" ;
}

BOOST_PYTHON_MODULE( libfps_example_module )
{
  namespace bpy = boost::python ;
  // bpy::def( "wrapped_function01", wrapped_function01 ) ;
  bpy::def( "hi_kat", hi_kat ) ;
}

