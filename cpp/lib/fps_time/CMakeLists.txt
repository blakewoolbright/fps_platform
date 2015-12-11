fps_add_library( 
  NAME      fps_time
  DEPENDS   fps_string
  FILES     timestamp.cpp
            datetime.cpp
            tz_manager.cpp 
)

add_subdirectory( test ) 
