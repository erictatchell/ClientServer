cmake_minimum_required(VERSION 3.26)
project(UDPChat)

set(CMAKE_CXX_STANDARD 23)

add_executable(UDPChat
        server.cpp
        Manager.hpp
        Manager.cpp)
add_executable(UDPChat-Client client.cpp client.hpp
        Manager.hpp
        Manager.cpp)

target_link_libraries(UDPChat ws2_32.lib)
target_link_libraries(UDPChat-Client ws2_32.lib)