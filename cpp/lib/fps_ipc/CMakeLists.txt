fps_add_library( 
  NAME        fps_ipc
  DEPENDS     fps_system
              fps_string
              fps_fs
              fps_except
  FILES       fps_ipc.cpp
              shared_memory.cpp 
              mapped_memory.cpp
)

add_subdirectory( test ) 
