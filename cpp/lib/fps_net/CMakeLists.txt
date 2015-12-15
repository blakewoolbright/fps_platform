fps_add_library( 
  NAME        fps_net

  DEPENDS     fps_util
              fps_string
              fps_time
              fps_except

  FILES       address.cpp
)

add_subdirectory( test )
