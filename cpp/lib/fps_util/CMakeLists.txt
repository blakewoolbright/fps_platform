fps_add_library( 
  NAME    fps_util
  DEPENDS fps_string
  FILES   byte_queue.cpp
          fps_util.cpp
)

add_subdirectory( test )

