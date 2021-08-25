cmake_minimum_required(VERSION 3.16)
project(cpp_concurrency_in_action)

set(CMAKE_CXX_STANDARD 14)

add_executable(cpp_concurrency_in_action main.cpp parallel_accumulate.h thread_guard.h)