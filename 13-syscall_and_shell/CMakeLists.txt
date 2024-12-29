cmake_minimum_required(VERSION 3.10)

# Project name
project(SyscallAndShell)

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding -g -O2")

# Add the executable
add_executable(sh sh-xv6.c)
