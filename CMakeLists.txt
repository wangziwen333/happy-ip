cmake_minimum_required(VERSION 3.0)
project(happy_ip)

# 清空build目录，保证CMakeCache.txt为最新
#file(REMOVE_RECURSE ${PROJECT_SOURCE_DIR}/build/*)
#message(STATUS "clean build file")

#debug或release模式
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()
message(STATUS "Build " ${CMAKE_BUILD_TYPE})

add_compile_options(-std=c++14)
message(STATUS "optional: -std=c++14")

if (NOT DEFINED BOOST_INCLUDE_DIR)
	message( FATAL_ERROR "not define BOOST_INCLUDE_DIR" )
endif()
if (NOT DEFINED PROTOBUF_INCLUDE_DIR OR NOT DEFINED PROTOBUF_LIB_DIR)
	message( FATAL_ERROR "not define PROTOBUF_INCLUDE_DIR or PROTOBUF_LIB_DIR" )
endif()
if (NOT DEFINED 3RDPARTY_INCLUDE_DIR)
	message( FATAL_ERROR "not define 3RDPARTY_INCLUDE_DIR" )
endif()

option(GLOG_OUTPUT OFF)
if (GLOG_OUTPUT)
	if(NOT DEFINED GLOG_INCLUDE_DIR OR NOT DEFINED GLOG_LIB_DIR)
		message( FATAL_ERROR "not define GLOG_INCLUDE_DIR or GLOG_LIB_DIR" )
	endif()
	message (STATUS "build with glog")
	add_definitions(-DGLOG_OUTPUT -DGLOG_NO_ABBREVIATED_SEVERITIES -DGOOGLE_GLOG_DLL_DECL=)
endif()
include_directories(${BOOST_INCLUDE_DIR} ./ ${PROTOBUF_INCLUDE_DIR} ${3RDPARTY_INCLUDE_DIR} ${GLOG_INCLUDE_DIR})

add_subdirectory(src)
message (STATUS "build examples")
add_subdirectory(examples)

install(FILES ${3RDPARTY_INCLUDE_DIR}/singleton.h ${3RDPARTY_INCLUDE_DIR}/thread_pool.h
		${3RDPARTY_INCLUDE_DIR}/config_glog.h ${3RDPARTY_INCLUDE_DIR}/output.h DESTINATION include)
