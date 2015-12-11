#---------------------------------------------------------------------------------
#
# Collection of cmake utility functions that aren't specific to Headlands.
#
#---------------------------------------------------------------------------------
function ( list_cmake_variables_with_prefix prefix _Result )
  get_cmake_property( var_list VARIABLES )
  string ( REGEX MATCHALL "(^|;)${prefix}[A-Za-z0-9_]*" match_list "${var_list}" )
  set    ( ${_Result} ${match_list} PARENT_SCOPE )
endfunction()

#---------------------------------------------------------------------------------
# Join list -> string
#---------------------------------------------------------------------------------
function( join target delimiter _RESULT )
  string( REGEX REPLACE "([^\\]|^);" "\\1${delimiter}" TMP_STR "${target}" )
  string( REGEX REPLACE "[\\](.)" "\\1" TMP_STR "${TMP_STR}" )
  set   ( ${_RESULT} "${TMP_STR}" PARENT_SCOPE )
endfunction()

#---------------------------------------------------------------------------------
# Split string -> list
#---------------------------------------------------------------------------------
function( split target delimiter _RESULT )
  string( REPLACE "${delimiter}" ";" TMP_STR "${target}" )
  set   ( ${_RESULT} "${TMP_STR}" PARENT_SCOPE )
endfunction()

#---------------------------------------------------------------------------------
# Stores 'value'+1 in _RESULT
#---------------------------------------------------------------------------------
macro( increment value _RESULT )
  math(EXPR ${_RESULT} "${value} + 1")
endmacro( increment )

#---------------------------------------------------------------------------------
# Stores 'value'-1 in _RESULT
#---------------------------------------------------------------------------------
macro( decrement value _RESULT )
  math(EXPR ${_RESULT} "${value} - 1")
endmacro( decrement )

#---------------------------------------------------------------------------------
# Create a symbolic link
#---------------------------------------------------------------------------------
macro( fps_create_symlink target_name link_name ) 
  if( "${target_name}" STREQUAL "" OR "${link_name}" STREQUAL "" ) 
    set( error_msg 
         " "
         " [ fps_create_symlink ]"
         "   Target      : required : '${target_name}'"
         "   Link        : required : '${link_name}'"
         "   FATAL ERROR : 'You must specify both a target file and link name.'" 
    )
    join( "${error_msg}" "\n" error_msg ) 
    message( FATAL_ERROR "\n${error_msg}\n" )
  endif()
  execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink ${target_name} ${link_name} )
endmacro( fps_create_symlink )

#---------------------------------------------------------------------------------
#
# Log message wrapper functions.
#
#---------------------------------------------------------------------------------
function( fps_log ) 
  string( REPLACE "\n" "\n-- " log_msg_txt "${ARGN}" )
  message( STATUS "${log_msg_txt}" )
endfunction( fps_log )

#---------------------------------------------------------------------------------
#
# Convert a filename like 'libboost_xyz.a', 'libboost_xyz.so', 'libboost_xyz.so.1' 
# to the a pure library name 'boost_xyz'
#
#---------------------------------------------------------------------------------
function( fps_library_file_to_library_name lib_file _RESULT )
  string( REGEX REPLACE "^lib" "" lib_name ${lib_file} )
  string( REGEX REPLACE "[.]a$|[.]so$|[.]so([.][0-9]+)*$" "" lib_name ${lib_name} )
  set( ${_RESULT} "${lib_name}" PARENT_SCOPE )
endfunction()
  
#---------------------------------------------------------------------------------
# Sets _RESULT true if 'value' is in the list indicated by 'list_name'.
#---------------------------------------------------------------------------------
function( fps_list_contains list_name value _RESULT )
  list( FIND ${list_name} ${value} value_idx )
  if( value_idx EQUAL -1 )
    set( ${_RESULT}  False  PARENT_SCOPE )
  else()
    set( ${_RESULT}  True   PARENT_SCOPE )
  endif()
