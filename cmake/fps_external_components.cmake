#----------------------------------------------------------------------------------------------------------------------------------------
#
# Function 
#   fps_add_external_component
#
# Arguments ( When adding complex components )
#   LABEL               : The requirement label/group to which this component should be added/appended.
#   DESCRIPTION         : Component text description.
#   VERSION             : Version info/string to associate with this component.
#   INCLUDE_DIRS        : List of additional header file directories to include when compiling.
#   SYSTEM_INCLUDE_DIRS : Like INCLUDE_DIRS, but warnings are supressed during compilation of these headers.
#   LIBRARY_DIRS        : List of additional directories to search when linking with this component.
#   REQUIRES            : List of system libraries that this component requires.  (ex: ssl, libssl.so, libssl.a)
#   PROVIDES            : List of libraries provided by this component.
#
# Argument ( When adding simple system libraries ) 
#   NAME                : A system library name ( eg. pthread, bz2, crypto, etc. ) 
#
# PROPERTIES 
#   SHARED              : Set the preferred linkage to 'shared' for this component - Overrides FPS_LINK_TYPE
#   STATIC              : Set the preferred linkage to 'static' for this component - Overrides FPS_LINK_TYPE
#  
#----------------------------------------------------------------------------------------------------------------------------------------
function( fps_add_external_component )

  fps_parse_argument_list( 
    ARG 
    "LABEL;DESCRIPTION;VERSION;INCLUDE_DIRS;SYSTEM_INCLUDE_DIRS;LIBRARY_DIRS;COMPILER_FLAGS;REQUIRES;PROVIDES;NAME" 
    "SHARED;STATIC" 
    ${ARGN} 
  )
  
  if( NOT ARG_LABEL )
    if( NOT ARG_NAME ) 
      set( error_msg 
           " " 
           " [ fps_add_required_component ]"
           "   Missing required argument :: Please provide either 'LABEL' or 'NAME' to this function."
           "   Your arguments            :: ${ARGN}"
           " "
         )
      join( "${error_msg}" "\n" error_msg )
      message( FATAL_ERROR "${error_msg}" )
    else() 
      set( ARG_LABEL ${ARG_NAME} )
    endif()
  endif()

  if( ARG_NAME AND NOT ARG_PROVIDES ) 
    set( ARG_PROVIDES ${ARG_NAME} ) 
  endif()

  if( BUILD_SHARED_LIBS ) 
    set( component_linkage "SHARED" ) 
  else() 
    set( component_linkage "STATIC" ) 
  endif() 

  #
  # Import global variables associated with this label to local scope if they exist.
  #
  string( TOUPPER "${ARG_LABEL}" UC_LABEL ) 
  set( component_library_names  ${FPS_${UC_LABEL}_LIBRARY_NAMES} )
  set( component_library_files  ${FPS_${UC_LABEL}_LIBRARY_FILES} )
  set( component_library_dirs   ${FPS_${UC_LABEL}_LIBRARY_DIRS} )
  set( component_include_dirs   ${FPS_${UC_LABEL}_INCLUDE_DIRS} )
  set( component_compiler_flags ${FPS_${UC_LABEL}_COMPILER_FLAGS} )

  if( NOT component_library_dirs )
    set( component_library_dirs ${FPS_LIB_SEARCH_PATHS} )
  #else()
  #  list( APPEND component_library_dirs ${FPS_LIB_SEARCH_PATHS} )
  endif()
  
  #
  # Force argument lists into cmake compatible list format.
  #
  split( "${ARG_INCLUDE_DIRS}"        " " ARG_INCLUDE_DIRS )
  split( "${ARG_INCLUDE_DIRS}"        " " ARG_INCLUDE_DIRS )
  split( "${ARG_SYSTEM_INCLUDE_DIRS}" " " ARG_SYSTEM_INCLUDE_DIRS )
  split( "${ARG_SYSTEM_INCLUDE_DIRS}" " " ARG_SYSTEM_INCLUDE_DIRS )
  split( "${ARG_LIBRARY_DIRS}"        " " ARG_LIBRARY_DIRS )
  split( "${ARG_LIBRARY_DIRS}"        " " ARG_LIBRARY_DIRS )
  split( "${ARG_PROVIDES}"            " " ARG_PROVIDES )
  split( "${ARG_PROVIDES}"            " " ARG_PROVIDES )
  split( "${ARG_REQUIRES}"            " " ARG_REQUIRES )
  split( "${ARG_REQUIRES}"            " " ARG_REQUIRES )

  if( ARG_PROVIDES )
    foreach( lib_file ${ARG_PROVIDES} )
      fps_library_file_to_library_name( ${lib_file} lib_name )
      list( APPEND component_library_names ${lib_name} )
    endforeach() 
  endif()

  if( ARG_REQUIRES )
    foreach( lib_name ${ARG_REQUIRES} )
      list( APPEND component_library_names ${lib_name} )
    endforeach() 
  endif()
  
  if( ARG_SYSTEM_INCLUDE_DIRS ) 
    set( FPS_SYSTEM_INCLUDE_DIRS "${FPS_SYSTEM_INCLUDE_DIRS};${ARG_SYSTEM_INCLUDE_DIRS}" PARENT_SCOPE )
    list( APPEND component_include_dirs ${ARG_SYSTEM_INCLUDE_DIRS} )
  endif() 

  if( ARG_INCLUDE_DIRS )
    list( APPEND component_include_dirs   ${ARG_INCLUDE_DIRS} )
  endif()
  if( ARG_LIBRARY_DIRS ) 
    list( APPEND component_library_dirs   ${ARG_LIBRARY_DIRS} ) 
  endif()
  if( ARG_COMPILER_FLAGS )
    list( APPEND component_compiler_flags ${ARG_COMPILER_FLAGS} )
  endif()

  if( component_library_names ) 
    list( REMOVE_DUPLICATES component_library_names ) 
  endif()
  if( component_library_dirs ) 
    list( REMOVE_DUPLICATES component_library_dirs ) 
  endif()  

  #
  # Tell cmake to dig through our search paths to locate the component libraries.
  #
  if( component_library_names )
    if( ARG_SHARED )
      set( component_linkage "SHARED" )
    elseif( ARG_STATIC )
      set( component_linkage "STATIC" )
    else() 
    endif()
    
    foreach( lib_name ${component_library_names} ) 
      #
      # If an actual library filename is supplied, just add it to the list, otherwise convert to filename and add both.
      #
      string( REGEX MATCH "(^lib.*[.]((so.*)|(a))$)" is_full_libname "${lib_name}" )
      if( NOT is_full_libname ) 
        if( component_linkage STREQUAL "SHARED" )
          set( libs_by_linkage lib${lib_name}.so ${lib_name} )
        else()
          set( libs_by_linkage lib${lib_name}.a ${lib_name} )
          set( component_linkage "STATIC" )
        endif() 
      else()
        set( libs_by_linkage ${lib_name} )
        list( REMOVE_ITEM component_library_names ${lib_name} )
      endif()
      unset( component_found_libs CACHE )

      find_library( component_found_libs NAMES ${libs_by_linkage} PATHS ${component_library_dirs} NO_DEFAULT_PATH ) 

      #if( NOT component_found_libs ) 
      #  list( APPEND component_library_files ${component_found_libs} )
      #else(
      #  find_library( component_found_libs NAMES ${libs_by_linkage} PATHS ${FPS_LIB_SEARCH_PATHS} ) 
      #endif() 

      if( NOT component_found_libs ) 
        join( "${libs_by_linkage}" ", " msg_lib_names )
        join( "${component_library_dirs}" ", " msg_lib_dirs )
        set( error_msg 
             " " 
             " [ fps_add_required_component ]"
             "   Failed to find library :: '${lib_name}'" 
             "   Candidate filenames    :: '${msg_lib_names}'"
             "   Directories searched   :: '${msg_lib_dirs}'" 
             " "
           )
        join( "${error_msg}" "\n" error_msg )
        message( FATAL_ERROR "${error_msg}" )
      else()
        list( APPEND component_library_files ${component_found_libs} )
      endif()

    endforeach()
  endif()

  if( component_include_dirs ) 
    list( REMOVE_DUPLICATES component_include_dirs ) 
  endif()    
  if( component_library_files ) 
    list( REMOVE_DUPLICATES component_library_files) 
  endif()  
  if( component_compiler_flags )
    list( REMOVE_DUPLICATES component_compiler_flags ) 
  endif() 

  #
  # Update the global variables associated with this requirement label 
  #
  set( FPS_${UC_LABEL}_INCLUDE_DIRS     "${component_include_dirs}"   PARENT_SCOPE )
  set( FPS_${UC_LABEL}_LIBRARY_NAMES    "${component_library_names}"  PARENT_SCOPE )
  set( FPS_${UC_LABEL}_LIBRARY_DIRS     "${component_library_dirs}"   PARENT_SCOPE )
  set( FPS_${UC_LABEL}_LIBRARY_FILES    "${component_library_files}"  PARENT_SCOPE )
  set( FPS_${UC_LABEL}_COMPILER_FLAGS   "${component_compiler_flags}" PARENT_SCOPE )
  set( FPS_${UC_LABEL}_LINKAGE          "${component_linkage}"        PARENT_SCOPE )

  set_property( GLOBAL APPEND PROPERTY ${FPS_PROP__REQ_LABELS} ${ARG_LABEL} )

  if( FPS_VERBOSE )
    set ( ml_delim "'\n                  + '" )
    set( log_msg 
         "[ Registered Component :: '${ARG_LABEL}' ]" 
         "  Label          :: '${ARG_LABEL}'"
         "  Description    :: '${ARG_DESCRIPTION}'" 
       )
    if( ARG_VERSION )
      list( APPEND log_msg "  Version        :: '${ARG_VERSION}'" )
    endif()
    if( ARG_INCLUDE_DIRS )
      join( "${component_include_dirs}" ${ml_delim} msg_txt )
      list( APPEND log_msg "  Include Dirs   :: '${msg_txt}'" ) 
    endif()
    if( ARG_LIBRARY_DIRS )
      join( "${component_library_dirs}" ${ml_delim} msg_txt )
      list( APPEND log_msg "  Library Dirs   :: '${msg_txt}'" ) 
    endif()
    if( component_library_names )
      join( "${component_library_names}" ${ml_delim} msg_txt )
      list( APPEND log_msg "  Library Names  :: '${msg_txt}'" )
    endif()
    if( component_library_files )
      join( "${component_library_files}" ${ml_delim} msg_txt )
      list( APPEND log_msg "  Library Files  :: '${msg_txt}'" )
    endif()
    if( component_linkage )
      string( TOLOWER "${component_linkage}" lc_linkage )
      list( APPEND log_msg "  Linkage        :: '${lc_linkage}'" )
    endif()
    if( component_compiler_flags ) 
      list( APPEND log_msg "  Compiler Flags :: '${component_compiler_flags}'" )
    endif()

    # file( APPEND "${FPS_BUILD_LOG}" "${FPS_INDENT}${log_msg}\n\n" )
    # join( "${log_msg}" "\n" log_msg ) 
    # fps_log( "${log_msg}\n" )
  endif()
