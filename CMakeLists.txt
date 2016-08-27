cmake_minimum_required(VERSION 3.5)

add_library(ctreemap ctmap.h ctmap.c)
target_include_directories(ctreemap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})