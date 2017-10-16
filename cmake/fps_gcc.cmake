#
# Define default compiler/linker flags for GCC based builds
#
set( FPS_COMMON_COMPILER_FLAGS
     -march=native
     -mtune=generic
     -fdiagnostics-show-option
     -fno-strict-aliasing
     -Wall
     -Wextra
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
     -Wno-empty-body
   )

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
  list( APPEND FPS_COMMON_COMPILER_FLAGS -g3 )
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
split( "${FPS_CXX_COMPILER_VERSION}" "." version_numbers )
list( GET version_numbers 0 FPS_COMPILER_MAJOR_VERSION ) 
list( GET version_numbers 1 FPS_COMPILER_MINOR_VERSION ) 
list( GET version_numbers 2 FPS_COMPILER_MINOR_REVISION )

# Add extended/version specific compiler flags 
list( APPEND FPS_COMMON_COMPILER_FLAGS 
      -std=c++17
      -frecord-gcc-switches 
      -Wno-delete-non-virtual-dtor
      -Wno-narrowing
      -Wno-maybe-uninitialized
      -Wno-unused-local-typedefs
    )

# Skip extended optimization flags for now
if( False ) 
  if( ${FPS_BUILD_TYPE} STREQUAL "optimized" )
    list( APPEND FPS_COMMON_COMPILER_FLAGS
          -flto
          -fwhole-program
          -fdevirtualize
        )
    list( APPEND FPS_COMMON_LINKER_FLAGS -flto )
  endif()
endif()

#else() 
#  set( error_msg 
#       "  "
#       " [ fps_gcc ]"
#       "   Error :: Unsupported GCC compiler version '${FPS_CXX_COMPILER_VERSION}'"
#       "   Major :: ${FPS_COMPILER_MAJOR_VERSION}"
#       "   Minor :: ${FPS_COMPILER_MINOR_VERSION}"
#       "   Rev   :: ${FPS_COMPILER_MINOR_REVISION}"
#       " "
#  )
#  join( "${error_msg}" "\n" error_msg ) 
#  message( FATAL_ERROR "${error_msg}" )
#endif()


#-----------------------------------------------------------------------------------------------------------------
# Convert list of default library search paths to list of linker appropriate -L directives.
#-----------------------------------------------------------------------------------------------------------------
foreach( path ${FPS_LIB_SEARCH_PATHS} )
  list( APPEND FPS_COMMON_LINKER_FLAGS "-L${path}" )
endforeach()




