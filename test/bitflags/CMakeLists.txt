add_executable(bitflags-test
  bitflags-t.cpp)

target_include_directories(bitflags-test
  PUBLIC
  ${CMAKE_SOURCE_DIR}/include)

add_test(NAME BitFlags COMMAND bitflags-test)