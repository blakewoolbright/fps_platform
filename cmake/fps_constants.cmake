#-----------------------------------------------------------------------------------------------------------------
#
# Core build system variable declarations.
#
#-----------------------------------------------------------------------------------------------------------------
site_name( FPS_BUILD_HOST )

#-----------------------------------------------------------------------------------------------------------------
#
# CMake variables populated by fpsmake 
#
#-----------------------------------------------------------------------------------------------------------------
set( FPSMAKE_VERSION          "$ENV{FPSMAKE_VERSION}" )             # The major version number of fpsmake.
set( FPS_COMPILER_DIR         "$ENV{FPS_COMPILER_DIR}" )            # Directory where our compiler lives.
set( FPS_TOOLCHAIN_DIR        "$ENV{FPS_TOOLCHAIN_DIR}" )           # Directory where our private toolchain lives.
set( FPS_BUILD_PLAN           "$ENV{FPS_BUILD_PLAN}" )              # Name of build plan { master, nightly, etc.. }
set( FPS_BUILD_TYPE           "$ENV{FPS_BUILD_TYPE}" )              # Optimized vs. Debug
set( FPS_LINK_TYPE            "$ENV{FPS_LINK_TYPE}" )               # Shared vs. Static linkage
set( FPS_ROOT_DIR             "$ENV{FPS_ROOT_DIR}" )                # Root directory of the source tree
set( FPS_CMAKE_DIR            "$ENV{FPS_CMAKE_DIR}" )               # Directory in which this file lives
set( FPS_BUILD_DIR            "$ENV{FPS_BUILD_DIR}" )               # Build intermediates and makefiles
set( FPS_BIN_DIR              "$ENV{FPS_BIN_DIR}" )                 # Path to compiled application binaries
set( FPS_LIB_DIR              "$ENV{FPS_LIB_DIR}" )                 # Path to compiled library binaries
set( FPS_EXTERNAL_DIR         "$ENV{FPS_EXTERNAL_DIR}" )            # Path to 3rd party components
set( FPS_EXTERNAL_CMAKE_DIR   "$ENV{FPS_EXTERNAL_CMAKE_DIR}" )
set( FPS_CPP_DIR              "$ENV{FPS_CPP_DIR}" )                 # Path to c++ source code
set( FPS_CPP_APP_DIR          "$ENV{FPS_CPP_DIR}/app" )
set( FPS_CPP_LIB_DIR          "$ENV{FPS_CPP_DIR}/lib" )
set( FPS_CPP_INC_DIR          "$ENV{FPS_CPP_INC_DIR}" )
set( FPS_PYTHON_DIR           "$ENV{FPS_PYTHON_DIR}"  )             # Path to source/python
set( FPS_SCRIPTS_DIR          "$ENV{FPS_SCRIPTS_DIR}" )             # Path to location of build scripts
set( FPS_OS_VENDOR            "$ENV{FPS_OS_VENDOR}" )
set( FPS_OS_VERSION           "$ENV{FPS_OS_VERSION}" )
set( FPS_PROFILE              "$ENV{FPS_PROFILE}" )
set( FPS_BUILD_INFO_FILE      "${CMAKE_BINARY_DIR}/fps_build_info.o" )

if( NOT EXISTS ${FPS_BUILD_INFO_FILE} )
  set( FPS_BUILD_INFO_FILE ) 
endif()

string( TOLOWER "${FPS_OS_VENDOR}" FPS_OS_VENDOR )

#-----------------------------------------------------------------------------------------------------------------
#
# CMake variables populated via command line
#
#   FPS_VERBOSE             # If greater than 0, enable verbose build output during makefile generation.
#   CMAKE_VERBOSE_MAKEFILE  # If true, enable verbose output during compilation and linking.
#
#-----------------------------------------------------------------------------------------------------------------
if( NOT FPS_BAMBOO_BUILD ) 
  set( FPS_BAMBOO_BUILD )  # Prevent 'CMake Warning : FPS_BAMBOO not used by build'
else() 
  if( NOT IS_DIRECTORY ${FPS_ROOT_DIR}/test_results ) 
    file( MAKE_DIRECTORY ${FPS_ROOT_DIR}/test_results ) 
  endif()
endif() 

