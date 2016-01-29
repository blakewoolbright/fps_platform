fps_add_library( 
  NAME    fps_util
  DEPENDS fps_string
          fps_except
  FILES   
          fps_util.cpp
          send_email.cpp
)

add_subdirectory( test )

