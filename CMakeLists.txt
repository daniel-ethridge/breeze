cmake_minimum_required(VERSION 3.22)

project(Breeze
  VERSION 1.0
  DESCRIPTION "A helper library with implementations of various useful tools"
  LANGUAGES CXX)

# target_include_directories(breeze
#   PUBLIC
#     ${CMAKE_SOURCE_DIR}/include
#   PRIVATE
#     ${CMAKE_SOURCE_DIR}/include/breeze
# )

include(CTest)

if (BUILD_TESTING)
  message(STATUS "Tests will be built.")
  enable_testing()

  # All test subdirectories included here.
  add_subdirectory(test/bitflags)
  add_subdirectory(test/math)

endif (BUILD_TESTING)