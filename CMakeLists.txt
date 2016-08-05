cmake_minimum_required(VERSION 3.5)
project(ReadStanford)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)

include_directories(rapidxml-1.13/)
add_executable(ReadStanford ${SOURCE_FILES})