set(TEST_NAME stopwatch-t)

project(${TEST_NAME})

add_executable(${TEST_NAME}
  ${TEST_NAME}.cpp)

target_include_directories(${TEST_NAME}
  PUBLIC
  ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${TEST_NAME}
  PRIVATE
  breeze)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
