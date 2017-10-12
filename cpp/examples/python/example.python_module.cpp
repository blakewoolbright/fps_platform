#include <boost/python.hpp>

const char * wrapped_function01() 
{
  return "example.boost_python : wrapped_function01" ;
}

BOOST_PYTHON_MODULE( fps_example_module )
{
  namespace bpy = boost::python ;
  bpy::def( "wrapped_function01", wrapped_function01 ) ;
}

