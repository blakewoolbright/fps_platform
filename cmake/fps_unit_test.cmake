#----------------------------------------------------------------------------------------------------------------------------------------
#
# FUNCTION   
#   fps_add_unit_test
#
# PARAMETERS 
#   NAME           : required : The name of the unit_test being added.
#   EXCLUDE_FROM   : optional : List of build plans from which this target should be excluded.
#   REQUIRES       : optional : List of external component labels (see: fps_requirements.cmake) or system libraries to include when linking.
#   DEPENDS        : optional : List of build targets on which this unit_test depends.  
#   COMPILER_FLAGS : optional : List of compiler flags to include when compiling this unit_test.
#   LINKER_FLAGS   : optional : List of linker flags to include when linking this unit_test.
#   PROPERTIES     : optional : List of property tags to associate with this library.  (see: fps_properties.cmake)
#   INCLUDES       : optional : List of additional include directories that should be supplied when compiling this library.
#   FILES          : required : List of c++ source files that comprise this unit_test.  
#
# EXAMPLE
#   fps_add_unit_test( 
#     NAME      fps_example_unit_test
#     REQUIRES  boost 
#     DEPENDS   fps_common 
#     FILES     main.cpp
#   )
#
#----------------------------------------------------------------------------------------------------------------------------------------
function( fps_add_unit_test )

  set( NIL )
  fps_parse_argument_list( 
    ARG 
    "NAME;REQUIRES;DEPENDS;FILES;COMPILER_FLAGS;LINKER_FLAGS;INCLUDES;EXCLUDE_FROM;RESULTS_DIR"  
    NIL 
    ${ARGN} 
  )
 
  #
  # Verify that we have a test name.
  #
  if( NOT ARG_NAME ) 
    set( error_msg  
         "   "
         "  [ fps_add_unit_test ]"
         "    File        : '${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt'"
         "    FATAL ERROR : 'Required argument 'NAME' missing from function argument list.'"
         "   " 
    )
    join( "${error_msg}" "\n" error_msg )
    message( FATAL_ERROR "${error_msg}" )
  endif() 

  if( NOT "${ARG_NAME}" MATCHES "^.*[.]unit_test$" )
    set( test__name "${ARG_NAME}.unit_test" )
  else()
    set( test__name ${ARG_NAME} )
  endif()

  fps_is_application_excluded( ${ARG_NAME} ARG_EXCLUDE_FROM is_excluded_from_plan ) 
  if( is_excluded_from_plan ) 
    return() 
  else()
    fps_is_application_excluded( ${test__name} ARG_EXCLUDE_FROM is_excluded_from_plan ) 
    if( is_excluded_from_plan ) 
      return() 
    endif()
  endif() 

  #
  # Merge arguments with associated defaults
  #
  set   (        test__compiler_flags ${FPS_COMMON_COMPILER_FLAGS} )
  list  ( APPEND test__compiler_flags ${ARG_COMPILER_FLAGS} )

  #
  # HACK: Reduce optimization under gcc 4.7.3 because boost 1.51 unit tests don't build correctly under -03
  #
  if( ${FPS_COMPILER_VERSION} STREQUAL "4.7.3" )
    list  ( REMOVE_DUPLICATES test__compiler_flags ) 
    list  ( REMOVE_ITEM       test__compiler_flags "-O3" ) 
    list  ( APPEND            test__compiler_flags "-O1" )
  endif() 

  set   (        test__dependencies   ${FPS_COMMON_DEPENDENCIES} )
  list  ( APPEND test__dependencies   ${ARG_DEPENDS} )
  set   (        test__requirements   ${FPS_COMMON_REQUIREMENTS} ) 
  list  ( APPEND test__requirements   ${ARG_REQUIRES} )
  list  ( APPEND test__requirements   boost )
  set   (        test__include_dirs   ${ARG_INCLUDES} )
  set   (        test__linker_flags   ${FPS_COMMON_LINKER_FLAGS} )
  list  ( APPEND test__linker_flags   ${ARG_LINKER_FLAGS} )

  #
  # Make sure people don't confuse the REQUIRES tag with the DEPENDS tag.
  #
  get_property( all_library_targets GLOBAL PROPERTY ${FPS_PROP__LIB_TARGETS} )
  foreach( req ${test__requirements} )
    fps_list_contains( all_library_targets "${req}" is_compiled_library )
    if( is_compiled_library ) 
      set( error_msg  
           "   "
           "  [ fps_add_unit_test ]"
           "    Name        : '${ARG_NAME}'"
           "    Binary      : '${test__name}'"
           "    File        : '${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt'"
           "    FATAL ERROR : 'Internal library dependencies should be supplied via the DEPENDS keyword."
           "                   Please move '${req}' from the REQUIRES list to the DEPENDS list.'" 
           "   " 
      )
      join( "${error_msg}" "\n" error_msg )
      message( FATAL_ERROR "${error_msg}" )
    endif()
  endforeach() 
  
  #
  # For each dependency, ensure that it's requirements, include dirs, etc are all inherited by 
  # this unit_test.
  #
  if( test_dependencies ) 
    list( REMOVE_DUPLICATES test_dependencies ) 
  endif() 

  foreach( dep ${test__dependencies} )
    #
    # Determine whether this is a utility dependency or a library dependency.
    #
    fps_list_contains( all_library_targets "${dep}" is_compiled_library )
    if( is_compiled_library )
      #
      # Import the dependency's requirements, dependencies, and include directories 
      #
      get_property( dep__requirements GLOBAL PROPERTY "${dep}.requires" )
      get_property( dep__dependencies GLOBAL PROPERTY "${dep}.depends" )
      get_property( dep__include_dirs GLOBAL PROPERTY "${dep}.includes" )

      #
      # And force the new unit_test to inherit them.
      #  
      list( APPEND test__include_dirs ${dep__include_dirs} )
      list( APPEND test__requirements ${dep__requirements} )
      # list( APPEND test__library_deps ${dep__dependencies} )
      # list( APPEND test__library_deps ${dep} )
      if( dep__dependencies )
        list( APPEND test__dependencies ${dep__dependencies} ) 
      endif()

    elseif( NOT TARGET ${dep} )
      set( error_msg  
           "   "
           "  [ fps_add_unit_test ]"
           "    Name        : '${ARG_NAME}'"
           "    Binary      : '${test__name}'" 
           "    File        : '${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt'"
           "    FATAL ERROR : 'Unknown dependency '${dep}' in DEPENDS list.  Has this target been added to the build yet?'"
           "   " 
      )
      join( "${error_msg}" "\n" error_msg )
      message( FATAL_ERROR "${error_msg}\n" )
    endif()
  endforeach()

  #
  # Extract associated include dirs from requirements list and add them.
  #
  list( APPEND test__requirements boost_testing )
  if( test__requirements ) 
    list( REMOVE_DUPLICATES test__requirements )
    fps_get_required_include_dirs( required_include_dirs ${test__requirements} )
    if( required_include_dirs ) 
      list( APPEND test__include_dirs ${required_include_dirs} ) 
    endif() 

    fps_get_required_compiler_flags( required_compiler_flags ${test__requirements} ) 
    if( required_compiler_flags ) 
      list( APPEND test__compiler_flags ${required_compiler_flags} ) 
    endif()
  endif()

  # if( test__library_deps ) 
  #  list( APPEND test__dependencies ${test__library_deps} ) 
  if( test__dependencies )
    list( REMOVE_DUPLICATES test__dependencies )
  endif()

  if( test__library_deps ) 
    list( REMOVE_DUPLICATES test__library_deps )  
  endif()
  if( test__utility_deps ) 
    list( REMOVE_DUPLICATES test__utility_deps ) 
  endif() 
  if( test__requirements ) 
    list( REMOVE_DUPLICATES test__requirements ) 
  endif() 
  if( test__include_dirs )
    list( REMOVE_DUPLICATES test__include_dirs ) 
  endif()
  if( test__compiler_flags ) 
    list( REMOVE_DUPLICATES test__compiler_flags ) 
  endif()

  #
  # If we have build a compiled build info file, add it to our list of input files
  # 
  if( FPS_BUILD_INFO_FILE ) 
    list( APPEND ARG_FILES "${FPS_BUILD_INFO_FILE}" )

    set_source_files_properties( ${FPS_BUILD_INFO_FILE} 
                                 PROPERTIES 
                                 EXTERNAL_OBJECT 1 
                                 GENERATED       1 
                               )
  endif()

  #
  # Add the unit_test target to the build, the set it's various flags, directories, libraries, etc...
  #
  add_executable         ( ${test__name} ${ARG_FILES} )
  set_target_properties  ( ${test__name} PROPERTIES LINKER_LANGUAGE "CXX" )
  if(test__compiler_flags)
    fps_add_compiler_flags( ${test__name} ${test__compiler_flags} ) 
  endif()
  if(test__linker_flags)
    fps_add_linker_flags( ${test__name} ${test__linker_flags} ) 
  endif() 

  fps_add_include_dirs( ${test__name} ${test__include_dirs} )
  if(test__requirements)
    fps_link_requirements( ${test__name} ${test__requirements} )
  endif()
  if(test__dependencies)
    fps_link_dependencies( ${test__name} ${test__dependencies} ) 
  endif()
  
  #
  # Register with CTest
  #
  if( NOT ARG_RESULTS_DIR ) 
    set( ARG_RESULTS_DIR ${FPS_ROOT_DIR}/test_results ) 
  else() 
    set( ARG_RESULTS_DIR ${FPS_ROOT_DIR}/${ARG_RESULTS_DIR} ) 
  endif() 
  set( test__label        ${FPS_BUILD_TYPE} )
  set( test__results_file "${ARG_RESULTS_DIR}/${test__name}.${FPS_BUILD_TYPE}.xml" ) 

  add_test( ${test__name} 
            ${FPS_SCRIPTS_DIR}/run_unit_test.sh --binary ${EXECUTABLE_OUTPUT_PATH}/${test__name}
          ) 

  #
  # Log application configuration in verbose mode
  #  
  if( FPS_VERBOSE )
    set ( ml_delim "\n                      " ) 
    set( log_msg 
         " "
         "[ fps_add_unit_test ]"
         "  Name            : '${ARG_NAME}'"
         "  Binary          : '${test__name}'"
         "  Label           : '${test__label}'"
         "  Results File    : '${test__results_file}'" 
    )
    join( "${test__dependencies}"  ${ml_delim} msg_txt )
    list( APPEND log_msg "  Dependencies    : '${msg_txt}'" )
    join( "${test__requirements}"  ${ml_delim} msg_txt )
    list( APPEND log_msg "  Requirements    : '${msg_txt}'" )
    join( "${test__include_dirs}"  ${ml_delim} msg_txt )
    list( APPEND log_msg "  Include Dirs    : '${msg_txt}'" )

    fps_get_required_library_files( test__required_lib_files ${test__requirements} )
    if( test__required_lib_files ) 
      join( "${test__required_lib_files}" ${ml_delim} msg_txt )
      list( APPEND log_msg "  Library Files   : '${msg_txt}'" )
    endif()
    fps_get_required_library_dirs( test__required_lib_dirs ${test__requirements} )
    if( test__required_lib_dirs ) 
      join( "${test__required_lib_dirs}" ${ml_delim} msg_txt )
      list( APPEND log_msg "  Library Dirs    : '${msg_txt}'" )
    endif()

    join( "${log_msg}" "\n${FPS_INDENT}" log_msg )
    file( APPEND "${FPS_BUILD_LOG}" "${FPS_INDENT}${log_msg}\n\n" )
    fps_log( "${log_msg}\n" ) 
  endif()

endfunction()
