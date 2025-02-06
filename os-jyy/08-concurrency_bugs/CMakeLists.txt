cmake_minimum_required(VERSION 3.10)
project(ConcurrencyBugs)

set(CMAKE_CXX_STANDARD 20)

# 启用 ThreadSanitizer
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
endif()

add_executable(race_example tsan_demo.cpp)
target_link_libraries(race_example pthread)