
#----------------------------------------------------------------------------------------------------------------------------------------
#
# FUNCTION   
#   fps_add_application
#
# ARGUMENTS
#   NAME           : required : The name of the application being added.
#   REQUIRES       : optional : List of external component labels (see: fps_requirements.cmake) or system libraries to include when linking.
#   DEPENDS        : optional : List of build targets on which this application depends.  
#   COMPILER_FLAGS : optional : List of compiler flags to include when compiling this application.
#   LINKER_FLAGS   : optional : List of linker flags to include when linking this application.
#   INCLUDES       : optional : List of additional include directories that should be supplied when compiling this library.
#   FILES          : required : List of c++ source files that comprise this application.  
#
# ATTRIBUTES 
#   UNIT_TEST      : optional : Make this application a unit test.
#   PROFILE        : optional : Create instrumented binaries for profile guided optimization.
#   OPTIONAL       : optional : Only build this program if it is explicitly part of the current custom plan.
#
# EXAMPLE
#   fps_add_application( 
#     NAME      fps_example_application
#     REQUIRES  boost 
#     DEPENDS   fps_string
#     FILES     main.cpp
#   )
#
#----------------------------------------------------------------------------------------------------------------------------------------
function( fps_add_application )
  #
  # Parse arguments
  #
  set( NIL )
  fps_parse_argument_list( 
    ARG
    "NAME;REQUIRES;DEPENDS;COMPILER_FLAGS;LINKER_FLAGS;INCLUDES;FILES"
    "OPTIONAL;PROFILE;UNIT_TEST" 
    ${ARGN}
  )

  if( NOT ARG_NAME )
    set( error_msg  
         "   "
         "  [ fps_add_application ]"
         "    File        : '${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt'"
         "    FATAL ERROR : 'Required argument 'NAME' missing from function call.'"
         "   " 
    )
    join( "${error_msg}" "\n" error_msg )
    message( FATAL_ERROR "${error_msg}" )
  endif()

  fps_is_application_excluded( ${ARG_NAME} ARG_EXCLUDE_FROM is_excluded_from_plan ) 
  if( is_excluded_from_plan ) 
    return() 
  endif() 

  if( NOT ARG_FILES )
    set( error_msg  
         "   "
         "  [ fps_add_application ]"
         "    Name        : ${ARG_NAME}"
         "    File        : ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt"
         "    FATAL ERROR : Required argument 'FILES' missing from function call."
         "   " 
    )
    join( "${error_msg}" "\n" error_msg )
    message( FATAL_ERROR "${error_msg}" )
  endif()

  #
  # Merge arguments with associated defaults
  #
  set   (        app__dependencies   ${FPS_COMMON_DEPENDENCIES} )
  list  ( APPEND app__dependencies   ${ARG_DEPENDS} )
  set   (        app__requirements   ${FPS_COMMON_REQUIREMENTS} ) 
  list  ( APPEND app__requirements   ${ARG_REQUIRES} )
  set   (        app__include_dirs   ${ARG_INCLUDES} )
  list  ( APPEND app__include_dirs   ${FPS_CPP_LIB_DIR} )
  set   (        app__compiler_flags ${FPS_COMMON_COMPILER_FLAGS} )
  list  ( APPEND app__compiler_flags ${ARG_COMPILER_FLAGS} )
  set   (        app__linker_flags   ${FPS_COMMON_LINKER_FLAGS} )
  list  ( APPEND app__linker_flags   ${ARG_LINKER_FLAGS} )

  #
  # Fetch global list of all library targets.
  #
  get_property( library_targets GLOBAL PROPERTY "${FPS_PROP__LIB_TARGETS}" )
  get_property( utility_targets GLOBAL PROPERTY "${FPS_PROP__UTIL_TARGETS}" )

  #
  # Make sure people don't confuse the REQUIRES tag with the DEPENDS tag.
  #
  if( ARG_UNIT_TEST ) 
    list( APPEND app__requirements boost_test ) 
  endif()

  if( app__requirements ) 
    list( REMOVE_DUPLICATES app__requirements )
  endif()
  foreach( req ${app__requirements} )
    fps_list_contains( library_targets "${req}" is_compiled_library )
    if( is_compiled_library ) 
      set( error_msg  
           "   "
           "  [ fps_add_application ]"
           "    Name        : ${ARG_NAME}"
           "    File        : ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt"
           "    FATAL ERROR : Internal library dependencies should be supplied via the DEPENDS keyword. Please move '${req}' from the REQUIRES list to the DEPENDS list." 
           "   " 
      )
      join( "${error_msg}" "\n" error_msg )
      message( FATAL_ERROR "${error_msg}" )
    endif()
  endforeach() 

  if( app__dependencies )
    list( REMOVE_DUPLICATES app__dependencies ) 
  endif()

  #
  # For each library dependency, ensure that it's requirements, include dirs, etc are all inherited by 
  # this application.
  #
  if( app__dependencies )
    foreach( dep ${app__dependencies} )
      fps_list_contains( utility_targets "${dep}" is_utility_library )
      fps_list_contains( library_targets "${dep}" is_compiled_library )
      if( NOT is_utility_library AND NOT TARGET ${dep} )
        set( error_msg  
             "   "
             "  [ fps_add_application ]"
             "    Name  : ${ARG_NAME}"
             "    File  : ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt"
             "    Error : Unknown dependency '${dep}' in DEPENDS list."
             "   " 
        )
        join( "${error_msg}" "\n" error_msg )
        message( FATAL_ERROR "${error_msg}" )
      endif() 

      #
      # Import the dependency's requirements, dependencies, and include directories 
      # and force the new application to inherit them.
      #
      get_property( dep__requirements GLOBAL PROPERTY "${dep}.requires" )
      get_property( dep__dependencies GLOBAL PROPERTY "${dep}.depends" )
      get_property( dep__include_dirs GLOBAL PROPERTY "${dep}.includes" )
      list( APPEND app__include_dirs ${dep__include_dirs} )
      list( APPEND app__requirements ${dep__requirements} )
      list( APPEND app__dependencies ${dep__dependencies} )

    endforeach()

    list( REMOVE_DUPLICATES app__dependencies )
  endif()

  #
  # Extract associated include dirs and compiler flags from requirements list and add them.
  #
  if( app__requirements ) 
    list( REMOVE_DUPLICATES app__requirements )
    fps_get_required_include_dirs( required_include_dirs ${app__requirements} )
    if( required_include_dirs ) 
      list( APPEND app__include_dirs ${required_include_dirs} ) 
    endif() 

    fps_get_required_compiler_flags( required_compiler_flags ${app__requirements} ) 
    if( required_compiler_flags ) 
      list( APPEND app__compiler_flags ${required_compiler_flags} ) 
    endif()
  endif()

  if( app__compiler_flags ) 
    list( REMOVE_DUPLICATES app__compiler_flags )
  endif()
  if( app__linker_flags ) 
    list( REMOVE_DUPLICATES app__linker_flags )
  endif()
  if( app__include_dirs )
    list( REMOVE_DUPLICATES app__include_dirs ) 
  endif()

  #
  # Add the application target to the build, the set it's various flags, directories, libraries, etc...
  #
  set( required_variants "normal" ) 
  set( pgo_instrumented  "pgo_instrumented" )
  set( pgo_optimized     "pgo_optimized" )

  if( FPS_PROFILE AND ARG_PROFILE ) # AND "${FPS_BUILD_TYPE}" STREQUAL "optimized" ) 
    list( APPEND required_variants ${pgo_instrumented} ) 
    list( APPEND required_variants ${pgo_optimized} )
  endif()

  list( REMOVE_DUPLICATES required_variants ) 
  foreach( variant ${required_variants} ) 
    set( variant__name ${ARG_NAME} )
    set( variant__compiler_flags ${app__compiler_flags} )
    set( variant__linker_flags   ${app__linker_flags} )
    set( variant__requirements   ${app__requirements} ) 
    set( variant__include_dirs   ${app__include_dirs} )
    set( variant__dependencies   ${app__dependencies} )

    if( ${variant} STREQUAL ${pgo_instrumented} ) 
      set ( variant__name "${variant__name}.${pgo_instrumented}" ) 
      list( APPEND variant__compiler_flags -fprofile-generate -ftest-coverage )
      # list( APPEND variant__compiler_flags "-fprofile-dir ${CMAKE_CURRENT_SOURCE_DIR}" ) 
      list( APPEND variant__linker_flags   -fprofile-generate ) 
      list( APPEND variant__requirements   gcov )

    elseif( ${variant} STREQUAL ${pgo_optimized} ) 
      set ( variant__name "${variant__name}.${pgo_optimized}" ) 
      list( APPEND variant__compiler_flags -fprofile-use )
      list( APPEND variant__linker_flags   -fprofile-use ) 
      # list( APPEND variant__requirements   gcov )
    elseif( NOT ${variant} STREQUAL "normal" )
      
      set( error_msg  
           "   "
           "  [ fps_add_application ]"
           "    Name  : ${variant_name}"
           "    File  : ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt"
           "    Error : Unknown application/profiling variant suffix '${variant}'"
           "   " 
      )
      join( "${error_msg}" "\n" error_msg )
      message( FATAL_ERROR "${error_msg}" )
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

    add_executable         ( ${variant__name} ${ARG_FILES} )
    set_target_properties  ( ${variant__name} PROPERTIES LINKER_LANGUAGE "CXX" )

    if( variant__compiler_flags ) 
      fps_add_compiler_flags( ${variant__name} ${variant__compiler_flags} ) 
    endif()

    if( variant__linker_flags ) 
      fps_add_linker_flags( ${variant__name} ${variant__linker_flags} )
    endif()

    if( variant__include_dirs )
      fps_add_include_dirs( ${variant__name} ${variant__include_dirs} )
    endif()

    if( variant__requirements )
      fps_link_requirements( ${variant__name} ${variant__requirements} )
    endif()

    if( variant__dependencies )
      fps_link_dependencies( ${variant__name} ${variant__dependencies} ) 
    endif()

    #
    # Register with CTest
    #
    if( ARG_UNIT_TEST ) 
      set( test__name ${ARG_NAME} )
      set( test__results_dir   ${FPS_ROOT_DIR}/test_results ) 
      set( test__results_file "${test__results_dir}/${test__name}.${FPS_BUILD_TYPE}.xml" ) 

      # ${FPS_SCRIPTS_DIR}/run_unit_test.sh --binary ${EXECUTABLE_OUTPUT_PATH}/${test__name}
      add_test( NAME ${test__name} COMMAND ${FPS_SCRIPTS_DIR}/run_unit_test.sh --binary ${FPS_BIN_DIR}/${test__name} )

    endif() 

    #
    # Log application configuration in verbose mode
    #  
    if( FPS_VERBOSE )
      set ( ml_delim "\n                      " ) 
      set( log_msg 
           " "
           "[ fps_add_application ]"
           "  Name            : ${variant__name}"
      )
      if( ARG_UNIT_TEST ) 
        list( APPEND log_msg "  Type            : Unit Test" ) 
      else()
        list( APPEND log_msg "  Type            : Application" ) 
      endif()

      join( "${variant__dependencies}"  ${ml_delim} msg_txt )
      list( APPEND log_msg "  Dependencies    : ${msg_txt}" )

      join( "${variant__requirements}"  ${ml_delim} msg_txt )
      list( APPEND log_msg "  Requirements    : ${msg_txt}" )

      if( variant__include_dirs AND FPS_VERY_VERBOSE )
        join( "${variant__include_dirs}"  ${ml_delim} msg_txt )
        list( APPEND log_msg "  Include Dirs    : ${msg_txt}" )
      endif()

      fps_get_required_library_files( variant__required_lib_files ${variant__requirements} )
      if( variant__required_lib_files AND FPS_VERY_VERBOSE ) 
        join( "${variant__required_lib_files}" ${ml_delim} msg_txt )
        list( APPEND log_msg "  Library Files   : ${msg_txt}" )
      endif()

      fps_get_required_library_dirs( variant__required_lib_dirs ${variant__requirements} )
      if( variant__required_lib_dirs AND FPS_VERY_VERBOSE ) 
        join( "${variant__required_lib_dirs}" ${ml_delim} msg_txt )
        list( APPEND log_msg "  Library Dirs    : ${msg_txt}" )
      endif()

      if( variant__compiler_flags AND FPS_VERY_VERBOSE ) 
        join( "${variant__compiler_flags}" ${ml_delim} msg_txt )
        list( APPEND log_msg "  Compiler Flags  : ${msg_txt}" )
      endif()

      if( variant__linker_flags AND FPS_VERY_VERBOSE ) 
        join( "${variant__linker_flags}" ${ml_delim} msg_txt )
        list( APPEND log_msg "  Linker Flags    : ${msg_txt}" )
      endif()
      
      if ( ARG_PROFILE ) 
        list( APPEND log_msg "  Profiling       : enabled" ) 
      endif()
    
      join( "${ARG_FILES}" ${ml_delim} msg_txt ) 
      list( APPEND log_msg   "  Files           : ${msg_txt}" )

      join( "${log_msg}" "\n${FPS_INDENT}" log_msg )
      file( APPEND "${FPS_BUILD_LOG}" "${FPS_INDENT}${log_msg}\n\n" )
      fps_log( "${log_msg}\n" ) 
    endif()
  endforeach() 

endfunction()                         

