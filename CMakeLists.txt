cmake_minimum_required(VERSION 3.0)
project(happy_ip)
set(BUILD_DIR "${PROJECT_SOURCE_DIR}/build/lib")

# 清空build目录，保证CMakeCache.txt为最新
file(REMOVE_RECURSE ./build/*)
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

#依赖包含 
if (NOT DEFINED BOOST_INCLUDE_DIR)
	message( FATAL_ERROR "not define BOOST_INCLUDE_DIR" )
endif()

include_directories(./src ./ ${BOOST_INCLUDE_DIR})

#设置lib输出目录
SET(LIBRARY_OUTPUT_PATH ${BUILD_DIR})

##############################编译network#####################################
#遍历主目录源文件
#file(GLOB_RECURSE SOURCE_FILES "./src/network/*.cpp")
#编译静态库程序
#add_library(network STATIC ${SOURCE_FILES})

##############################编译ip#####################################
#遍历主目录源文件
file(GLOB_RECURSE SOURCE_FILES "./src/ip/*")
#编译静态库程序
add_library(ip STATIC ${SOURCE_FILES})

# 设置安装库目录
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib)
install(TARGETS ip ARCHIVE DESTINATION ./)
install(DIRECTORY src/ip DESTINATION ${PROJECT_SOURCE_DIR}/install/include FILES_MATCHING PATTERN "*.h")
install(FILES singleton.h thread_pool.h DESTINATION ${PROJECT_SOURCE_DIR}/install/include)

option(BUILD_EXAMPLES OFF)
if (BUILD_EXAMPLES)
	message (STATUS "build examples")
	add_subdirectory(examples)
endif()
#if (NOT DEFINED BOOST_LIB_DIR)
	#message( FATAL_ERROR "not define BOOST_LIB_DIR" )
#endif()
#LINK_DIRECTORIES(${BUILD_DIR} ${BOOST_LIB_DIR})
#include_directories(./examples)
##############################编译tcp_server#####################################
#程序名
#set(OUT_NAME tcp_server)
#可执行程序
#file(GLOB_RECURSE SOURCE_FILES "./examples/tcp-server/*" "./examples/utils.h")
#add_executable(${OUT_NAME} ${SOURCE_FILES})
#连接库
#set(LINK_LIST ip)
#target_link_libraries(${OUT_NAME} ${LINK_LIST})
