cmake_minimum_required(VERSION 3.5)

add_library(ctreemap TreeMap.h TreeMap.c)
target_include_directories(ctreemap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})