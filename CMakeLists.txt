cmake_minimum_required(VERSION 3.22)

project(Breeze
  VERSION 1.0
  DESCRIPTION "A helper library with implementations of various useful tools"
  LANGUAGES CXX)

set(BREEZE_INC ${CMAKE_SOURCE_DIR}/include/breeze)

set(BITFLAGS ${BREEZE_INC}/bitflags.h)
source_group("Include" FILES ${BITFLAGS})

add_library(breeze
  ${BITFLAGS})

set_target_properties(breeze
  PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(breeze
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_SOURCE_DIR}/include/breeze
)

set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)  # Removes random added projects in IDE generation
include(CTest)

if (BUILD_TESTING)
  message(STATUS "Tests will be built.")
  enable_testing()

  # All test subdirectories included here.
  add_subdirectory(test/bitflags)

endif (BUILD_TESTING)