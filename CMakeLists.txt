cmake_minimum_required(VERSION 3.15)
project(ZMQ_Implemented_In_Cpp LANGUAGES CXX)

set(CMAKE_TOOLCHAIN_FILE "C:/english_directory/ZMQ_Implemented_In_Cpp/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(cppzmq CONFIG REQUIRED)

file(GLOB SRC_FILES "src/*.cpp")

add_executable(ZMQ_Server "src/lec-05-prg-01-req-rep-basic-server.cpp")
add_executable(ZMQ_Client "src/lec-05-prg-02-req-rep-basic-client.cpp")

target_link_libraries(ZMQ_Server PRIVATE cppzmq)
target_link_libraries(ZMQ_Client PRIVATE cppzmq)