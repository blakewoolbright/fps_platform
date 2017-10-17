fps_add_application( 
  NAME          fps_system.unit_test
  REQUIRES      boost
  DEPENDS       fps_string
                fps_system
  UNIT_TEST
  FILES         fps_system.unit_test.cpp 
)
