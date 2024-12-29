cmake_minimum_required(VERSION 3.10)

project(c_stdlib)

set(CMAKE_C_STANDARD 11)

add_executable(siz sizeof_.c)
add_executable(sys sys_.c)
