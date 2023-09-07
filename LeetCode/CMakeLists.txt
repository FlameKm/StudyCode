cmake_minimum_required(VERSION 3.26)
project(LeetCode)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SOURCES leetcode/editor/cn/*cpp)

add_executable(LeetCode ${SOURCES})
