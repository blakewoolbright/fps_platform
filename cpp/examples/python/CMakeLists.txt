fps_add_library( 
  NAME          fps_example_module 
  DEPENDS       fps_build_info
                fps_util
  REQUIRES      boost boost-python
  FILES         example.python_module.cpp
)

