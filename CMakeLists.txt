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
if (NOT DEFINED BOOST_INCLUDE)
	message( FATAL_ERROR "not define BOOST_INCLUDE" )
endif()

include_directories(./src ./ ${BOOST_INCLUDE})

#设置lib输出目录
SET(LIBRARY_OUTPUT_PATH ${BUILD_DIR})

##############################编译network#####################################
#遍历主目录源文件
#file(GLOB_RECURSE SOURCE_FILES "./src/network/*.cpp")
#编译静态库程序
#add_library(network STATIC ${SOURCE_FILES})

##############################编译ip#####################################
#遍历主目录源文件
file(GLOB_RECURSE SOURCE_FILES "./src/ip/*.cpp" "./src/ip/*.h")
#编译静态库程序
add_library(ip STATIC ${SOURCE_FILES})

# 设置安装库目录
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib)
install(TARGETS ip ARCHIVE DESTINATION ./)
install(DIRECTORY src/ip DESTINATION ${PROJECT_SOURCE_DIR}/install/include FILES_MATCHING PATTERN "*.h")
install(FILES singleton.h thread_pool.h DESTINATION ${PROJECT_SOURCE_DIR}/install/include)
