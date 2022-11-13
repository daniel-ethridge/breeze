project(math-test)

add_executable(polynomial-test
  polynomial-t.cpp)

  target_include_directories(polynomial-test
  PUBLIC
  ${CMAKE_SOURCE_DIR}/include)

add_test(NAME Polynomial COMMAND polynomial-test)