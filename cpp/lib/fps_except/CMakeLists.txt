fps_add_library(
  NAME    fps_except
  DEPENDS fps_string
  FILES   exception.cpp
          logic_error.cpp
          runtime_error.cpp
)

add_subdirectory( test )