endfunction()

#-------------------------------------------------------------------------------------------------------------------
#
# Create requirement definitions for any system libraries that we don't directly maintain in our source tree.
#
#-------------------------------------------------------------------------------------------------------------------
if( "${FPS_OS_VENDOR}" STREQUAL "ubuntu" )
  set( PGSQL_VER "9.3" )
  set( PGSQL_INC /usr/include/postgresql )
  set( PGSQL_LIB /usr/lib /usr/lib/x86_64-linux-gnu )
else()
  set( PGSQL_VER "9.2" )
  set( PGSQL_INC /usr/pgsql-${PGSQL_VER}/include )
  set( PGSQL_LIB /usr/pgsql-${PGSQL_VER}/lib )
endif()

#fps_add_external_component( 
#  LABEL               pq
#  SHARED
#  DESCRIPTION         "Postgres SQL Client Library"
#  VERSION             ${PGSQL_VER}
#  INCLUDE_DIRS        ${PGSQL_INC}
#  LIBRARY_DIRS        ${PGSQL_LIB}
#  REQUIRES            crypto
#                      ssl
#  PROVIDES            pq
#)

fps_add_external_component( 
  LABEL              pthread
  SHARED
  DESCRIPTION        "PThreads Library"
  PROVIDES           pthread
)

