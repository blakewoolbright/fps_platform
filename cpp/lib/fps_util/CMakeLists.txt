fps_add_library( 
  NAME    fps_util
  DEPENDS fps_json 
  FILES   byte_queue.cpp
          build_info.cpp 
          fps_util.cpp
)

add_subdirectory( test )

