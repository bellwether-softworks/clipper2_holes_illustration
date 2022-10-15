cmake_minimum_required(VERSION 3.23)
project(clipper2_holes)
add_definitions(-std=c++17)

add_executable(clipper2_holes main.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CLIPPER2_TESTS OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/clipper2/CPP)
include_directories(${CMAKE_SOURCE_DIR}/clipper2/CPP)
include_directories(${CMAKE_SOURCE_DIR}/clipper2/CPP/Clipper2Lib/include)
target_link_libraries(clipper2_holes Clipper2)
