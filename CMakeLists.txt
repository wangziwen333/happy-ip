cmake_minimum_required(VERSION 3.0)
project(happy_ip)

# 清空build目录，保证CMakeCache.txt为最新
file(REMOVE_RECURSE ${PROJECT_SOURCE_DIR}/build/*)
message(STATUS "clean build file")

#debug或release模式
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug") 
endif()
message(STATUS "Build " ${CMAKE_BUILD_TYPE})

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-fpermissive)
    message(STATUS "optional: -fpermissive")  
    add_compile_options(-std=c++11)
    message(STATUS "optional: -std=c++11")   
endif(CMAKE_COMPILER_IS_GNUCXX)

if (NOT DEFINED BOOST_INCLUDE_DIR)
	message( FATAL_ERROR "not define BOOST_INCLUDE_DIR" )
endif()
include_directories(${BOOST_INCLUDE_DIR} ./)

add_subdirectory(src)
option(BUILD_EXAMPLES OFF)
if (BUILD_EXAMPLES)
	message (STATUS "build examples")
	add_subdirectory(examples)
endif()
