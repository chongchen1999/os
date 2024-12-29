cmake_minimum_required(VERSION 3.10)

# Project name
project(SyscallAndShellProject)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-ffreestanding -O2)

# Define the executable and specify source file
# add_executable(sh sh-xv6.c)
add_executable(sig signal-handler.c)

# target_compile_options(sh PRIVATE -g)