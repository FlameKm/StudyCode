cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
#add_library(fun STATIC fun.cpp)

#SET(CMAKE_EXE_LINKER_FLAGS "-static")

project(user_daemon)
add_executable(user_daemon daemon.c)

project(user_app)
add_executable(user_app main.c)
