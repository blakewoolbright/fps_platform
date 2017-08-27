#----------------------------------------------------------------------------------------------------------------------------------------
# FUNCTION
#   fps_is_application_excluded
#
# SYNOPSIS
#   Given a target name and a list of plans from which the target has been explicitly excluded, determine whether the target should
#   be added to this build.
#
#----------------------------------------------------------------------------------------------------------------------------------------
function( fps_is_application_excluded  target_name  excluded_plans  _RESULT ) 
  #
  # First, exclude target if the current plan name appears in the target's list of excluded plans.
  #
  fps_list_contains( ${excluded_plans} "${FPS_BUILD_PLAN}" is_excluded )
  if( is_excluded ) 
    set( ${_RESULT} True PARENT_SCOPE ) 
    return() 
  endif() 

  #
  # Next, if FPS_CUSTOM_PLAN_TARGETS is populated, this is a custom build, so exclude target if it's not in the list.
  # 
  if( FPS_CUSTOM_PLAN_TARGETS )
    fps_list_contains( FPS_CUSTOM_PLAN_TARGETS "${target_name}" is_plan_target )
    if( is_plan_target ) 
      set( ${_RESULT} False PARENT_SCOPE ) 
    else() 
      set( ${_RESULT} "True" PARENT_SCOPE )
    endif() 
    return() 
  endif()

  #
  # Otherwise only exclude if the special keyword 'all' appears in the exclusion list.
  #
  fps_list_contains( ${excluded_plans} "all" is_excluded ) 
  if( is_excluded ) 
    set( ${_RESULT} True PARENT_SCOPE )
  else() 
    set( ${_RESULT} False PARENT_SCOPE )
  endif()

endfunction()

#----------------------------------------------------------------------------------------------------------------------------------------
function( fps_is_library_excluded  target_name  excluded_plans  _RESULT ) 
  #
  # First, exclude target if the current plan name appears in the target's list of excluded plans.
  #
  fps_list_contains( ${excluded_plans} "${FPS_BUILD_PLAN}" is_excluded )
  if( is_excluded ) 
    set( ${_RESULT} True PARENT_SCOPE ) 
    return() 
  endif() 

  #
  # Otherwise only exclude if the special keyword 'all' appears in the exclusion list.
  #
  fps_list_contains( ${excluded_plans} "all" is_excluded ) 
  if( is_excluded ) 
    set( ${_RESULT} True PARENT_SCOPE )
  else() 
    set( ${_RESULT} False PARENT_SCOPE )
  endif()

endfunction()

#----------------------------------------------------------------------------------------------------------------------------------------
# FUNCTION
#   fps_add_compiler_flags 
#
# SYNOPSIS 
#   Append additional compiler flags to the specified target's current flagset. 
# 
#----------------------------------------------------------------------------------------------------------------------------------------
function( fps_add_compiler_flags target_name )

  if( NOT target_name OR NOT TARGET ${target_name} )
    message( FATAL_ERROR "fps_add_compiler_flags :: Invoked with invalid target name '${target_name}'." )
    return ()
  endif()

  if( NOT ARGN )
    return()
  endif()

  # Note: Compiler flags are stored as a single string, so we split, append, and rejoin w/ whitespace.
  get_property( compiler_flags TARGET ${target_name} PROPERTY COMPILE_FLAGS )
  split       ( "${compiler_flags}" " " compiler_flags )
  list        ( APPEND compiler_flags ${ARGN} )

  # don't let any target-specific flags trump the special 
  # FPS_CMAKE_SUPPRESS_DEBUG_INFO-induced flags:
  if(DEFINED ENV{FPS_CMAKE_SUPPRESS_DEBUG_INFO}) 
    if ( "$ENV{FPS_CMAKE_SUPPRESS_DEBUG_INFO}" EQUAL 1)
      list( APPEND compiler_flags -g0 -fno-record-gcc-switches )
    endif()
  endif()

  join        ( "${compiler_flags}" " " compiler_flags )
  set_property( TARGET ${target_name} PROPERTY COMPILE_FLAGS "${compiler_flags}" )

endfunction()

