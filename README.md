# Executor 智能车指令执行组件

# Executor 智能车指令执行组件



## 项目概述



Executor 是一个基于 C++ 开发的智能车指令执行组件，用于解析和执行智能车的移动、转向、加速、倒车、掉头等控制指令。该组件遵循高质量编程规范，采用**面向对象编程（封装/继承/多态）** 与**函数式编程（Lambda 优化）** 结合的设计思路，支持跨平台编译（Windows/Linux/macOS），并通过 Google Test（GTest）构建完善的测试防护网，确保指令执行的正确性和稳定性。



## 项目结构



```Plain Text
C++作业/
├── include/                # 头文件目录（可选，若项目规模扩展）
│   └── executor.h          # 核心类接口定义
├── src/
│   ├── executor.cpp        # Executor 类实现
│   └── ExecutorImpl.cpp    # 面向对象优化后的实现（可选，基于实验2-3设计）
├── tests/
│   └── ExecutorTest.cpp    # GTest 单元测试用例
├── CMakeLists.txt          # CMake 编译配置文件
├── .gitignore              # Git 忽略文件（忽略 build/ 等编译产物）
└── README.md               # 项目说明文档（本文档）
```



### 核心文件说明



| 文件/目录          | 功能描述                                                                   |
| ------------------ | -------------------------------------------------------------------------- |
| `executor.h`       | 定义 Executor 类接口，包含初始化、指令执行、状态查询等核心方法。           |
| `executor.cpp`     | 实现 Executor 类的指令逻辑（基本指令 M/L/R、加速 F、倒车 B、掉头 TR 等）。 |
| `ExecutorTest.cpp` | 基于 GTest 的单元测试，覆盖初始化、所有指令功能的验证。                    |
| `CMakeLists.txt`   | 配置编译规则，生成可执行文件和测试程序（如 `test_executor`）。             |
| `.gitignore`       | 指定 Git 忽略的文件（如 `build/` 编译目录、`.exe` 可执行文件）。           |


## 环境准备



### 依赖工具



1. **C++ 编译器**：

    - Windows：Visual Studio 2019+（MSVC）

    - Linux/macOS：GCC 11+ 或 Clang 14+

2. **构建工具**：CMake 4.2.0+（参考之前的 CMake 更新步骤）

3. **版本控制**：Git

4. **测试框架**：Google Test（GTest），推荐通过 **vcpkg** 安装（依赖管理更便捷）



### 依赖安装（以 GTest 为例）



使用 vcpkg 安装 GTest（适用于 Windows/Linux/macOS）：



```Bash
# 1. 克隆 vcpkg（若未安装）
git clone https://github.com/microsoft/vcpkg.git
# 2. 初始化 vcpkg（Windows）
.\vcpkg\bootstrap-vcpkg.bat
# 3. 安装 GTest（x64 架构，Windows）
.\vcpkg\vcpkg install gtest:x64-windows
# Linux/macOS 初始化并安装
./vcpkg/bootstrap-vcpkg.sh
./vcpkg/vcpkg install gtest:x64-linux  # Linux
./vcpkg/vcpkg install gtest:x64-osx    # macOS
```



## 快速开始



### 1. 克隆项目



```Bash
git clone https://github.com/你的用户名/Executor.git  # 替换为你的 GitHub/Gitee 仓库地址
cd Executor
```



### 2. 编译项目（CMake 命令行）



推荐使用「源码与构建产物分离」的方式（避免污染源码目录）：



```Bash
# 1. 创建并进入 build 目录
mkdir build && cd build
# 2. 生成构建文件（指定 vcpkg 工具链，若安装了 GTest）
cmake .. -DCMAKE_TOOLCHAIN_FILE="你的 vcpkg 路径/scripts/buildsystems/vcpkg.cmake"
# 3. 编译（Debug 模式，Windows 可省略 --config Debug，默认 Debug）
cmake --build . --config Debug
```



### 3. 编译项目（VS Code + CMake Tools 扩展）



1. 打开项目文件夹，安装「CMake Tools」和「C/C++」扩展；

2. 点击底部状态栏「选择工具包」，选择你的编译器（如 `Visual Studio 2022 Release - x64`）；

3. 点击底部「生成」按钮（锤子图标），自动完成编译。



## 核心功能说明



Executor 组件支持多种指令，指令行为遵循需求定义，核心功能如下：



### 1. 基础功能（需求1）



#### 初始化



- 默认初始化：`Executor executor;` → 位置 (0,0)，朝向 'N'；

- 自定义初始化：`Executor executor(5, 3, 'E');` → 位置 (5,3)，朝向 'E'。



#### 基本指令



