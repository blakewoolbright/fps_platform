fps_add_application( 
  NAME     example.shared_memory
  DEPENDS  fps_ipc
  FILES    example.shared_memory.cpp
)

fps_add_application( 
  NAME     example.swmr_shm_queue.writer
  DEPENDS  fps_ipc 
  FILES    example.swmr_shm_queue.writer.cpp
)

fps_add_application( 
  NAME     example.swmr_shm_queue.reader
  DEPENDS  fps_ipc 
  FILES    example.swmr_shm_queue.reader.cpp
)