#----------------------------------------------------------------------------------------------------------------------------------------
# FUNCTION
#   fps_add_linker_flags
#
# SYNOPSIS
#   Append additional linker flag(s) to the specified target's current flagset.
#
#----------------------------------------------------------------------------------------------------------------------------------------
function( fps_add_linker_flags target_name )

  if( NOT target_name OR NOT TARGET ${target_name} )
    message( FATAL_ERROR "fps_add_linker_flags :: Invoked with invalid target name '${target_name}'." )
    return ()
  endif()

  if( NOT ARGN )
    return()
  endif()

  # Note: Linker flags are stored as a single string, so we split, append, make distinct, and rejoin.
  get_property( linker_flags TARGET ${target_name} PROPERTY LINK_FLAGS  )
  split       ( "${linker_flags}" " " linker_flags )
  list        ( APPEND linker_flags ${ARGN} )
  list        ( REMOVE_DUPLICATES linker_flags )
  join        ( "${linker_flags}" " " linker_flags )
  set_target_properties( ${target_name} PROPERTIES LINK_FLAGS "${linker_flags}" )

endfunction()

#----------------------------------------------------------------------------------------------------------------------------------------
# FUNCTION
#   fps_add_include_dirs
#  
# SYNOPSIS
#   Append additional directories to the specified target's list of paths to search when resolving #include directives.
#
#----------------------------------------------------------------------------------------------------------------------------------------
function( fps_add_include_dirs target_name )

  if( NOT target_name OR NOT TARGET ${target_name} )
    message( FATAL_ERROR "fps_add_include_dirs :: Invoked with invalid target name '${target_name}'." )
    return ()
  endif()

  if( NOT ARGN )
    return()
  endif()

  #
  # Get existing include directories property value so we can append to it if necessary.
  #
  get_target_property( include_dirs "${target_name}" INCLUDE_DIRECTORIES )
  if( NOT include_dirs ) 
    set( include_dirs )
  endif() 

  #
  # Sort include dirs by whether they are 'system' includes (no warnings during compilation) or regular includes.
  #
  set( sys_include_dirs )
  foreach( dir ${FPS_COMMON_INCLUDE_DIRS} )
    fps_list_contains( FPS_SYSTEM_INCLUDE_DIRS "${dir}" is_system_include )
    if( NOT is_system_include ) 
      list( APPEND ARGN ${dir} ) 
    else() 
      list( APPEND sys_include_dirs "-isystem ${dir}" )
    endif() 
  endforeach()
  
  foreach( dir ${ARGN} )
    fps_list_contains( FPS_SYSTEM_INCLUDE_DIRS "${dir}" is_system_include )
    if( is_system_include ) 
      list( APPEND sys_include_dirs "-isystem ${dir}" )
    else()
      list( APPEND include_dirs ${dir} )
    endif()
  endforeach()

  if( include_dirs )
    list( REMOVE_DUPLICATES include_dirs ) 
    set_target_properties( ${target_name} PROPERTIES INCLUDE_DIRECTORIES "${include_dirs}" )
  endif() 

  if( sys_include_dirs ) 
    list( REVERSE sys_include_dirs )
    join( "${sys_include_dirs}" " " sys_include_dirs )
    get_property( compiler_flags TARGET ${target_name} PROPERTY COMPILE_FLAGS )
    set_property( TARGET ${target_name} PROPERTY COMPILE_FLAGS "${sys_include_dirs} ${compiler_flags}" )
  endif()

endfunction()

#----------------------------------------------------------------------------------------------------------------------------------------
# FUNCTION
#   fps_link_requirements 
#
# SYNOPSIS
#   Given a valid target name and a list of requirement labels or library names, resolve requirement list to 
#   specific libraries and add them to the target's linker command.
#
#----------------------------------------------------------------------------------------------------------------------------------------
function( fps_link_requirements target_name )
  if( NOT target_name OR NOT TARGET ${target_name} )
    message( FATAL_ERROR "fps_link_requirements :: Invoked with invalid target name '${target_name}'." )
    return ()
  endif()

  if( NOT ARGN )
    return()
  endif()

  get_property( all__requirement_labels GLOBAL PROPERTY ${FPS_PROPERTY_NAME__REQUIREMENT_LABELS} )

  set( required_lib_dirs ) 
  set( required_lib_names ) 
  set( required_lib_files )
  foreach( label ${ARGN} )
    fps_list_contains( all__requirement_labels "${label}" is_requirement_label )
    if( is_requirement_label ) 
      string( TOUPPER "${label}" uc_label )
      list( APPEND required_lib_dirs ${FPS_${uc_label}_LIBRARY_DIRS} )
      list( APPEND required_lib_files ${FPS_${uc_label}_LIBRARY_FILES} )
    else()
      list( APPEND required_lib_names ${label} )
    endif() 
  endforeach() 

  if( required_lib_dirs ) 
    list( REMOVE_DUPLICATES required_lib_dirs ) 
    set( required_link_flags )
    foreach( lib_dir ${required_lib_dirs} )
      list( APPEND required_link_flags "-L${lib_dir}" )
    endforeach() 
    fps_add_linker_flags( ${target_name} ${required_link_flags} )
  endif() 
  
  if( required_lib_names )
    list( REMOVE_DUPLICATES required_lib_names ) 
    foreach( lib_name ${required_lib_names} ) 
      target_link_libraries( ${target_name} ${lib_name} )
    endforeach() 
  endif()

  if( required_lib_files )
    list( REMOVE_DUPLICATES required_lib_files ) 
    foreach( lib_file ${required_lib_files} ) 
      target_link_libraries( ${target_name} ${lib_file} ) 
    endforeach() 
  endif() 
