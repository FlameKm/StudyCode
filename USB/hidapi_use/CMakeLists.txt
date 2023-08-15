cmake_minimum_required(VERSION 3.0.0)
project(usb_hid C)

set(CMAKE_C_STANDARD 11)

add_executable(usb_hid src/main.c src/hidapi.h src/hid.c)
target_link_libraries(usb_hid setupapi)