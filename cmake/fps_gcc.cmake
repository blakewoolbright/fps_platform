
#
# Define default compiler/linker flags for GCC based builds
#
set( FPS_COMMON_COMPILER_FLAGS
     -march=native
     -mtune=generic
     -fdiagnostics-show-option
     -frecord-gcc-switches
     -fno-strict-aliasing
     -Wall
     -Wunused-variable
     -Wno-unused-function
     -Wno-unused-parameter
     -Wno-missing-field-initializers
     -Wno-comment
     -Wno-sign-compare
     -Wno-ignored-qualifiers
     -Werror
     -Wtype-limits
     -Wformat
     -Wfatal-errors
     -Wextra
     -Wno-empty-body
)

if( FPS_BUILD_INFO_FILE ) 
  list( APPEND FPS_COMMON_COMPILER_FLAGS "-DFPS__ENABLE_BUILD_INFO" )
endif()

#-----------------------------------------------------------------------------------------------------------------
#
# Use position independent code when working in share library mode
#
#-----------------------------------------------------------------------------------------------------------------
if( BUILD_SHARED_LIBS )
  list( APPEND FPS_COMMON_COMPILER_FLAGS "-fPIC" )
endif()

#-----------------------------------------------------------------------------------------------------------------
#
# Specify the appropriate optimization and debug info levels.
#
#-----------------------------------------------------------------------------------------------------------------
if( "${FPS_BUILD_TYPE}" STREQUAL "optimized" )
  list( APPEND FPS_COMMON_COMPILER_FLAGS -O3 -g1 )
elseif( "${FPS_BUILD_TYPE}" STREQUAL "debug" )
  list( APPEND FPS_COMMON_COMPILER_FLAGS -O -g3 )
else()
  set( error_msg 
       " " 
       " [ fps_gcc ]"
       "   Error :: FPS_BUILD_TYPE=${FPS_BUILD_TYPE} (Should be 'optimized' or 'debug')." 
       " "
     )
  join( "${error_msg}" "\n" error_msg )
  message( FATAL_ERROR "${error_msg}" )
endif()

set( FPS_COMMON_LINKER_FLAGS ) 

#-----------------------------------------------------------------------------------------------------------------
#
# Handle any version specific compiler configuration logic below.
#
#-----------------------------------------------------------------------------------------------------------------
split( "${FPS_COMPILER_VERSION}" "." version_numbers )
list( GET version_numbers 0 FPS_GCC_MAJOR_VERSION ) 
list( GET version_numbers 1 FPS_GCC_MINOR_VERSION ) 
list( GET version_numbers 2 FPS_GCC_MINOR_REVISION )

if( FPS_GCC_MAJOR_VERSION EQUAL 5 )

  list( APPEND FPS_COMMON_COMPILER_FLAGS 
    -std=c++11
    -Wno-unused-but-set-variable 
    -Wno-delete-non-virtual-dtor
    -Wno-narrowing
    -Wno-maybe-uninitialized
    -Wno-unused-local-typedefs
  )
  
  if( ${FPS_BUILD_TYPE} STREQUAL "optimized" )
    list( APPEND FPS_COMMON_COMPILER_FLAGS
          -flto
          -fwhole-program
          -fdevirtualize
        )
    list( APPEND FPS_COMMON_LINKER_FLAGS -flto )
  endif()

else() 
  set( error_msg 
       "  "
       " [ fps_gcc ]"
       "   Error :: Unsupported GCC compiler version '${FPS_COMPILER_VERSION}'"
       "   Major :: ${FPS_GCC_MAJOR_VERSION}"
       "   Minor :: ${FPS_GCC_MINOR_VERSION}"
       "   Rev   :: ${FPS_GCC_MINOR_REVISION}"
       " "
  )
  join( "${error_msg}" "\n" error_msg ) 
  message( FATAL_ERROR "${error_msg}" )
endif()

#-----------------------------------------------------------------------------------------------------------------
# Convert list of default library search paths to list of linker appropriate -L directives.
#-----------------------------------------------------------------------------------------------------------------
foreach( path ${FPS_LIB_SEARCH_PATHS} )
  list( APPEND FPS_COMMON_LINKER_FLAGS "-L${path}" )
endforeach()

#-----------------------------------------------------------------------------------------------------------------
# Remove any duplicate flags.
#-----------------------------------------------------------------------------------------------------------------
list( REMOVE_DUPLICATES FPS_COMMON_COMPILER_FLAGS )
list( REMOVE_DUPLICATES FPS_COMMON_LINKER_FLAGS )

#-----------------------------------------------------------------------------------------------------------------
#
# Publish verbose configuration details
#
#-----------------------------------------------------------------------------------------------------------------
if( FPS_VERBOSE )
  set ( ml_delim "'\n                   + '" ) 
  set ( log_msg 
        " "  
        "[ GCC Configuration : v${FPS_COMPILER_VERSION} ]"
      )
  join( "${FPS_COMMON_COMPILER_FLAGS}" ${ml_delim} msg_txt ) 
  list( APPEND log_msg "  Compiler Flags : '${msg_txt}'" )
  join( "${FPS_COMMON_LINKER_FLAGS}" ${ml_delim} msg_txt )
  list( APPEND log_msg "  Linker Flags   : '${msg_txt}'" )
  join( "${FPS_COMMON_INCLUDE_DIRS}" ${ml_delim} msg_txt ) 
  list( APPEND log_msg "  Include Dirs   : '${msg_txt}'" )
  join( "${log_msg}" "\n${FPS_INDENT}" log_msg )
endif()




