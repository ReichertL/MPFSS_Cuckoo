cmake_minimum_required (VERSION 2.6)
project (Tutorial)
set( CMAKE_VERBOSE_MAKEFILE on )
add_definitions(-Wall -g)

add_executable(Tutorial test.c)
target_link_libraries(Tutorial m)