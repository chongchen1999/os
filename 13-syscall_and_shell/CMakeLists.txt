cmake_minimum_required(VERSION 3.10)

# Project name
project(SyscallAndShellProject)

# Set compiler and linker flags for freestanding build
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding -nostdlib -g -O2")

# Define the executable and specify source file
add_executable(sh sh-xv6.c)

# Link without standard C runtime (no default libraries)
set_target_properties(sh PROPERTIES LINK_FLAGS "-nostdlib")
