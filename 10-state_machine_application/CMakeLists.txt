cmake_minimum_required(VERSION 3.10)
project(state_machine_application)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")  # Enable optimization for accurate timing

add_executable(01 ilp-demo.c)