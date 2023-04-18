cmake_minimum_required(VERSION 3.22)

project(Breeze
  VERSION 1.0
  DESCRIPTION "A helper library with implementations of various useful tools"
  LANGUAGES CXX)

add_library(breeze
  include/breeze/orm.h
  src/orm.cpp)

target_include_directories(breeze
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_SOURCE_DIR}/include/breeze
)

find_package(Qt5 REQUIRED COMPONENTS Core Sql)

target_link_libraries(breeze
  PUBLIC
  Qt5::Core
  Qt5::Sql
)

include(CTest)

if (BUILD_TESTING)
  message(STATUS "Tests will be built.")
  enable_testing()

# All test subdirectories included here.
# add_subdirectory(test/bitflags)
add_subdirectory(test/orm)

endif (BUILD_TESTING)