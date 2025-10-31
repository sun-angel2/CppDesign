#include <iostream>
#include <cassert>
#include "executor.h"

// 简单的测试框架宏
#define TEST_CASE(name) void test_##name()
#define RUN_TEST(name) do { \
    std::cout << "Running " #name "... "; \
    test_##name(); \
    std::cout << "Passed!" << std::endl; \
} while(0)

// 测试默认初始化
TEST_CASE(DefaultInitialization) {
    Executor executor;
    assert(executor.getX() == 0);
    assert(executor.getY() == 0);
    assert(executor.getHeading() == 'N');
}

// 测试自定义初始化
TEST_CASE(CustomInitialization) {
    Executor executor(5, 3, 'E');
    assert(executor.getX() == 5);
    assert(executor.getY() == 3);
    assert(executor.getHeading() == 'E');
}

// 测试前进指令
TEST_CASE(MoveForward) {
    // 向北移动
    Executor executor1;
    executor1.executeCommand('M');
    assert(executor1.getX() == 0);
    assert(executor1.getY() == 1);
    assert(executor1.getHeading() == 'N');

    // 向东移动
    Executor executor2(2, 3, 'E');
    executor2.executeCommand('M');
    assert(executor2.getX() == 3);
    assert(executor2.getY() == 3);
    assert(executor2.getHeading() == 'E');

    // 向南移动
    Executor executor3(0, 0, 'S');
    executor3.executeCommand('M');
    assert(executor3.getX() == 0);
    assert(executor3.getY() == -1);
    assert(executor3.getHeading() == 'S');

    // 向西移动
    Executor executor4(5, 5, 'W');
    executor4.executeCommand('M');
    assert(executor4.getX() == 4);
    assert(executor4.getY() == 5);
    assert(executor4.getHeading() == 'W');
}

// 测试左转指令
TEST_CASE(TurnLeft) {
    Executor executor;
    executor.executeCommand('L');  // 从北向左转
    assert(executor.getHeading() == 'W');
    
    executor.executeCommand('L');  // 从西向左转
    assert(executor.getHeading() == 'S');
    
    executor.executeCommand('L');  // 从南向左转
    assert(executor.getHeading() == 'E');
    
    executor.executeCommand('L');  // 从东向左转
    assert(executor.getHeading() == 'N');  // 回到初始方向
}

// 测试右转指令
TEST_CASE(TurnRight) {
    Executor executor;
    executor.executeCommand('R');  // 从北向右转
    assert(executor.getHeading() == 'E');
    
    executor.executeCommand('R');  // 从东向右转
    assert(executor.getHeading() == 'S');
    
    executor.executeCommand('R');  // 从南向右转
    assert(executor.getHeading() == 'W');
    
    executor.executeCommand('R');  // 从西向右转
    assert(executor.getHeading() == 'N');  // 回到初始方向
}

// 测试批量指令执行
TEST_CASE(BatchCommands) {
    // 测试1：前进、右转、前进
    Executor executor1;
    executor1.executeCommands("MRM");
    assert(executor1.getX() == 1);
    assert(executor1.getY() == 0);
    assert(executor1.getHeading() == 'E');

    // 测试2：复杂路径
    Executor executor2(1, 2, 'S');
    executor2.executeCommands("MLMRMLMM");
    assert(executor2.getX() == 4);
    assert(executor2.getY() == 0);
    assert(executor2.getHeading() == 'E');
}

// 测试getStatus方法
TEST_CASE(GetStatus) {
    Executor executor(3, 4, 'W');
    int32_t x, y;
    char heading;
    executor.getStatus(x, y, heading);
    assert(x == 3);
    assert(y == 4);
    assert(heading == 'W');
}

int main() {
    std::cout << "Running all tests..." << std::endl;
    
    RUN_TEST(DefaultInitialization);
    RUN_TEST(CustomInitialization);
    RUN_TEST(MoveForward);
    RUN_TEST(TurnLeft);
    RUN_TEST(TurnRight);
    RUN_TEST(BatchCommands);
    RUN_TEST(GetStatus);
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}