endfunction()   

#----------------------------------------------------------------------------------------------------------------------------------------
# FUNCTION
#   fps_get_required_library_names
#
# SYNOPSIS
#   Given a valid target name and a list of requirement labels or library names, resolve to just library names.
#
#----------------------------------------------------------------------------------------------------------------------------------------
function( fps_get_required_library_names _RESULT )
  if( NOT ARGN )
    set( ${_RESULT} PARENT_SCOPE )
    return()
  endif()

  set( results_list )
  get_property( all__requirement_labels GLOBAL PROPERTY ${FPS_PROPERTY_NAME__REQUIREMENT_LABELS} )
  foreach( label ${ARGN} )
    fps_list_contains( all__requirement_labels "${label}" is_requirement_label )
    if( is_requirement_label ) 
      string( TOUPPER "${label}" uc_label )
      set( label_required_lib_names ${FPS_${uc_label}_LIBRARY_NAMES} )
      if( label_required_lib_names )
        list( APPEND results_list ${label_required_lib_names} )
      endif()
    else()
      list( APPEND results_list ${label} )
    endif() 
  endforeach() 
  if( results_list ) 
    list( REMOVE_DUPLICATES results_list )
  endif()
  set( ${_RESULT} ${results_list} PARENT_SCOPE ) 
endfunction()   

#----------------------------------------------------------------------------------------------------------------------------------------
# FUNCTION
#   fps_get_required_library_files
#
# SYNOPSIS
#   Given a valid target name and a list of requirement labels or library names, resolve requirement list to 
#   specific libraries and return it.
#
# NOTE: This is essential for cython builds.
#
#----------------------------------------------------------------------------------------------------------------------------------------
function( fps_get_required_library_files _RESULT )
  if( NOT ARGN )
    set( ${_RESULT} PARENT_SCOPE )
    return()
  endif()

  set( results_list )
  get_property( all__requirement_labels GLOBAL PROPERTY ${FPS_PROPERTY_NAME__REQUIREMENT_LABELS} )
  foreach( label ${ARGN} )
    fps_list_contains( all__requirement_labels "${label}" is_requirement_label )
    if( is_requirement_label ) 
      string( TOUPPER "${label}" uc_label )
      set( label_required_lib_files ${FPS_${uc_label}_LIBRARY_FILES} )
      if( label_required_lib_files )
        list( APPEND results_list ${label_required_lib_files} )
      endif()
    endif() 
  endforeach() 
  if( results_list ) 
    list( REMOVE_DUPLICATES results_list )
  endif()
  set( ${_RESULT} ${results_list} PARENT_SCOPE ) 
endfunction()   

#----------------------------------------------------------------------------------------------------------------------------------------
# FUNCTION
#   fps_get_required_library_dirs
#
# SYNOPSIS
#   Given a valid target name and a list of requirement labels or library names, resolve requirement list to 
#   a list of directories to include in the linker's search path.
#
#----------------------------------------------------------------------------------------------------------------------------------------
function( fps_get_required_library_dirs _RESULT )
  if( NOT ARGN )
    set( ${_RESULT} PARENT_SCOPE )
    return()
  endif()

  set( results_list ${FPS_LIB_SEARCH_PATHS} )
  get_property( all__requirement_labels GLOBAL PROPERTY ${FPS_PROPERTY_NAME__REQUIREMENT_LABELS} )
  foreach( label ${ARGN} )
    fps_list_contains( all__requirement_labels "${label}" is_requirement_label )
    if( is_requirement_label ) 
      string( TOUPPER "${label}" uc_label )
      set( label_required_lib_dirs ${FPS_${uc_label}_LIBRARY_DIRS} )
      if( label_required_lib_dirs )
        list( APPEND results_list ${label_required_lib_dirs} )
      endif()
    endif() 
  endforeach() 
  if( results_list ) 
    list( REMOVE_DUPLICATES results_list )
  endif()
  set( ${_RESULT} ${results_list} PARENT_SCOPE ) 
