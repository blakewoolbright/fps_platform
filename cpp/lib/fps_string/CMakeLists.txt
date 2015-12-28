fps_add_library( 
  NAME      fps_string 
  REQUIRES  boost
  DEPENDS   fps_math
  FILES     format.cpp
            fps_string.cpp
)

add_subdirectory( test )

