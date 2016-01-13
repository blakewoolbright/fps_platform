fps_add_library( 
  NAME        fps_system
  DEPENDS     fps_string
  FILES       cpu_affinity_mask.cpp
              fps_system.cpp
              
)

add_subdirectory( test ) 
