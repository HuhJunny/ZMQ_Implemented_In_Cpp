cmake_minimum_required(VERSION 3.15)
project(ZMQ_Implemented_In_Cpp LANGUAGES CXX)

set(CMAKE_TOOLCHAIN_FILE "C:/english_directory/ZMQ_Implemented_In_Cpp/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(cppzmq CONFIG REQUIRED)

file(GLOB SRC_FILES "src/*.cpp")

add_executable(ZMQ_Server_01 "src/lec-05-prg-01-req-rep-basic-server.cpp")
add_executable(ZMQ_Client_02 "src/lec-05-prg-02-req-rep-basic-client.cpp")
add_executable(ZMQ_Server_03 "src/lec-05-prg-03-pub-sub-basic-server.cpp")
add_executable(ZMQ_Client_04 "src/lec-05-prg-04-pub-sub-basic-client.cpp")


target_link_libraries(ZMQ_Server_01 PRIVATE cppzmq)
target_link_libraries(ZMQ_Client_02 PRIVATE cppzmq)
target_link_libraries(ZMQ_Server_03 PRIVATE cppzmq)
target_link_libraries(ZMQ_Client_04 PRIVATE cppzmq)