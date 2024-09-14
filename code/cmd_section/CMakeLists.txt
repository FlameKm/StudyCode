cmake_minimum_required(VERSION 3.0)
project(cmd_run)

set(CMAKE_CXX_STANDARD 11)

add_executable(cmd_run main.cpp cmd.cpp)

#SET(CMAKE_EXE_LINKER_FLAGS "-static")

#target_link_directories(cmd_run PRIVATE lib)
#target_link_libraries(cmd_run PRIVATE fun)