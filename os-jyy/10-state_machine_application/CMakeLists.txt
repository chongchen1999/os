cmake_minimum_required(VERSION 3.10)
project(state_machine_application)

# Set the C standard
set(CMAKE_C_STANDARD 99)

# Enable gprof profiling
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -pg")      # Add -pg for profiling during compilation
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")  # Add -pg for linking

# Add the executable
add_executable(01 ilp-demo.c)