#-----------------------------------------------------------------------------------------------------------------
#
# Configure build plan.  If this is a user-defined build plan import the plan definition file.
#
#-----------------------------------------------------------------------------------------------------------------
set( FPS_PRODUCTION_BUILD_PLANS "master" )
set( FPS_CUSTOM_PLAN_TARGETS )                            # Custom plans only include certain targets.
if( NOT FPS_BUILD_PLAN )
  set( error_msg 
       " "
       " [ fps_constants ]"
       "   Error : Required environment variable 'FPS_BUILD_PLAN' not populated"
       " " 
     ) 
  join( "${error_msg}" "\n" error_msg )
  message( FATAL_ERROR "${error_msg}" )
endif()

fps_list_contains( FPS_PRODUCTION_BUILD_PLANS "${FPS_BUILD_PLAN}" is_production_plan )
if( NOT is_production_plan )
  set( custom_plan_file "${FPS_CMAKE_DIR}/plans/${FPS_BUILD_PLAN}.plan" )
  if( NOT EXISTS ${custom_plan_file} )
    set( error_msg 
         " "
         " [ fps_constants ]"
         "   Error : Unable to open custom plan file '${custom_plan_file}'" 
         " " 
       ) 
    join( "${error_msg}" "\n" error_msg )
    message( FATAL_ERROR "${error_msg}" )
  endif() 

  #
  # Build plan files are simple, whitespace delimited lists of target names.
  # 
  file( STRINGS ${custom_plan_file} custom_plan_lines NO_HEX_CONVERSION ) 
  foreach( line ${custom_plan_lines} )
    split( "${line}" " " target_list ) 
    list( APPEND FPS_CUSTOM_PLAN_TARGETS ${target_list} ) 
  endforeach() 
  set( custom_plan_file ) 
endif() 

#-----------------------------------------------------------------------------------------------------------------
#
# Configure installation support
#
#-----------------------------------------------------------------------------------------------------------------
set( CMAKE_INSTALL_PREFIX ${FPS_ROOT_DIR} ) 

#-----------------------------------------------------------------------------------------------------------------
#
# Configure default library linkage policy.
#
#-----------------------------------------------------------------------------------------------------------------
set( BUILD_SHARED_LIBS "False" )
if( "${FPS_LINK_TYPE}" STREQUAL "shared" )
  set( BUILD_SHARED_LIBS "True" )
  set( CMAKE_FIND_LIBRARY_SUFFIXES .so .a )
else()
  set( BUILD_SHARED_LIBS "False" )
  set( CMAKE_FIND_LIBRARY_SUFFIXES .a .so )
endif()

#-----------------------------------------------------------------------------------------------------------------
#
# Populate the Cmake variables that control where compiled applications and libraries are written.
#
#-----------------------------------------------------------------------------------------------------------------
set( FPS_BIN_DIR_NAME        "bin.${FPS_BUILD_TYPE}.${FPS_LINK_TYPE}" )
set( FPS_LIB_DIR_NAME        "lib.${FPS_BUILD_TYPE}.${FPS_LINK_TYPE}" )
set( EXECUTABLE_OUTPUT_PATH "${FPS_BUILD_DIR}/${FPS_BIN_DIR_NAME}" )   # Compiled applications dir.
set( LIBRARY_OUTPUT_PATH    "${FPS_BUILD_DIR}/${FPS_LIB_DIR_NAME}" )   # Compiled libraries dir.
if( NOT IS_DIRECTORY ${EXECUTABLE_OUTPUT_PATH} )
  file( MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH} )
endif()
if( NOT IS_DIRECTORY ${LIBRARY_OUTPUT_PATH} )
  file( MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH} )
endif()

#-----------------------------------------------------------------------------------------------------------------
#
# List of directories to add to all compiled targets' header file search path.
#
#-----------------------------------------------------------------------------------------------------------------
set( FPS_COMMON_INCLUDE_DIRS
     ${FPS_CPP_DIR}/include
     ${FPS_CPP_DIR}/lib
     ${FPS_CPP_DIR}/app
)

#-----------------------------------------------------------------------------------------------------------------
#
# List of paths to include when searching for external libraries at compile time or run time.
#
#-----------------------------------------------------------------------------------------------------------------
set( FPS_LIB_SEARCH_PATHS 
     ${FPS_COMPILER_DIR}/lib64
     ${FPS_COMPILER_DIR}/lib
)

set( FPS_LIB_RUNTIME_PATHS
     ${LIBRARY_OUTPUT_PATH} 
     ${FPS_EXTERNAL_DIR}/lib 
     /usr/lib64
     /lib64
)

