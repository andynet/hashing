cmake_minimum_required(VERSION 3.17)
project(hashing C)
set(CMAKE_C_STANDARD 99)

add_library(hashing src/hashing.h src/hashing.c)

add_executable(main main.c)
target_link_libraries(main hashing)

add_executable(test test/test_hashing.c)
target_link_libraries(test hashing)                     # tested library
target_link_libraries(test check m rt pthread subunit)  # libraries needed for testing