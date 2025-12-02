cmake_minimum_required(VERSION 3.14)

project(ExecutorGTest)
set(CMAKE_CXX_STANDARD 14) # GTest requires at least C++14 in newer versions, or we stick to an older one. 
                           # C++11 is supported by GTest 1.12.x. Let's use C++14 to be safe with modern GTest.
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 使用 FetchContent 自动下载并集成 GoogleTest
include(FetchContent)
message(STATUS "Fetching GoogleTest...")
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# 强制使用共享 CRT (Windows上常用)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(SOURCE_FILES
    executor.cpp
    ExecutorTest.cpp
)

add_executable(test_executor ${SOURCE_FILES})

# 链接 gtest_main (包含 main 函数)
target_link_libraries(test_executor
    PRIVATE gtest_main
)

enable_testing()
add_test(
    NAME AllExecutorTests
    COMMAND test_executor
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
