fps_add_library( 
  NAME    fps_container
  DEPENDS fps_string
          fps_util
          fps_ntp
  FILES   byte_queue.cpp
          fps_container.cpp
)

add_subdirectory( test )

