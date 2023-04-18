add_executable(orm-test
  orm-t.cpp)

target_include_directories(orm-test
  PUBLIC
  ${CMAKE_SOURCE_DIR}/include)

find_package(Qt5 REQUIRED COMPONENTS Core Sql)

target_link_libraries(orm-test
  PUBLIC
  breeze
  Qt5::Core
  Qt5::Sql
)

add_test(NAME ORM COMMAND orm-test)