
fps_add_unit_test( 
  NAME          fps_fs.unit_test
  DEPENDS       fps_fs
  FILES         fps_fs.unit_test.cpp 
)

if( False )
  fps_add_unit_test( 
    NAME          fps_fs.path
    DEPENDS       fps_fs
    FILES         fps_fs.path.unit_test.cpp 
  )

  fps_add_unit_test( 
    NAME          fps_fs.glob
    DEPENDS       fps_fs
    FILES         fps_fs.glob.unit_test.cpp 
  )

  fps_add_unit_test( 
    NAME          fps_fs.buffer
    DEPENDS       fps_fs
    FILES         fps_fs.buffer.unit_test.cpp 
  )

  fps_add_unit_test( 
    NAME          fps_fs.file
    DEPENDS       fps_fs
    FILES         fps_fs.file.unit_test.cpp 
  )

  fps_add_unit_test( 
    NAME          fps_fs.buffered_file
    DEPENDS       fps_fs
    FILES         fps_fs.buffered_file.unit_test.cpp 
  )
endif()