endfunction()

#---------------------------------------------------------------------------------
# Returns intersection of 'list1' and 'list2' via _RESULT
#---------------------------------------------------------------------------------
macro( intersection list1 list2 _RESULT )
  set(intersect_tmp "")
  foreach(l ${list1})
    if("${list2}" MATCHES "(^|;)${l}(;|$)")
      set(intersect_tmp ${intersect_tmp} ${l})
    endif("${list2}" MATCHES "(^|;)${l}(;|$)")
  endforeach(l)
  set( ${_RESULT} ${intersect_tmp} PARENT_SCOPE )
endmacro( intersection )

#------------------------------------------------------------------------------------
# fps_disable_warnings
# Disables compiler warnings.
#------------------------------------------------------------------------------------
macro( fps_disable_warnings )
  add_definitions( ${CMAKE_CXX_FLAGS} "-w" )
endmacro()

#------------------------------------------------------------------------------------
# Identify Host OS Flavor
#------------------------------------------------------------------------------------
function( fps_os_vendor _RESULT ) 
  execute_process( COMMAND lsb_release -a OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE cmd_output ERROR_QUIET )
  split( "${cmd_output}" "\n" cmd_output )  
  string( TOLOWER "${cmd_output}" cmd_output )
  foreach( line ${cmd_output} ) 
    string( REGEX MATCH "^Description:" is_match "${line}" ) 
    if( is_match ) 
      string( REGEX REPLACE "^Description:(.*)$" "\\1" line "${line}" ) 
      string( STRIP ${line} line )
      set( ${_RESULT} ${line} PARENT_SCOPE ) 
      return()
    endif()
  endforeach()
 
  execute_process( COMMAND head -1 /etc/issue OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE cmd_output ERROR_QUIET ) 
  string( REPLACE "\\n" "" cmd_output "${cmd_output}" ) 
  string( REPLACE "\\r" "" cmd_output "${cmd_output}" ) 
  string( REPLACE "\\l" "" cmd_output "${cmd_output}" ) 
  string( STRIP "${cmd_output}" cmd_output )
  
  if( NOT "${cmd_output}" STREQUAL "" ) 
    set( ${_RESULT} ${cmd_output} PARENT_SCOPE )
  else()
    set( ${_RESULT} "UNKNOWN-OS-VENDOR" PARENT_SCOPE )
  endif() 

endfunction() 


#------------------------------------------------------------------------------------
# Identify Compiler Version
#------------------------------------------------------------------------------------
function( fps_compiler_version _RESULT )
  execute_process( COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                   OUTPUT_STRIP_TRAILING_WHITESPACE
                   OUTPUT_VARIABLE compiler_ver )
  # string( REGEX REPLACE "([0-9])\\.([0-9])(\\.[0-9])?" "\\1.\\2" compiler_ver ${compiler_ver} )
  string( REGEX REPLACE "([0-9])\\.([0-9])\\.([0-9])?" "\\1.\\2.\\3" compiler_ver ${compiler_ver} ) 
  set( ${_RESULT} ${compiler_ver} PARENT_SCOPE )
endfunction()

#------------------------------------------------------------------------------------
# Retrieve a brief description of the compiler and store in _RESULT.
#------------------------------------------------------------------------------------
function( fps_compiler_description _RESULT )
  execute_process( COMMAND ${CMAKE_CXX_COMPILER} -v OUTPUT_STRIP_TRAILING_WHITESPACE
                   ERROR_VARIABLE compiler_desc )
  split( ${compiler_desc}  "\n"  compiler_desc )
  list( REMOVE_ITEM compiler_desc "" )
  set( is_match "" )
  foreach( line ${compiler_desc} )
    string( REGEX MATCH "^clang " is_match ${line} )
    if( NOT ${is_match} STREQUAL "" )
      set( ${_RESULT} ${line} PARENT_SCOPE )
      return()
    endif()
    
    string( REGEX MATCH "^gcc " is_match ${line} )
    if( NOT ${is_match} STREQUAL "" )
      set( ${_RESULT} ${line} PARENT_SCOPE )
      return()
    endif()
  endforeach()
