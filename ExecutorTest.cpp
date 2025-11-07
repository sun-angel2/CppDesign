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

// --- 需求2：加速功能测试 ---

TEST(ExecutorTest, AccelerationToggle) {
    Executor executor;
    // 初始为非加速状态，执行F进入加速
    executor.executeCommand('F');
    // 验证进入加速状态后的行为（例如M前进2格）
    executor.executeCommand('M');
    EXPECT_EQ(executor.getY(), 2);

    // 再次执行F退出加速
    executor.executeCommand('F');
    // 验证退出加速状态后的行为（例如M前进1格）
    executor.executeCommand('M');
    EXPECT_EQ(executor.getY(), 3);
}

TEST(ExecutorTest, AccelerationMoveForward) {
    Executor executor;
    executor.executeCommand('F'); // 进入加速
    executor.executeCommand('M');
    EXPECT_EQ(executor.getX(), 0);
    EXPECT_EQ(executor.getY(), 2); // 前进2格
    EXPECT_EQ(executor.getHeading(), 'N');
}

TEST(ExecutorTest, AccelerationTurnLeft) {
    Executor executor;
    executor.executeCommand('F'); // 进入加速
    executor.executeCommand('L');
    // 先前进1格到(0,1)，再左转
    EXPECT_EQ(executor.getX(), 0);
    EXPECT_EQ(executor.getY(), 1);
    EXPECT_EQ(executor.getHeading(), 'W');
}

TEST(ExecutorTest, AccelerationTurnRight) {
    Executor executor(1, 1, 'E');
    executor.executeCommand('F'); // 进入加速
    executor.executeCommand('R');
    // 先前进1格到(2,1)，再右转
    EXPECT_EQ(executor.getX(), 2);
    EXPECT_EQ(executor.getY(), 1);
    EXPECT_EQ(executor.getHeading(), 'S');
}

TEST(ExecutorTest, AccelerationComplex) {
    Executor executor;
    executor.executeCommands("FMRML");
    // F -> 加速
    // M -> 向北前进2格到 (0,2)
    // R -> 前进1格到 (0,3)，然后右转朝向E
    // M -> 向东前进2格到 (2,3)
    // L -> 前进1格到 (3,3)，然后左转朝向N
    EXPECT_EQ(executor.getX(), 3);
    EXPECT_EQ(executor.getY(), 3);
    EXPECT_EQ(executor.getHeading(), 'N');
}

// --- 需求3：倒车功能测试 ---

TEST(ExecutorTest, ReverseToggle) {
    Executor executor;
    executor.executeCommand('B'); // 进入倒车
    executor.executeCommand('M'); // 后退
    EXPECT_EQ(executor.getY(), -1);

    executor.executeCommand('B'); // 退出倒车
    executor.executeCommand('M'); // 前进
    EXPECT_EQ(executor.getY(), 0);
}

TEST(ExecutorTest, ReverseModeCommands) {
    // 测试倒车后退
    {
        Executor executor(0, 0, 'N');
        executor.executeCommands("BM");
        EXPECT_EQ(executor.getY(), -1);
        EXPECT_EQ(executor.getHeading(), 'N');
    }
    // 测试倒车时左转(实际右转)
    {
        Executor executor;
        executor.executeCommands("BL");
        EXPECT_EQ(executor.getHeading(), 'E');
    }
    // 测试倒车时右转(实际左转)
    {
        Executor executor;
        executor.executeCommands("BR");
        EXPECT_EQ(executor.getHeading(), 'W');
    }
}

TEST(ExecutorTest, Combined_F_B_State_Commands) {
    // 测试F+B状态下后退
    {
        Executor executor;
        executor.executeCommands("FBM");
        EXPECT_EQ(executor.getY(), -2);
    }
    // 测试F+B状态下左转(先退再右转)
    {
        Executor executor(1, 1, 'S');
        executor.executeCommands("FBL");
        EXPECT_EQ(executor.getY(), 2); // S向后退是Y+1
        EXPECT_EQ(executor.getHeading(), 'E'); // S倒车时L是右转,S右边是E
    }
    // 测试F+B状态下右转(先退再左转)
    {
        Executor executor(1, 1, 'S');
        executor.executeCommands("FBR");
        EXPECT_EQ(executor.getY(), 2); // S向后退是Y+1
        EXPECT_EQ(executor.getHeading(), 'W'); // S倒车时R是左转,S左边是W
    }
}

TEST(ExecutorTest, CombinedStateComplex) {
    Executor executor;
    executor.executeCommands("BMRBMLM");
    // B -> 倒车
    // M -> 后退到 (0,-1)
    // R -> 左转到 W
    // B -> 退出倒车
    // M -> 向西前进到 (-1,-1)
    // L -> 左转到 S
    // M -> 向南前进到 (-1,-2)
    EXPECT_EQ(executor.getX(), -1);
    EXPECT_EQ(executor.getY(), -2);
    EXPECT_EQ(executor.getHeading(), 'S');
}

// --- 需求4：掉头功能测试 ---

TEST(ExecutorTest, TurnRoundNormal) {
    Executor executor;
    executor.executeCommands("TR");
    // 初始(0,0,N) -> L -> (0,0,W) -> M -> (-1,0,W) -> L -> (-1,0,S)
    EXPECT_EQ(executor.getX(), -1);
    EXPECT_EQ(executor.getY(), 0);
    EXPECT_EQ(executor.getHeading(), 'S');
}

TEST(ExecutorTest, TurnRoundAccelerated) {
    Executor executor;
    executor.executeCommands("FTR");
    // F -> 加速
    // TR -> M -> (0,1,N) -> L -> (0,1,W) -> M -> (-1,1,W) -> L -> (-1,1,S)
    EXPECT_EQ(executor.getX(), -1);
    EXPECT_EQ(executor.getY(), 1);
    EXPECT_EQ(executor.getHeading(), 'S');
}

TEST(ExecutorTest, TurnRoundUnaffectedByReverse) {
    Executor executor;
    // B -> 倒车, TR不受其影响
    executor.executeCommands("BTR");
    // 初始(0,0,N) -> L -> (0,0,W) -> M -> (-1,0,W) -> L -> (-1,0,S)
    EXPECT_EQ(executor.getX(), -1);
    EXPECT_EQ(executor.getY(), 0);
    EXPECT_EQ(executor.getHeading(), 'S');
}

TEST(ExecutorTest, TurnRoundComplex) {
    Executor executor(1, 1, 'E');
    executor.executeCommands("MTRM");
    // M -> (2,1,E)
    // TR -> L -> (2,1,N) -> M -> (2,2,N) -> L -> (2,2,W)
    // M -> (1,2,W)
    EXPECT_EQ(executor.getX(), 1);
    EXPECT_EQ(executor.getY(), 2);
    EXPECT_EQ(executor.getHeading(), 'W');
}

// 程序入口：初始化GTest并运行所有测试用例
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}