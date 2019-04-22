
fps_add_application( 
  NAME            basic_compilation.test
  FILES           basic_compilation.test.cpp
)

fps_add_application( 
  NAME            boost_compilation.test
  REQUIRES        boost
  DEPENDS         fps_string
  PROFILE
  FILES           boost_compilation.test.cpp
)

add_subdirectory( fps_net ) 
# add_subdirectory( cubist.fib ) 
add_subdirectory( atlassian )

if( False ) 
    fps_add_application( 
      NAME            devirtualization_test.off.case_00
      COMPILER_FLAGS  -DCASE_1_ENABLED
                      -DFORCE_INLINE
                      -fverbose-asm
      FILES           devirtualization_test.cpp
                      devirtualization_test_types.cpp
    )

    fps_add_application( 
      NAME            devirtualization_test.on.case_01
      COMPILER_FLAGS  -DCASE_1_ENABLED
                      -fdevirtualize
                      -flto
                      -fwhole-program
                      -fverbose-asm
      LINKER_FLAGS    -flto
      FILES           devirtualization_test.cpp
                      devirtualization_test_types.cpp
    )

    fps_add_application( 
      NAME            devirtualization_test.off_inline.case_01
      COMPILER_FLAGS  -DCASE_1_ENABLED
                      -fverbose-asm
      FILES           devirtualization_test.cpp
                      devirtualization_test_types.cpp
    )

    fps_add_application( 
      NAME            devirtualization_test.on_inline.case_01
      COMPILER_FLAGS  -DCASE_1_ENABLED
                      -DFORCE_INLINE 
                      -fdevirtualize
                      -flto
                      -fwhole-program
                      -fverbose-asm
      LINKER_FLAGS    -flto
      FILES           devirtualization_test.cpp
    )

    fps_add_application( 
      NAME            devirtualization_test.off.case_02
      COMPILER_FLAGS  -DCASE_2_ENABLED
                      -fverbose-asm
      FILES           devirtualization_test.cpp
                      devirtualization_test_types.cpp
    )

    fps_add_application( 
      NAME            devirtualization_test.on.case_02
      COMPILER_FLAGS  -DCASE_2_ENABLED
                      -fdevirtualize
                      -flto
                      -fwhole-program
                      -fverbose-asm
      LINKER_FLAGS    -flto
      FILES           devirtualization_test.cpp
                      devirtualization_test_types.cpp
    )
endif()