endfunction()

#------------------------------------------------------------------------------------
# Retrieve various identifying strings from compiler version info.
#------------------------------------------------------------------------------------
function( fps_identify_compiler _VENDOR _VERSION _DESCRIPTION )
  execute_process( COMMAND ${CMAKE_CXX_COMPILER} -v OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_VARIABLE long_desc )
  split( ${long_desc}  "\n"  long_desc )
  list( REMOVE_ITEM long_desc "" )
  set( is_match "" )
  foreach( line ${long_desc} )
    string( REGEX MATCH "^clang " is_match ${line} )
    if( NOT ${is_match} STREQUAL "" )
      split( ${line} " " words )
      list( GET words 2 version )
      set( ${_VENDOR} "clang" PARENT_SCOPE )
      set( ${_VERSION} ${version} PARENT_SCOPE )
      set( ${_DESCRIPTION} ${line} PARENT_SCOPE )
      return()
    endif()
    
    string( REGEX MATCH "^gcc " is_match ${line} )
    if( NOT ${is_match} STREQUAL "" )
      split( ${line} " " words )
      list( GET words 2 version )
      set( ${_VENDOR} "gcc" PARENT_SCOPE )
      set( ${_VERSION} ${version} PARENT_SCOPE )
      set( ${_DESCRIPTION} ${line} PARENT_SCOPE )
      return()
    endif()
  endforeach()
endfunction()

#------------------------------------------------------------------------------------
# Retrieve the current git branch and store in _RESULT.
#------------------------------------------------------------------------------------
function( fps_git_branch _RESULT )
  execute_process( COMMAND git branch --no-color
                   OUTPUT_STRIP_TRAILING_WHITESPACE
                   OUTPUT_VARIABLE git_branch )

  string( REGEX REPLACE "^[^*]" "" git_branch ${git_branch} )
  string( REGEX REPLACE "\\* (.*)" "\\1" git_branch ${git_branch} )
  split( ${git_branch} "\n" git_branch )
  list( GET git_branch 0 git_branch )

  set   ( ${_RESULT} ${git_branch} PARENT_SCOPE )
endfunction()

#------------------------------------------------------------------------------------
# Retrieve the current git commit and store in _RESULT.
#------------------------------------------------------------------------------------
function( fps_git_commit _RESULT )
  execute_process( COMMAND git log -n1 --pretty=format:\"%H\"
                   OUTPUT_STRIP_TRAILING_WHITESPACE
                   OUTPUT_VARIABLE git_commit )

  string( REGEX  MATCH  "([^\"]+)"  git_commit  ${git_commit} )
  set   ( ${_RESULT} ${git_commit} PARENT_SCOPE )
endfunction()

#------------------------------------------------------------------------------------
# Retrieve the current git description wrt the cm-* tags and store in _RESULT.
#------------------------------------------------------------------------------------
function( fps_git_description _RESULT )
  execute_process( 
    COMMAND git describe --match "cg-[0-9]*" --tags
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE git_description 
  )

  if( NOT git_description ) 
    execute_process( 
      COMMAND git describe --match "ub[0-9]*-[0-9]*" --tags
      OUTPUT_STRIP_TRAILING_WHITESPACE
      OUTPUT_VARIABLE git_description 
    )
  endif()

  set( ${_RESULT} ${git_description} PARENT_SCOPE )

endfunction()

#------------------------------------------------------------------------------------
# Retrieve bool indicating whether or not there are uncommitted source code changes and store in _RESULT.
#------------------------------------------------------------------------------------
function( fps_git_modified _RESULT )
  execute_process( COMMAND git status --porcelain
                   OUTPUT_STRIP_TRAILING_WHITESPACE
                   OUTPUT_VARIABLE git_modified )

  if( git_modified EQUAL "" )
    set( ${_RESULT}  "false"  PARENT_SCOPE )
  else()
    set( ${_RESULT}  "true"   PARENT_SCOPE )
  endif()