| 指令 | 行为描述                                                          |
| ---- | ----------------------------------------------------------------- |
| M    | 前进 1 格（朝向决定方向：N→Y+1，S→Y-1，E→X+1，W→X-1），朝向不变。 |
| L    | 左转 90°（N→W→S→E→N），位置不变。                                 |
| R    | 右转 90°（N→E→S→W→N），位置不变。                                 |


#### 批量指令执行



通过 `executeCommands` 执行字符串指令，例如：



```C++
executor.executeCommands("MRM");  // 先前进→右转→前进
```



### 2. 加速功能（需求2）



- 指令 **F**：切换加速状态（第一次 F 进入加速，第二次 F 取消）；

- 加速状态下的指令行为：

    - M：前进 2 格（一格一格移动，不跳跃）；

    - L：先前进 1 格，再左转 90°；

    - R：先前进 1 格，再右转 90°。



### 3. 倒车功能（需求3）



- 指令 **B**：切换倒车状态（第一次 B 进入倒车，第二次 B 取消）；

- 倒车状态下的指令行为：

    - M：倒退 1 格（朝向不变，N→Y-1，S→Y+1，E→X-1，W→X+1）；

    - L：右转 90°（与正常 L 相反），位置不变；

    - R：左转 90°（与正常 R 相反），位置不变；

- **叠加状态（F+B）**：

    - M：倒退 2 格；

    - L：先倒退 1 格，再右转 90°；

    - R：先倒退 1 格，再左转 90°。



### 4. 掉头功能（需求4）



- 指令 **TR**：掉头操作（不受 B 影响，受 F 影响）；

- 正常状态：左转 90° → 前进 1 格 → 左转 90°；

- 加速状态（F 激活）：前进 1 格 → 左转 90° → 前进 1 格 → 左转 90°。



## 使用示例



以下是一个简单的使用示例，演示初始化、执行指令、查询状态的流程：



```C++
#include "executor.h"
#include <iostream>

int main() {
    // 1. 自定义初始化：位置 (1, 2)，朝向 'S'
    Executor executor(1, 2, 'S');
    
    // 2. 执行指令："MLMRMLMM"（参考需求文档示例）
    executor.executeCommands("MLMRMLMM");
    
    // 3. 查询状态
    int32_t x, y;
    char heading;
    executor.getStatus(x, y, heading);
    
    // 4. 输出结果（预期：x=4, y=0, heading='E'）
    std::cout << "当前位置：(" << x << ", " << y << ")" << std::endl;
    std::cout << "当前朝向：" << heading << std::endl;
    
    return 0;
}
```



编译并运行上述代码，输出如下：



```Plain Text
当前位置：(4, 0)
当前朝向：E
```



## 测试指南



### 1. 运行测试程序



编译完成后，测试程序（`test_executor.exe`）位于 `build/Debug` 或 `build/Release` 目录：



```Bash
# 进入测试程序目录
cd build/Debug
# 运行测试
./test_executor.exe
```



### 2. 使用 CTest 运行测试



```Bash
# 在 build 目录下执行
ctest -C Debug  # Debug 模式，Release 模式替换为 -C Release
```



### 3. 测试覆盖范围



测试用例覆盖以下场景：



- 初始化（默认/自定义）；

- 基本指令（M/L/R）；

- 加速指令 F；

- 倒车指令 B；

- 掉头指令 TR；

- 状态叠加（F+B）。



## 版本控制规范



1. **忽略编译产物**：`.gitignore` 已配置忽略 `build/` 目录，无需手动删除；

2. **提交规范**：提交信息需清晰，例如 `feat: 添加倒车指令 B 的实现`、`test: 补充 TR 指令的测试用例`；

3. **双远程同步**：可配置 `origin` 同时关联 GitHub 和 Gitee，实现一次 push 同步双仓库（参考之前的 Git 配置步骤）。



## 常见问题（FAQ）



1. **CMake 报错“找不到 GTest”**：确保指定了正确的 vcpkg 工具链路径，或手动设置 `GTEST_ROOT` 环境变量指向 GTest 安装目录。

2. **VS Code 中 CMake 扩展无法识别工具包**：检查编译器是否正确安装，重启 VS Code 或重新选择工具包（底部状态栏「选择工具包」）。

3. **测试程序运行失败**：确认 GTest 已正确链接，或检查代码中指令逻辑是否与需求一致（如 TR 指令的加速状态处理）。



## 参考文献



1. [CMake 官方文档](https://cmake.org/documentation/)

2. [Google Test 官方文档](https://google.github.io/googletest/)

3. [vcpkg 依赖管理](https://vcpkg.io/)

4. [C++ 参考手册](https://en.cppreference.com/)

5. [VS Code CMake Tools 扩展指南](https://code.visualstudio.com/docs/cpp/cmake-linux)