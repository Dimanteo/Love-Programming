cmake_minimum_required(VERSION 3.15)
project(Programming_Language)

set(CMAKE_CXX_STANDARD 14)

add_executable(FrontEnd frontend.cpp)
add_executable(BackEnd backend.cpp)
add_executable(MiddleEnd middleend.cpp)
add_executable(ReverseEnd r_frontend.cpp)