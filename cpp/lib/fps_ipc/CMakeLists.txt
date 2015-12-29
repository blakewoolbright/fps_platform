fps_add_library( 
  NAME        fps_ipc
  DEPENDS     fps_system
              fps_string
  FILES       fps_ipc.cpp
              shared_memory.cpp 
)

add_subdirectory( test ) 
