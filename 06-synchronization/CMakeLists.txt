cmake_minimum_required(VERSION 3.10)

project(Sync)

set(CMAKE_C_STANDARD 11)

add_executable(a producer_consumer_mutax_lock.c)
add_executable(b print_seq.c)
add_executable(c producer_consumer_semaphore.c)
add_executable(d producer_consumer_atomic_integer.c)
add_executable(e philosopher.c)
