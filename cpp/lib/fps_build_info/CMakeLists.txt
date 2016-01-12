fps_add_library( 
  NAME    fps_build_info
  DEPENDS fps_json 
  FILES   fps_build_info.cpp
)

add_subdirectory( test )

