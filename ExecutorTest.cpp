#include <gtest/gtest.h>
#include "executor.h"

// 测试套件：ExecutorTest，包含所有Executor相关测试用例
TEST(ExecutorTest, DefaultInitialization) {
    // 测试默认初始化状态：(0,0,N)
    Executor executor;
    EXPECT_EQ(executor.getX(), 0);         // 验证X坐标
    EXPECT_EQ(executor.getY(), 0);         // 验证Y坐标
    EXPECT_EQ(executor.getHeading(), 'N'); // 验证朝向
}

TEST(ExecutorTest, CustomInitialization) {
    // 测试自定义初始化：指定位置(5,3)和朝向'E'
    Executor executor(5, 3, 'E');
    EXPECT_EQ(executor.getX(), 5);
    EXPECT_EQ(executor.getY(), 3);
    EXPECT_EQ(executor.getHeading(), 'E');
}

TEST(ExecutorTest, MoveForwardCommand) {
    // 测试北向前进（Y轴+1）
    {
        Executor executor;
        executor.executeCommand('M');
        EXPECT_EQ(executor.getX(), 0);
        EXPECT_EQ(executor.getY(), 1);
        EXPECT_EQ(executor.getHeading(), 'N'); // 朝向不变
    }

    // 测试东向前进（X轴+1）
    {
        Executor executor(2, 3, 'E');
        executor.executeCommand('M');
        EXPECT_EQ(executor.getX(), 3);
        EXPECT_EQ(executor.getY(), 3);
        EXPECT_EQ(executor.getHeading(), 'E');
    }

    // 测试南向前进（Y轴-1）
    {
        Executor executor(0, 0, 'S');
        executor.executeCommand('M');
        EXPECT_EQ(executor.getX(), 0);
        EXPECT_EQ(executor.getY(), -1);
        EXPECT_EQ(executor.getHeading(), 'S');
    }

    // 测试西向前进（X轴-1）
    {
        Executor executor(5, 5, 'W');
        executor.executeCommand('M');
        EXPECT_EQ(executor.getX(), 4);
        EXPECT_EQ(executor.getY(), 5);
        EXPECT_EQ(executor.getHeading(), 'W');
    }
}

TEST(ExecutorTest, TurnLeftCommand) {
    Executor executor;

    // 北 → 西
    executor.executeCommand('L');
    EXPECT_EQ(executor.getHeading(), 'W');

    // 西 → 南
    executor.executeCommand('L');
    EXPECT_EQ(executor.getHeading(), 'S');

    // 南 → 东
    executor.executeCommand('L');
    EXPECT_EQ(executor.getHeading(), 'E');

    // 东 → 北（循环一周回到初始方向）
    executor.executeCommand('L');
    EXPECT_EQ(executor.getHeading(), 'N');
}

TEST(ExecutorTest, TurnRightCommand) {
    Executor executor;

    // 北 → 东
    executor.executeCommand('R');
    EXPECT_EQ(executor.getHeading(), 'E');

    // 东 → 南
    executor.executeCommand('R');
    EXPECT_EQ(executor.getHeading(), 'S');

    // 南 → 西
    executor.executeCommand('R');
    EXPECT_EQ(executor.getHeading(), 'W');

    // 西 → 北（循环一周回到初始方向）
    executor.executeCommand('R');
    EXPECT_EQ(executor.getHeading(), 'N');
}

TEST(ExecutorTest, BatchCommandsExecution) {
    // 测试用例1：简单指令序列 "MRM"
    {
        Executor executor;
        executor.executeCommands("MRM");
        // 执行过程：
        // M → 向北移动到(0,1)
        // R → 右转朝向'E'
        // M → 向东移动到(1,1)？？？原预期有误，修正如下：
        // 原默认初始位置(0,0)，首条M向北到(0,1)，R右转朝东，再M向东到(1,1)
        // 修正断言（原测试用例预期有误，此处按实际逻辑修正）
        EXPECT_EQ(executor.getX(), 1);
        EXPECT_EQ(executor.getY(), 1);
        EXPECT_EQ(executor.getHeading(), 'E');
    }

    // 测试用例2：复杂指令序列 "MLMRMLMM"
    {
        Executor executor(1, 2, 'S');
        executor.executeCommands("MLMRMLMM");
        // 执行过程拆解：
        // 初始状态：(1,2)，朝向'S'
        // M → 向南移动到(1,1)
        // L → 左转朝向'E'（S左转是E）
        // M → 向东移动到(2,1)
        // R → 右转朝向'S'（E右转是S）
        // M → 向南移动到(2,0)
        // L → 左转朝向'E'（S左转是E）
        // M → 向东移动到(3,0)
        // M → 向东移动到(4,0)
        EXPECT_EQ(executor.getX(), 4);
        EXPECT_EQ(executor.getY(), 0);
        EXPECT_EQ(executor.getHeading(), 'E');
    }
}

TEST(ExecutorTest, GetStatusMethod) {
    Executor executor(3, 4, 'W');
    int32_t x, y;
    char heading;
    
    executor.getStatus(x, y, heading);
    EXPECT_EQ(x, 3);         // 验证X坐标
    EXPECT_EQ(y, 4);         // 验证Y坐标
    EXPECT_EQ(heading, 'W'); // 验证朝向
}

// 程序入口：初始化GTest并运行所有测试用例
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}