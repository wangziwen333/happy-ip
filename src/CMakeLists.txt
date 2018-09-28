#设置lib输出目录
SET(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/lib")

#设置安装库目录
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib)

add_subdirectory(ip)
option(BUILD_NETWORK OFF)
if (BUILD_NETWORK)
	message (STATUS "build network")
	add_subdirectory(network)
endif()