#-----------------------------------------------------------------------------------------------------------------
# Collect any/all header files that should be included with -isystem rather than -I here.
#-----------------------------------------------------------------------------------------------------------------
set( FPS_SYSTEM_INCLUDE_DIRS )

#-----------------------------------------------------------------------------------------------------------------
if( IS_DIRECTORY /usr/lib/x86_64-linux-gnu )
  list( APPEND FPS_LIB_RUNTIME_PATHS    /usr/lib/x86_64-linux-gnu )
endif()

if( IS_DIRECTORY /usr/include/x86_64-linux-gnu )
  list( APPEND FPS_COMMON_INCLUDE_DIRS /usr/include/x86_64-linux-gnu )
  list( APPEND FPS_SYSTEM_INCLUDE_DIRS /usr/include/x86_64-linux-gnu )
endif()

list( APPEND FPS_LIB_SEARCH_PATHS ${FPS_LIB_RUNTIME_PATHS} )
list( REMOVE_DUPLICATES FPS_LIB_SEARCH_PATHS )
set ( CMAKE_FIND_LIBRARY_SUFFIXES .so .a )

#-----------------------------------------------------------------------------------------------------------------
#
# List of shared external requirements.
#
#-----------------------------------------------------------------------------------------------------------------
# set( FPS_COMMON_REQUIREMENTS pthread )
set( FPS_COMMON_REQUIREMENTS rt )

#-----------------------------------------------------------------------------------------------------------------
#
# List of common internal target dependencies.
#
#-----------------------------------------------------------------------------------------------------------------
set( FPS_COMMON_DEPENDENCIES ) 

#-----------------------------------------------------------------------------------------------------------------
#
# Build targets that are valid dependencies/requirements are sorted into various global lists depending on type.
#
#-----------------------------------------------------------------------------------------------------------------
set( FPS_PROP__LIB_TARGETS    "fps.library_targets" )
set( FPS_PROP__UTIL_TARGETS    "fps.utility_targets" )
set( FPS_PROP__REQ_LABELS "fps.requirement_labels" )
set_property( GLOBAL PROPERTY ${FPS_PROP__LIB_TARGETS} )
set_property( GLOBAL PROPERTY ${FPS_PROP__UTIL_TARGETS} )
set_property( GLOBAL PROPERTY ${FPS_PROP__REQ_LABELS} ) 

#-----------------------------------------------------------------------------------------------------------------
#
# Define property labels/gorups for use with fps_add_library and fps_add_application.
#
#-----------------------------------------------------------------------------------------------------------------
set( FPS_PROP__LINK_WHOLE_ARCHIVE "fps_property.link_whole_archive" )
set( FPS_PROPERTY_LABEL__LINK_WHOLE_ARCHIVE "link_whole_archive" )
set_property( GLOBAL PROPERTY "${FPS_PROP__LINK_WHOLE_ARCHIVE}" )

#-----------------------------------------------------------------------------------------------------------------
#
# Misc output formatting variables
#
#-----------------------------------------------------------------------------------------------------------------
set( FPS_INDENT   "  "                ) # Default   message indentation.
set( FPS_ML_DELIM "'\n${FPS_INDENT}'" ) # Multiline message delimited.

#-----------------------------------------------------------------------------------------------------------------
#
# Import additional information about our compiler, git checkout, etc.
#
#-----------------------------------------------------------------------------------------------------------------
if( NOT FPS_COMPILER_VENDOR OR NOT FPS_COMPILER_VERSION )
  fps_identify_compiler( FPS_COMPILER_VENDOR FPS_COMPILER_VERSION FPS_COMPILER_LABEL )
else()
  set( FPS_COMPILER_LABEL "${FPS_COMPILER_VENDOR}-${FPS_COMPILER_VERSION}" )
endif()

string( STRIP "${FPS_COMPILER_LABEL}" FPS_COMPILER_LABEL )

#-----------------------------------------------------------------------------------------------------------------
#
# Make sure our current compiler's bin directory is in the PATH environment variable.  This ensures that any child
# scripts/processes use the same gcc/g++ binary version
#
#-----------------------------------------------------------------------------------------------------------------
set( ENV{PATH} "${FPS_COMPILER_DIR}/bin:$ENV{PATH}" )

