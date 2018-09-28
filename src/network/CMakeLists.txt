#依赖包含 
include_directories(../)

# 遍历主目录源文件
file(GLOB_RECURSE SOURCE_FILES "./*.h" "./*.cpp" "./*.cc")

#编译静态库程序
add_library(network STATIC ${SOURCE_FILES})

#设置安装库目录
install(TARGETS network ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ./ DESTINATION ${PROJECT_SOURCE_DIR}/install/include FILES_MATCHING PATTERN "*.h")
