cmake_minimum_required( VERSION 2.8 )

project( fps_platform )
set    ( CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH} )
include( fps )

add_subdirectory( external )
add_subdirectory( cpp )

