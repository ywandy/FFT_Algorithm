cmake_minimum_required(VERSION 3.9)
project(FFT_Algorithm)

set(CMAKE_CXX_STANDARD 17)
include_directories(inc)
set(SOURCE_FILES src/main.cpp src/FFT.cpp inc/FFT.h)
add_executable(FFT_Algorithm ${SOURCE_FILES} inc/FFT.h)