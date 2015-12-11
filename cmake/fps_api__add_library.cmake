# FUNCTION   
#   fps_add_library 
#
# PARAMETERS 
#   NAME           : required : The name of the library being added.
#   EXCLUDE_FROM   : optional : List of build plans from which this target should be excluded.
#   REQUIRES       : optional : List of external component labels (see: fps_requirements.cmake) or system libraries to include when linking.
#   DEPENDS        : optional : List of build targets on which this library depends.  
#   COMPILER_FLAGS : optional : List of compiler flags to include when compiling this libary.
#   PROPERTIES     : optional : List of property tags to associate with this library.  (see: fps_constants.cmake)
#   INCLUDES       : optional : List of additional include directories that should be supplied when compiling this library.
#   FILES          : optional : List of c++ source files that comprise this library.  
#                               If no FILES are specified, a header-only library is created.
#
# ATTRIBUTES 
#   SHARED         : optional : Always build this library as a shared library.
#   STATIC         : optional : Always build this library as a static library. 
#   PSEUDO         : optional : Internal flag that indicates potential witchcraft.  (See: fps_add_object_files)
#
# EXAMPLE
#   fps_add_library( 
#     NAME      fps_example_library
#     REQUIRES  boost 
#     DEPENDS   fps_string
#               fps_exception
#     FILES     example_library.cpp
#   )
#
#----------------------------------------------------------------------------------------------------------------------------------------
function( fps_add_library )
  
  fps_parse_argument_list( 
    ARG
    "NAME;EXCLUDE_FROM;REQUIRES;DEPENDS;COMPILER_FLAGS;PROPERTIES;INCLUDES;FILES;OBJECTS;DESTINATION"
    "SHARED;STATIC;PSEUDO;STRIPPED"
    ${ARGN}
  )

  if( NOT ARG_NAME )
    set( error_msg  
         "  "
         "  [ fps_add_library ]"
         "    File  : '${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt'"
         "    Error : Required argument 'NAME' not specified."
         "  " 
       )
    join( "${error_msg}" "\n" error_msg )
    message( FATAL_ERROR "${error_msg}" )
  endif()

  if( ARG_EXCLUDE_FROM ) 
    fps_is_library_excluded( ${ARG_NAME} ARG_EXCLUDE_FROM is_excluded_from_plan ) 
    if( is_excluded_from_plan ) 
      return() 
    endif() 
  endif() 

  #
  # Select a library linkage type
  #
  if( ARG_STATIC AND ARG_SHARED )
    set( error_msg  
         "   "
         "  [ fps_add_library ]"
         "    Name  : '${ARG_NAME}'"
         "    File  : '${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt'"
         "    Error : Both STATIC and SHARED linkage cannot be specified for a library target."
         "   " 
    )
    join( "${error_msg}" "\n" error_msg )
    message( FATAL_ERROR "${error_msg}" )
  elseif( ARG_STATIC )
    set( lib__linkage STATIC )
  elseif( ARG_SHARED ) 
    set( lib__linkage SHARED ) 
  endif() 
  string( TOUPPER ${FPS_LINK_TYPE} lib__linkage )

  #
  # Merge arguments with associated defaults
  #
  set ( lib__dependencies          ${FPS_COMMON_DEPENDENCIES} )
  list( APPEND lib__dependencies   ${ARG_DEPENDS} )
  set ( lib__requirements          ${FPS_COMMON_REQUIREMENTS} ) 
  list( APPEND lib__requirements   ${ARG_REQUIRES} )
  set ( lib__include_dirs          ${ARG_INCLUDES} ) 
  list( APPEND lib__include_dirs   ${FPS_CPP_LIB_DIR} )
  set ( lib__compiler_flags        ${FPS_COMMON_COMPILER_FLAGS} )
  list( APPEND lib__compiler_flags ${ARG_COMPILER_FLAGS} )

  #
  # Make sure people don't confuse the REQUIRES tag with the DEPENDS tag.
  #
  get_property( all_library_targets GLOBAL PROPERTY ${FPS_PROPERTY_NAME__LIBRARY_TARGETS} )
  if( lib__requirements ) 
    set( is_compiled_library )
    foreach( req ${lib__requirements} )
      fps_list_contains( all_library_targets "${req}" is_compiled_library )
      if( is_compiled_library ) 
        set( error_msg  
             "   "
             "  [ fps_add_library ]"
             "    Name  : '${ARG_NAME}'"
             "    File  : '${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt'"
             "    Error : Dependencies on internal libraries should be defined via the DEPENDS argument list."
             "             Please move '${req}' from the REQUIRES list to the DEPENDS list.'" 
             "   " 
        )
        join( "${error_msg}" "\n" error_msg )
        message( FATAL_ERROR "${error_msg}" )
      endif()
    endforeach() 
    list( REMOVE_DUPLICATES lib__requirements )
  endif() 

  #
  # Sort dependency arguments by type and propogate inherited dependencies forward.
  #
  if( lib__dependencies ) 
    foreach( dep ${lib__dependencies} )
      fps_list_contains( all_library_targets  "${dep}"  is_library_target ) 
      if( is_library_target )
        #
        # Import the dependency's requirements, dependencies, and include directories 
        #
        get_property( dep__requirements GLOBAL PROPERTY "${dep}.requires" )
        get_property( dep__dependencies GLOBAL PROPERTY "${dep}.depends" )
        get_property( dep__include_dirs GLOBAL PROPERTY "${dep}.includes" )
        list( APPEND lib__include_dirs ${dep__include_dirs} )
        list( APPEND lib__requirements ${dep__requirements} )
        list( APPEND lib__dependencies ${dep__dependencies} )

      elseif( NOT TARGET ${dep} )
        set( error_msg  
             "   "
             "  [ fps_add_library ]"
             "    Name  : '${ARG_NAME}'"
             "    File  : '${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt'"
             "    Error : Unknown dependency '${dep}' in DEPENDS argument list."
             "   " 
        )
        join( "${error_msg}" "\n" error_msg )
        message( FATAL_ERROR "${error_msg}" )
      endif()
    endforeach() 
    list( REMOVE_DUPLICATES lib__dependencies )
  endif() 

  #
  # Import additional resources associated with each requirement label.
  #
  if( lib__requirements ) 
    list( REMOVE_DUPLICATES lib__requirements )
    #
    # Fetch include dirs associated with each requirement and apped to our library's list.
    #
    fps_get_required_include_dirs( required_include_dirs ${lib__requirements} )
    if( required_include_dirs ) 
      list( APPEND lib__include_dirs ${required_include_dirs} )
    endif()
  endif()
  
  if( lib__include_dirs ) 
    list( REMOVE_DUPLICATES lib__include_dirs )
  endif() 

  if( ARG_OBJECTS ) 
    if( NOT ARG_FILES ) 
      set( ARG_FILES ${ARG_OBJECTS} ) 
    else() 
      list( APPEND ARG_FILES ${ARG_OBJECTS} )
    endif()
  endif() 

  if( FPS_VERBOSE )
    #
    # Create build log message and write it.
    # 
    set ( ml_delim "\n                      " ) 
    set( log_msg 
         " "
         "[ fps_add_library ]"
         "  Name            : ${ARG_NAME}"
         "  Linkage         : ${lib__linkage}" 
       )
    if( lib__dependencies )
      join( "${lib__dependencies}" ${ml_delim} lib_deps_txt )
      list( APPEND log_msg "  Dependencies    : ${lib_deps_txt}" )
    endif()

    if( lib__requirements )
      join( "${lib__requirements}" ${ml_delim} lib_reqs_txt )
      list( APPEND log_msg "  Requirements    : ${lib_reqs_txt}" )
    endif() 

    if( lib__include_dirs AND ${FPS_VERBOSE} GREATER 1 )
      join( "${lib__include_dirs}" ${ml_delim} lib_incs_txt )
      list( APPEND log_msg "  Include Dirs    : ${lib_incs_txt}" )
    endif() 

    if( ARG_PROPERTIES )
      list( APPEND log_msg "  Properties      : ${ARG_PROPERTIES}" )
    endif()

    join( "${log_msg}" "\n${FPS_INDENT}" log_msg )
    file( APPEND ${FPS_BUILD_LOG} "${FPS_INDENT}${log_msg}\n\n" )
    fps_log( "${log_msg}\n" )
  endif()

  #
  # If this target includes files that must be compiled, create a library.
  #
  if( ARG_FILES )
    
    # Add fps_build_info.o to the list of files to compile/link
    if( FPS_BUILD_INFO_FILE ) 
      list( APPEND ARG_FILES "${FPS_BUILD_INFO_FILE}" )
      set_source_files_properties( ${FPS_BUILD_INFO_FILE} 
                                   PROPERTIES 
                                   EXTERNAL_OBJECT 1 
                                   GENERATED       1 
                                 )
    endif()

    add_library( ${ARG_NAME} ${lib__linkage} ${ARG_FILES} ) 

    if( lib__compiler_flags )
      fps_add_compiler_flags( ${ARG_NAME} ${lib__compiler_flags} ) 
    endif()

    if( lib__include_dirs )
      fps_add_include_dirs  ( ${ARG_NAME} ${lib__include_dirs}   )
    endif()

    if( lib__requirements ) # AND NOT BUILD_SHARED_LIBS )
      fps_link_requirements( ${ARG_NAME} ${lib__requirements} )
    endif()

    if( lib__dependencies )
      fps_link_dependencies( ${ARG_NAME} ${lib__dependencies} )
    endif()

    #
    # Add library to global list of library targets.
    # 
    if( NOT ARG_PSEUDO ) 
      set_property( GLOBAL APPEND PROPERTY ${FPS_PROPERTY_NAME__LIBRARY_TARGETS} ${ARG_NAME} )
    endif()

  else() 
    # 
    # If this target does not require direct compilation, create a target on which others targets can depend.
    #
    add_custom_target( ${ARG_NAME} ALL ) 
    if( lib__dependencies ) 
      list( REMOVE_DUPLICATES lib__dependencies ) 
      add_dependencies( ${ARG_NAME} ${lib__dependencies} ) 
    endif()
    
    #
    # And add it to our global list of utility targets.
    # 
    set_property( GLOBAL APPEND PROPERTY ${FPS_PROPERTY_NAME__UTILITY_TARGETS} ${ARG_NAME} )
  endif()

  #
  # Use global properties to store information about this target's requirements, dependencies, and header locations.
  #
  set_property( GLOBAL PROPERTY "${ARG_NAME}.requires" ${lib__requirements} )
  set_property( GLOBAL PROPERTY "${ARG_NAME}.depends"  ${lib__dependencies} )
  set_property( GLOBAL PROPERTY "${ARG_NAME}.includes" ${lib__include_dirs} )
  
  #
  # Finally import and apply property label arguments to the target.
  #
  if( ARG_PROPERTIES ) 
    foreach( property_label ${ARG_PROPERTIES} ) 
      if( "${property_label}" STREQUAL "${FPS_PROPERTY_LABEL__LINK_WHOLE_ARCHIVE}" )
        set_property( GLOBAL APPEND PROPERTY "${FPS_PROPERTY_GROUP__LINK_WHOLE_ARCHIVE}" ${ARG_NAME} )
      endif()
    endforeach()
  endif()

endfunction()        




















