cmake_minimum_required (VERSION 2.6)
project (LinReg)

set( CMAKE_VERBOSE_MAKEFILE on )
set(UTIL ~/code_masterarbeit/code_semesterprojekt/leos_code/util)
set(OBLIVCH /home/turing/code_masterarbeit/code_semesterprojekt/obliv-c/src/ext/oblivc)
set (CMAKE_CXX_STANDARD 11)

add_definitions(-Wall -g -std=c99 -D_POSIX_C_SOURCE=199309L)

#-----------------------------------------------------------------
find_program(FOO_EXECUTABLE foo)
function(preprocess_oc out_var)
  set(result)
  foreach(in_f ${ARGN})

    #message(STATUS "${in_f}")
    file(RELATIVE_PATH out_f ${CMAKE_CURRENT_SOURCE_DIR} ${in_f})
    set(out_oo "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${CMAKE_PROJECT_NAME}.dir/${out_f}.oo")
    set(out_od "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${CMAKE_PROJECT_NAME}.dir/${out_f}.od")
    #message(STATUS "${out_oo}")
    #message(STATUS "${out_od}")
    #message(STATUS "${CMAKE_C_COMPILER} -c ${CMAKE_C_FLAGS} ${in_f} -o ${out_oo} -I ${OBLIVCH}")
    #message(STATUS "${CMAKE_C_COMPILER} -MM ${CMAKE_C_FLAGS} ${in_f} -MT ${out_oo} > ${out_od}")

    add_custom_command(OUTPUT ${out_oo} ${out_od}
      COMMAND ${CMAKE_C_COMPILER} -c ${CMAKE_C_FLAGS} ${in_f} -o ${out_oo} -I ${OBLIVCH}
      COMMAND ${CMAKE_C_COMPILER} -MM ${CMAKE_C_FLAGS} ${in_f} -MT ${out_oo} > ${out_od}

	  DEPENDS ${in_f}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Creating preprocessed foo file ${out_f}"
      VERBATIM
      )
    list(APPEND result ${out_oo})
    list(APPEND result ${out_od})

  endforeach()
  set(${out_var} "${result}" PARENT_SCOPE)
endfunction()


# #-----------------------------------------------------------------

preprocess_oc(prep_files ${CMAKE_SOURCE_DIR}/linreg.oc)
set(SOURCES  linreg.c ${UTIL}/util.c)
add_executable(LinReg ${SOURCES} ${prep_files})


target_include_directories(LinReg PRIVATE ${UTIL})

target_link_libraries(LinReg m)