cmake_minimum_required(VERSION 3.19)
project(MyWebServer CXX)

set(CMAKE_CXX_STANDARD 11)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif ()

set(
        COMPILE_FLAGS
        -g
        -Wall
        -std=c++11
        -D_PTHREADS
        -Wno-unused-parameter
)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O0")

# 参数变成g++风格
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${COMPILE_FLAGS}")
string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(status "cxx_flags = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

add_subdirectory(WebServer)