endfunction()

#------------------------------------------------------------------------------------
# Retrieve the current time and store in _RESULT.
# Example : "Thu, 10 May 2012 11:08:56 -0400"
#------------------------------------------------------------------------------------
function( fps_date_and_time _RESULT )
  execute_process( COMMAND date -R OUTPUT_STRIP_TRAILING_WHITESPACE
                   OUTPUT_VARIABLE timestamp_now )
  set( ${_RESULT} ${timestamp_now} PARENT_SCOPE )
endfunction()

#------------------------------------------------------------------------------------
# Parse a cmake style list of arguments.
# SET( arguments
#      hello 
#      OPTION3 world
#      LIST3  element1  element2 
#      OPTION2
#      LIST1 element1  element2  )
#
# fps_parse_argument_list( ARG "LIST1;LIST2;LIST3" "OPTION1;OPTION2;OPTION3" ${arguments} )
#
# Results in 7 distinct variables:
#  * ARG_DEFAULT_ARGS: hello;world
#  * ARG_LIST1: fuz;baz
#  * ARG_LIST2:
#  * ARG_LIST3: foo;bar
#  * ARG_OPTION1: FALSE
#  * ARG_OPTION2: TRUE
#  * ARG_OPTION3: TRUE
#------------------------------------------------------------------------------------
macro( fps_parse_argument_list  prefix  arg_names  option_names )
  set( default_args )

  foreach(arg_name ${arg_names} )
    set( ${prefix}_${arg_name} )
  endforeach(arg_name)

  foreach( option ${option_names} )
    set( ${prefix}_${option}  false )
  endforeach(option)

  set( current_arg_name DEFAULTS )
  set( current_arg_list )

  foreach( arg ${ARGN} )
    set( larg_names ${arg_names} )
    list( FIND larg_names "${arg}" is_arg_name )
    if( is_arg_name GREATER -1 )
      set( ${prefix}_${current_arg_name} ${current_arg_list} )
      set( current_arg_name ${arg} )
      set( current_arg_list )
    else() 
      set ( loption_names  ${option_names} )
      list( FIND loption_names "${arg}" is_option )
      if( is_option GREATER -1 )
        set( ${prefix}_${arg} true )
      else() 
        set( current_arg_list ${current_arg_list} ${arg} )
      endif()
    endif() 
  endforeach() 

  set( ${prefix}_${current_arg_name} ${current_arg_list} )
endmacro() 

#------------------------------------------------------------------------------------
# Wrapper for find_library
# Disallows searches along cmake's default paths.
#------------------------------------------------------------------------------------
function( fps_find_libraries )
  fps_parse_argument_list( arg "NAMES;PATHS;RESULT" "IGNORE_CACHE;REQUIRED" ${ARGN} )
  set( lib_names ${arg_NAMES} )
  set( lib_paths ${arg_PATHS} )
  set( _RESULT   ${arg_RESULT} )

  set( lib_required "" )
  if( arg_REQUIRED )
    set( lib_required REQUIRED )
  endif()
 
  set( lib_files )
  if( arg_IGNORE_CACHE )  
    unset( ${_RESULT} CACHE )
  endif() 

  # foreach( lib ${lib_names} )
  find_library( ${_RESULT} NAMES ${lib_names} PATHS ${lib_paths} ${lib_required} NO_DEFAULT_PATH )
  message( "\nSearched for ${lib_names} in ${lib_paths}, found ${${_RESULT}}, stored in ${_RESULT}\n" )
  list( APPEND lib_files ${${_RESULT}} )

  set( ${_RESULT}  ${lib_files}  PARENT_SCOPE )
endfunction()


