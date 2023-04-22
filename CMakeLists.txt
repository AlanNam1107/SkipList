cmake_minimum_required(VERSION 3.0.0)
project(skiplist)

# include(CTest)
# enable_testing()
add_compile_options(-std=c++17 -Wall)

#设定可执行二进制文件的目录
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

# 设定源文件目录
aux_source_directory (${PROJECT_SOURCE_DIR}/src SRC_LIST)

#设定头文件目录
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/include)

# 设置生成可执行文件名称
add_executable (skiplist ${SRC_LIST})


# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
# 添加编译选项
