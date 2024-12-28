cmake_minimum_required(VERSION 3.10)
project(ProcessAddressSpace)

# Set the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
enable_language(ASM)

# Add the executable
add_executable(01 ptr2main.c)

add_executable(02 minimal.s)
set_target_properties(02 PROPERTIES LINK_FLAGS "-nostdlib")

