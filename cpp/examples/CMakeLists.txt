fps_add_application( 
  NAME          example.display_fps_build_info
  DEPENDS       fps_build_info
                fps_util
  FILES         example.display_build_info.cpp
)

add_subdirectory( ipc )
add_subdirectory( asm )
add_subdirectory( python )
