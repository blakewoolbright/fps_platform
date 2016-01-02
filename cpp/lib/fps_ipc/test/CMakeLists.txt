fps_add_unit_test ( 
  NAME          fps_ipc.thread_fifo
  REQUIRES      boost
  DEPENDS       fps_string
                fps_time
                fps_ipc
  FILES         fps_ipc.thread_fifo.unit_test.cpp 
)
  
fps_add_unit_test ( 
  NAME          fps_ipc.shm
  REQUIRES      boost
  DEPENDS       fps_string
                fps_time
                fps_ipc
                fps_fs
  FILES         fps_ipc.shm.unit_test.cpp 
)
