fps_add_library( 
  NAME       fps_fs

  REQUIRES   boost
             bz2
             z

  DEPENDS    fps_string
             fps_util

  FILES      path.cpp
             glob.cpp
             file.cpp 
)

add_subdirectory( test )
