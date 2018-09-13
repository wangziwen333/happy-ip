if (NOT DEFINED BOOST_LIB_DIR)
	message( FATAL_ERROR "not define BOOST_LIB_DIR" )
endif()

include_directories(./)
LINK_DIRECTORIES(${BUILD_DIR} ${BOOST_LIB_DIR})
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
##############################编译tcp_server#####################################
#程序名
set(OUT_NAME tcp_server)
#可执行程序
file(GLOB_RECURSE SOURCE_FILES "./tcp-server/*" "./utils.h")
add_executable(${OUT_NAME} ${SOURCE_FILES})
#连接库
target_link_libraries(${OUT_NAME} ip)
if(CMAKE_COMPILER_IS_GNUCXX)
	SET(CMAKE_EXE_LINKER_FLAGS " -no-pie")
	target_link_libraries(${OUT_NAME} boost_system-mt boost_thread-mt Threads::Threads)
endif()
##############################编译tcp_client#####################################
#程序名
set(OUT_NAME tcp_client)
#可执行程序
file(GLOB_RECURSE SOURCE_FILES "./tcp-client/*" "./utils.h")
add_executable(${OUT_NAME} ${SOURCE_FILES})
#连接库
target_link_libraries(${OUT_NAME} ip)
if(CMAKE_COMPILER_IS_GNUCXX)
	target_link_libraries(${OUT_NAME} boost_system-mt boost_thread-mt Threads::Threads)
endif()