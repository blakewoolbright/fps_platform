fps_add_application( 
  NAME     shm_fifo.reader_test
  DEPENDS  fps_ipc
  FILES    shm_fifo.reader_test.cpp
)

fps_add_application( 
  NAME     shm_fifo.writer_test
  DEPENDS  fps_ipc
  FILES    shm_fifo.writer_test.cpp
)