#-------------------------------------------------------------------------------------------------------------------
#
# Find libcurl 
#
#-------------------------------------------------------------------------------------------------------------------
#  fps_add_external_component( 
#    LABEL               curl
#    SHARED
#    DESCRIPTION         "CURL HTTP Utility Library"
#    VERSION             "7.35"
#    REQUIRES            ssl
#                        crypto
#                        gssapi_krb5
#                        
#    PROVIDES            curl
#  )

#-------------------------------------------------------------------------------------------------------------------
#
# Now include the requirement definitions for components that live in the externals directory.
#
#-------------------------------------------------------------------------------------------------------------------
file( GLOB script_list "${FPS_EXTERNAL_CMAKE_DIR}/*.cmake" )
foreach( entry ${script_list} )
  include( ${entry} )
endforeach()


#-------------------------------------------------------------------------------------------------------------------
# Add boost components
#-------------------------------------------------------------------------------------------------------------------
fps_add_external_component( 
    LABEL                "boost"
    DESCRIPTION          "Boost C++ Libraries (system default)"
    SYSTEM_INCLUDE_DIRS  /usr/include
    REQUIRES             libpthread.so
    PROVIDES             "boost_system boost_program_options boost_filesystem boost_regex boost_thread boost_date_time boost_serialization"
)

fps_add_external_component( 
    LABEL                "boost-python"
    DESCRIPTION          "Boost C++ / Python Interop Library"
    SYSTEM_INCLUDE_DIRS  /usr/include/python2.7
    REQUIRES             pthread python2.7
    PROVIDES             boost_python
)

#-------------------------------------------------------------------------------------------------------------------
# Finally, make sure we know where to find the boost unit test framework. 
#-------------------------------------------------------------------------------------------------------------------
if( "${FPS_BOOST_LINKAGE}" STREQUAL "SHARED" ) 
  set( boost_testing_flags -DBOOST_TEST_DYN_LINK )
else()
  set( boost_testing_flags )
endif()

fps_add_external_component( 
  LABEL                 boost_testing
  DESCRIPTION           "Boost Unit Test Framework"
  SYSTEM_INCLUDE_DIRS   ${FPS_BOOST_INCLUDE_DIRS}
  LIBRARY_DIRS          ${FPS_BOOST_LIBRARY_DIRS}
  COMPILER_FLAGS        ${boost_testing_flags}
  PROVIDES              boost_unit_test_framework
  ${FPS_BOOST_LINKAGE}
)
  




