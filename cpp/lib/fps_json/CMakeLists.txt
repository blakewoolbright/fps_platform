fps_add_library( 
  NAME      fps_json
  REQUIRES  boost
  DEPENDS   fps_string
            fps_fs
  FILES     value.cpp
            document.cpp
)

add_subdirectory( test )
add_subdirectory( schema )

