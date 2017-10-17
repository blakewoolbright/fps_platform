fps_add_application ( 
  NAME          fps_except.unit_test
  REQUIRES      boost
  DEPENDS       fps_except
                fps_string
  UNIT_TEST
  FILES         fps_except.unit_test.cpp 
)
