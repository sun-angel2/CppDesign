# 最低CMake版本要求（兼容主流环境）
cmake_minimum_required(VERSION 3.10)

# 先设置vcpkg工具链（必须在project前，确保依赖查找生效）
set(CMAKE_TOOLCHAIN_FILE "E:/vcpkg/scripts/buildsystems/vcpkg.cmake" 
    CACHE STRING "vcpkg toolchain file")

# 项目名称与C++标准
project(ExecutorGTest)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 强制刷新依赖路径（解决缓存冲突）
unset(GTEST_LIBRARY CACHE)
unset(GTEST_INCLUDE_DIR CACHE)
unset(GTEST_MAIN_LIBRARY CACHE)

# 查找GTest库（通过vcpkg安装的版本）
find_package(GTest REQUIRED)
if(NOT GTEST_FOUND)
    message(FATAL_ERROR "GTest未找到！请确认已通过vcpkg安装：vcpkg install gtest:x64-windows")
endif()

# 包含头文件目录（项目自身+GTest）
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}  # 项目根目录（executor.h所在位置）
    ${GTEST_INCLUDE_DIRS}        # GTest头文件路径
)

# 源文件列表
set(SOURCE_FILES
    executor.cpp        # 核心实现
    ExecutorTest.cpp    # 测试用例
)

# 生成测试可执行文件
add_executable(test_executor ${SOURCE_FILES})

# 链接GTest库（使用vcpkg提供的目标，自动处理路径）
target_link_libraries(test_executor
    PRIVATE GTest::gtest
    PRIVATE GTest::gtest_main
)

# Windows特殊处理：MinGW需要链接pthread
if(MINGW)
    target_link_libraries(test_executor PRIVATE pthread)
endif()

# 测试配置
enable_testing()
add_test(
    NAME AllExecutorTests
    COMMAND test_executor
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)