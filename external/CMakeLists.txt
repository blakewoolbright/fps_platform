
set( INDICATOR_FILE "${CMAKE_CURRENT_SOURCE_DIR}/.fps_external.indicator" )
set( CONFIGURE_CMD  "${FPS_SCRIPTS_DIR}/fpsmake.configure" )

if( FPS_VERBOSE )
  set( CONFIGURE_CMD "${CONFIGURE_CMD} -v" )
endif()

add_custom_command( 
  OUTPUT            ${INDICATOR_FILE}
  COMMAND           ${CONFIGURE_CMD}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  VERBATIM
)

add_custom_target          ( external.components DEPENDS ${INDICATOR_FILE} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
set_source_files_properties( ${INDICATOR_FILE} PROPERTIES GENERATED True )
set_property               ( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${INDICATOR_FILE} )

