fps_add_library( 
  NAME        fps_ipc
  DEPENDS     fps_system
              fps_string
              fps_fs
  FILES       fps_ipc.cpp
              shm_region.cpp 
              shm_segment.cpp 
)

add_subdirectory( test ) 
