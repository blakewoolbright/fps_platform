#
# Add internal libraries to build
#
add_subdirectory( lib  )

#
# Add example applications to build 
#
add_subdirectory( examples ) 

#
# Add production applications to build 
#
add_subdirectory( app  )

#
# Add unit tests to build
#
add_subdirectory( test ) 
