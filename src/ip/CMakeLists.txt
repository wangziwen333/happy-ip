#project(ip)
#依赖包含 
include_directories(../ ../../)

#设置lib输出目录
SET(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/lib")

# 遍历主目录源文件
file(GLOB_RECURSE SOURCE_FILES "./*.h" "./*.cpp")

#编译静态库程序
add_library(ip STATIC ${SOURCE_FILES})

#设置安装库目录
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib)
install(TARGETS ip ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ./ DESTINATION ${PROJECT_SOURCE_DIR}/install/include FILES_MATCHING PATTERN "*.h")
install(FILES ${PROJECT_SOURCE_DIR}/singleton.h ${PROJECT_SOURCE_DIR}/thread_pool.h DESTINATION ${PROJECT_SOURCE_DIR}/install/include)
