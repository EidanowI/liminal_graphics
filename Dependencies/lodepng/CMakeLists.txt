cmake_minimum_required(VERSION 3.8)

add_library(lodepng lodepng.cpp)

target_include_directories(lodepng PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})