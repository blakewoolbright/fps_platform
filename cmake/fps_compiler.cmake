#-----------------------------------------------------------------------------------------------------------------
#
# Now configure compilation settings based on the current compilation toolset.
#
#-----------------------------------------------------------------------------------------------------------------
if( "${FPS_COMPILER_VENDOR}" STREQUAL "gcc" )
  include( fps_gcc )
elseif( "${FPS_COMPILER_VENDOR}" STREQUAL "clang" )
  include( fps_clang )
else()
  set( error_msg 
       " " 
       " [ fps_compiler ]"
       "   Error : Unsupported compiler ${FPS_COMPILER_VENDOR} (${FPS_COMPILER_LABEL})" 
       " "
     )
  join( "${error_msg}" "\n" error_msg )
  message( FATAL_ERROR "${error_msg}" )
endif()

#-----------------------------------------------------------------------------------------------------------------
# Decide whether to enable embedded build info 
#-----------------------------------------------------------------------------------------------------------------
if( FPS_BUILD_INFO_FILE ) 
  list( APPEND FPS_COMMON_COMPILER_FLAGS "-DFPS__ENABLE_BUILD_INFO" )
endif()

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
        "[ GCC Configuration : v${FPS_CXX_COMPILER_VERSION} ]"
      )
  join( "${FPS_COMMON_COMPILER_FLAGS}" ${ml_delim} msg_txt ) 
  list( APPEND log_msg "  Compiler Flags : '${msg_txt}'" )
  join( "${FPS_COMMON_LINKER_FLAGS}" ${ml_delim} msg_txt )
  list( APPEND log_msg "  Linker Flags   : '${msg_txt}'" )
  join( "${FPS_COMMON_INCLUDE_DIRS}" ${ml_delim} msg_txt ) 
  list( APPEND log_msg "  Include Dirs   : '${msg_txt}'" )
  join( "${log_msg}" "\n${FPS_INDENT}" log_msg )
endif()
