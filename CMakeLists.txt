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
add_executable(ZMQ_Server_05 "src/lec-05-prg-05-pub-sub-and-pull-push-server.cpp")
add_executable(ZMQ_Client_06 "src/lec-05-prg-06-pub-sub-and-pull-push-client.cpp")
add_executable(ZMQ_Server_07 "src/lec-05-prg-07-pub-sub-and-pull-push-server-v2.cpp")
add_executable(ZMQ_Client_08 "src/lec-05-prg-08-pub-sub-and-pull-push-client-v2.cpp")
add_executable(ZMQ_Server_09 "src/lec-05-prg-09-dealer-router-async-server.cpp")
add_executable(ZMQ_Client_10 "src/lec-05-prg-10-dealer-router-async-client.cpp")
add_executable(ZMQ_Client_11 "src/lec-05-prg-11-dealer-router-async-client-thread.cpp")


target_link_libraries(ZMQ_Server_01 PRIVATE cppzmq)
target_link_libraries(ZMQ_Client_02 PRIVATE cppzmq)
target_link_libraries(ZMQ_Server_03 PRIVATE cppzmq)
target_link_libraries(ZMQ_Client_04 PRIVATE cppzmq)
target_link_libraries(ZMQ_Server_05 PRIVATE cppzmq)
target_link_libraries(ZMQ_Client_06 PRIVATE cppzmq)
target_link_libraries(ZMQ_Server_07 PRIVATE cppzmq)
target_link_libraries(ZMQ_Client_08 PRIVATE cppzmq)
target_link_libraries(ZMQ_Server_09 PRIVATE cppzmq)
target_link_libraries(ZMQ_Client_10 PRIVATE cppzmq)
target_link_libraries(ZMQ_Client_11 PRIVATE cppzmq)