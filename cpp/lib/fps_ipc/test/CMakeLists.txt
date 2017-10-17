fps_add_application ( 
  NAME          fps_ipc.thread_fifo.unit_test
  REQUIRES      boost
  DEPENDS       fps_string
                fps_time
                fps_ipc
  UNIT_TEST
  FILES         fps_ipc.thread_fifo.unit_test.cpp 
)
  
fps_add_application ( 
  NAME          fps_ipc.shm.unit_test
  REQUIRES      boost
  DEPENDS       fps_string
                fps_time
                fps_ipc
                fps_fs
  UNIT_TEST
  FILES         fps_ipc.shm.unit_test.cpp 
)