#-----------------------------------------------------------------------------------------------------------------
#
# Create the environment string that CTest will pass to our unit/regression tests.
#
#-----------------------------------------------------------------------------------------------------------------
join( "${FPS_LIB_RUNTIME_PATHS}" ":" TEST_LD_LIB_PATH )
set( FPS_TEST_ENVIRONMENT
     "FPS_ROOT_DIR=${FPS_ROOT_DIR}"
     "FPS_CPP_DIR=${FPS_CPP_DIR}"
     "FPS_SCRIPTS_DIR=${FPS_SCRIPTS_DIR}"
     "FPSMAKE_VERSION=${FPSMAKE_VERSION}"
     "FPS_COMPILER_DIR=${FPS_COMPILER_DIR}"
     "FPS_COMPILER_VENDOR=${FPS_COMPILER_VENDOR}"
     "FPS_COMPILER_VERSION=${FPS_COMPILER_VERSION}"
     "FPS_OS_VENDOR=${FPS_OS_VENDOR}"
     "PATH=${FPS_ROOT_DIR}/scripts:${EXECUTABLE_OUTPUT_PATH}:${FPS_SCRIPTS_DIR}:/usr/bin:/bin"
     "PYTHONPATH=${FPS_PYTHON_DIR}"
     "LD_LIBRARY_PATH=${TEST_LD_LIB_PATH}"
)

#if( ${FPS_OS_VENDOR} STREQUAL "ubuntu" ) 
#  list( APPEND FPS_TEST_ENVIRONMENT "LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu:/lib64:/usr/lib" )
#endif()

#-----------------------------------------------------------------------------------------------------------------
# In verbose mode, we write a detailed logfile 
#-----------------------------------------------------------------------------------------------------------------
set( FPS_BUILD_LOG "${CMAKE_BINARY_DIR}/fpsmake.verbose.log" )
file( REMOVE ${FPS_BUILD_LOG} )
if( FPS_VERBOSE ) 
  execute_process( COMMAND date OUTPUT_VARIABLE date_string ) 
  string( STRIP "${date_string}" date_string )
  set( log_msg "${FPS_INDENT}[ Generated : ${date_string} ]\n" )
  file( WRITE ${FPS_BUILD_LOG} "${log_msg}" )
endif()

#-----------------------------------------------------------------------------------------------------------------
#
# Now configure compilation settings based on the current compilation toolset.
#
#-----------------------------------------------------------------------------------------------------------------
if( "${FPS_COMPILER_VENDOR}" STREQUAL "gcc" )
  include( fps_gcc )
elseif( "${FPS_COMPILER_VENDOR}" STREQUAL "clang" )
  # TODO: Add clang bindings
  include( fps_gcc )
else()
  set( error_msg 
       " " 
       " [ fps_constants ]"
       "   Error : Unsupported compiler ${FPS_COMPILER_VENDOR} (${FPS_COMPILER_LABEL})" 
       " "
     )
  join( "${error_msg}" "\n" error_msg )
  message( FATAL_ERROR "${error_msg}" )
endif()

#-----------------------------------------------------------------------------------------------------------------
# Enable unit testing via CTest 
#-----------------------------------------------------------------------------------------------------------------
enable_testing() 

#-----------------------------------------------------------------------------------------------------------------
#
# Log basic configuration details.
#
#-----------------------------------------------------------------------------------------------------------------
set ( log_msg 
      " "
      "[ Regenerating Build System ]" 
      "  Host OS           : ${FPS_OS_VENDOR} v${FPS_OS_VERSION}"
      "  Toolsets          : ${FPS_COMPILER_LABEL} (${FPS_COMPILER_DIR})" 
      "                    : cmake version ${CMAKE_VERSION}" 
      "                    : fpsmake version ${FPSMAKE_VERSION}"
      "  Platform Root     : ${FPS_ROOT_DIR}" 
      "  Build Plan        : ${FPS_BUILD_PLAN}"
      "  Build Type        : ${FPS_BUILD_TYPE}"
      "  Library Linkage   : ${FPS_LINK_TYPE}"
     )

if( FPS_PROFILE )
  list( APPEND log_msg "  Profiling         : enabled" ) 
endif() 

join  ( "${log_msg}" "\n" log_msg )
fps_log( "${log_msg}\n" )

if( FPS_VERBOSE ) 
 file( APPEND ${FPS_BUILD_LOG} "${FPS_INDENT}${log_msg}\n\n" ) 
endif()