endfunction()   


#----------------------------------------------------------------------------------------------------------------------------------------
# FUNCTION 
#   fps_link_dependencies
# 
# SYNOPSIS
#   Given a valid target and a list of internal library names, create a build dependency between target and all libraries
#   and add libraries to target's linker command. 
#   
#----------------------------------------------------------------------------------------------------------------------------------------
function( fps_link_dependencies target_name )

  if( NOT target_name OR NOT TARGET ${target_name} )
    message( FATAL_ERROR "fps_link_dependencies :: Invoked with invalid target name '${target_name}'." )
    return ()
  endif()

  if( NOT ARGN )
    return()
  endif()

  #
  # Fetch a list of all libraries where 'whole archive' linkage is required
  #
  get_property( fps__library_targets    GLOBAL PROPERTY ${FPS_PROPERTY_NAME__LIBRARY_TARGETS} ) 
  get_property( pg__link_whole_archive  GLOBAL PROPERTY ${FPS_PROPERTY_GROUP__LINK_WHOLE_ARCHIVE} )
  foreach( dep ${ARGN} )
    #
    # Header only libraries are bound to custom targets of type UTILITY.
    # We don't want to ask cmake to link with these, so check the type before asking.
    #
    fps_list_contains( fps__library_targets ${dep} is_fps_library )
    if( is_fps_library )
      #
      # Determine whether to apply the 'whole archive linkage' property and link with the library
      #
      fps_list_contains( pg__link_whole_archive ${dep} use_whole_archive_linkage )
      if( NOT BUILD_SHARED_LIBS AND use_whole_archive_linkage )
        target_link_libraries( ${target_name} "-Wl,--whole-archive ${LIBRARY_OUTPUT_PATH}/lib${dep}.a -Wl,--no-whole-archive" )
        target_link_libraries( ${target_name} ${dep} )
      else()
        target_link_libraries( ${target_name} ${dep} )
      endif()
    endif()
    add_dependencies( ${target_name} ${dep} )
  endforeach()

endfunction()

#----------------------------------------------------------------------------------------------------------------------------------------
# FUNCTION
#   fps_get_required_include_dirs 
#  
# SYNOPSIS
#   Given a list of requirement labels, return a list of include directories associated with these requirements.
#
#----------------------------------------------------------------------------------------------------------------------------------------
function( fps_get_required_include_dirs _RESULT )
  if( NOT ARGN ) 
    set( ${_RESULT} PARENT_SCOPE ) 
    return() 
  endif() 

  set( include_dirs ) 

  foreach( label ${ARGN} )
    string( TOUPPER "${label}" uc_label )
    if( FPS_${uc_label}_INCLUDE_DIRS ) 
      list( APPEND include_dirs ${FPS_${uc_label}_INCLUDE_DIRS} )
    endif() 
  endforeach() 

  if( include_dirs ) 
    list( REMOVE_DUPLICATES include_dirs )
  endif()

  set( ${_RESULT} "${include_dirs}" PARENT_SCOPE )
endfunction()

#----------------------------------------------------------------------------------------------------------------------------------------
# FUNCTION
#   fps_get_required_compiler_flags
#  
# SYNOPSIS
#   Given a list of requirement labels, return a list of compiler flags associated with these requirements.
#
#----------------------------------------------------------------------------------------------------------------------------------------
function( fps_get_required_compiler_flags _RESULT )
  if( NOT ARGN ) 
    set( ${_RESULT} PARENT_SCOPE ) 
    return() 
  endif() 

  set( compiler_flags ) 

  foreach( label ${ARGN} )
    string( TOUPPER "${label}" uc_label )
    if( FPS_${uc_label}_COMPILER_FLAGS ) 
      list( APPEND compiler_flags ${FPS_${uc_label}_COMPILER_FLAGS} )
    endif() 
  endforeach() 

  if( compiler_flags ) 
    list( REMOVE_DUPLICATES compiler_flags )
  endif()

  set( ${_RESULT} "${compiler_flags}" PARENT_SCOPE )
endfunction()


