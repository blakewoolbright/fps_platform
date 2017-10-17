
fps_add_application( 
  NAME          fps_fs.unit_test
  DEPENDS       fps_fs
  UNIT_TEST
  FILES         fps_fs.unit_test.cpp 
)

if( False )
  fps_add_application( 
    NAME          fps_fs.path.unit_test
    DEPENDS       fps_fs
    UNIT_TEST
    FILES         fps_fs.path.unit_test.cpp 
  )

  fps_add_application( 
    NAME          fps_fs.glob.unit_test
    DEPENDS       fps_fs
    UNIT_TEST
    FILES         fps_fs.glob.unit_test.cpp 
  )

  fps_add_application( 
    NAME          fps_fs.buffer.unit_test
    DEPENDS       fps_fs
    UNIT_TEST
    FILES         fps_fs.buffer.unit_test.cpp 
  )

  fps_add_application( 
    NAME          fps_fs.file.unit_test
    DEPENDS       fps_fs
    UNIT_TEST
    FILES         fps_fs.file.unit_test.cpp 
  )

  fps_add_application( 
    NAME          fps_fs.buffered_file.unit_test
    DEPENDS       fps_fs
    UNIT_TEST
    FILES         fps_fs.buffered_file.unit_test.cpp 
  )
endif()
