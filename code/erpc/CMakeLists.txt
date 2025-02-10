cmake_minimum_required(VERSION 3.5)
project(erpcgen)

add_executable(client
        client.cpp
        erpcgen/c_hello_client.cpp
        erpcgen/hello_client.cpp
        erpcgen/hello_interface.cpp
)

target_link_libraries(client PRIVATE ${CMAKE_SOURCE_DIR}/erpc/lib/liberpc.a)

target_include_directories(client PRIVATE erpc/include)
target_include_directories(client PRIVATE erpcgen)

add_executable(server
        server.cpp
        erpcgen/c_hello_server.cpp
        erpcgen/hello_server.cpp
        erpcgen/hello_interface.cpp
)
target_link_libraries(server PRIVATE ${CMAKE_SOURCE_DIR}/erpc/lib/liberpc.a)
target_include_directories(server PRIVATE erpc/include)
target_include_directories(server PRIVATE erpcgen)



