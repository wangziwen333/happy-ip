#依赖包含 
include_directories(../)
#if(CMAKE_COMPILER_IS_GNUCXX)
    #add_compile_options(-fpermissive)  
#endif(CMAKE_COMPILER_IS_GNUCXX)
# 遍历主目录源文件
file(GLOB_RECURSE SOURCE_FILES "./*.h" "./*.cpp" "./*.cc")

#编译静态库程序
add_library(network STATIC ${SOURCE_FILES})

#设置安装库目录
install(TARGETS network ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ./ DESTINATION ${PROJECT_SOURCE_DIR}/install/include/network FILES_MATCHING PATTERN "*.h")
