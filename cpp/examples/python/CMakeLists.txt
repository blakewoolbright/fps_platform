fps_add_library( 
  NAME          example_boost_module
  DEPENDS       fps_build_info
                fps_util
  REQUIRES      boost boost-python python
  PYMODULE 
  FILES         example.boost_python.cpp
)

fps_add_application( 
  NAME          example.embedded_python
  DEPENDS       fps_build_info
                fps_util
  REQUIRES      python
  FILES         example.embedded_python.cpp
)
