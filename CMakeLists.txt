cmake_minimum_required(VERSION 3.17)
project(hashing C)
set(CMAKE_C_STANDARD 99)

file(REMOVE cmake-build-debug-coverage/CMakeFiles/hashing.dir/src/hashing.c.gcda)
file(REMOVE cmake-build-debug-coverage/CMakeFiles/test.dir/test/test_hashing.c.gcda)

link_directories(/usr/local/lib)
add_library(hashing src/hashing.h src/hashing.c)
add_library(safe_alloc src/safe_alloc.h src/safe_alloc.c)

add_executable(main main.c)
target_link_libraries(main hashing safe_alloc)

add_executable(test test/test_hashing.c)
# tested library
target_link_libraries(test hashing safe_alloc)
# libraries needed for testing
target_link_libraries(test check m rt pthread subunit)

add_executable(crit test/crit.c)
target_link_libraries(crit hashing safe_alloc)
target_link_libraries(crit criterion)
