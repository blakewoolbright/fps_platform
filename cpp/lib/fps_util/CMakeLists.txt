fps_add_library( 
  NAME    fps_util
  DEPENDS fps_json 
  FILES   byte_queue.cpp
          build_info.cpp 
  
)

add_subdirectory( test )

