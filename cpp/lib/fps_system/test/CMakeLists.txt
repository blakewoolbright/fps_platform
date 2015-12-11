fps_add_unit_test( 
  NAME          fps_system
  REQUIRES      boost
  DEPENDS       fps_string
                fps_system
  FILES         fps_system.unit_test.cpp 
)
