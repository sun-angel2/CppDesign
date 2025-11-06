cmake_minimum_required(VERSION 3.10)
project(MyTestProject)

# 指定 vcpkg 工具链文件
set(CMAKE_TOOLCHAIN_FILE "E:/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# 查找 Google Test 库
find_package(GTest REQUIRED)

# 添加可执行文件
add_executable(test_runner ExecutorTest.cpp)

# 链接 GTest 库
target_link_libraries(test_runner PRIVATE GTest::gtest GTest::gtest_main)