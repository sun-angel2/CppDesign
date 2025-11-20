# C++ 智能车指令执行器 (Executor)

本项目是一个基于 C++ 实现的智能车控制系统核心组件——指令执行器。它能够解析并执行一系列复杂的移动和转向指令，支持多种状态（如加速、倒车）的切换与叠加。项目采用CMake进行构建管理，并使用Google Test框架进行单元测试，确保代码质量与功能的正确性。

## 核心功能

`Executor` 组件能够管理车辆的位置 `(x, y)` 和朝向 `(N, S, E, W)`，并响应以下指令：

### 1. 基础指令

| 指令 | 行为描述 |
| :--- | :--- |
| `M` | **前进**: 朝当前方向前进1格。 |
| `L` | **左转**: 原地左转90度 (N→W→S→E→N)。 |
| `R` | **右转**: 原地右转90度 (N→E→S→W→N)。 |

### 2. 状态切换指令

| 指令 | 状态 | 描述 |
| :--- | :--- | :--- |
| `F` | **加速状态** | 切换车辆的加速状态。此状态会改变 `M`, `L`, `R`, `TR` 的行为。 |
| `B` | **倒车状态** | 切换车辆的倒车状态。此状态会改变 `M`, `L`, `R` 的行为。 |

### 3. 组合状态下的指令行为

- **加速状态 (`F` 激活)**:
  - `M`: 前进2格（分两次前进1格，每次移动更新位置）。
  - `L`: 先前进1格，再左转90度。
  - `R`: 先前进1格，再右转90度。

- **倒车状态 (`B` 激活)**:
  - `M`: **后退**1格，朝向不变。
  - `L`: **右转**90度（与常规行为相反）。
  - `R`: **左转**90度（与常规行为相反）。

- **加速与倒车叠加状态 (`F` 和 `B` 均激活)**:
  - `M`: **后退**2格（分两次后退1格，每次移动更新位置）。
  - `L`: 先**后退**1格，再**右转**90度。
  - `R`: 先**后退**1格，再**左转**90度。

### 4. 特殊指令

- `TR` (**掉头**):
  - **普通状态**: 左转90度 → 前进1格 → 左转90度。
  - **加速状态**: 前进1格 → 左转90度 → 前进1格 → 左转90度。
  - **注意**: `TR` 指令不受倒车状态 (`B`) 影响。

#### 示例序列

假设智能车初始位置为 `(0,0)`，朝向 `N` (北)。

| 指令序列 | 最终位置 | 最终朝向 | 描述 |
| :-------- | :-------- | :-------- | :--- |
| `M`       | `(0,1)`   | `N`       | 前进1格。 |
| `RM`      | `(1,0)`   | `E`       | 右转，然后前进1格。 |
| `FM`      | `(0,2)`   | `N`       | 开启加速，前进2格。 |
| `BMM`     | `(0,-2)`  | `N`       | 开启倒车，后退2格。 |
| `FBTR`    | `(-1,1)`  | `S`       | 开启加速与倒车，但`TR`不受倒车影响。执行加速状态下的`TR`指令：前进1格 → 左转90度 → 前进1格 → 左转90度。 |

---

## 环境配置

在开始之前，请确保您的系统已安装以下工具：

1.  **C++ 编译器**:
    - **Windows**: Visual Studio 2019 或更高版本 (MSVC)。
    - **Linux/macOS**: GCC 11+ 或 Clang 14+。
2.  **CMake**: 版本 3.10 或更高。
3.  **Git**: 用于版本控制。
4.  **vcpkg (推荐)**: 用于管理第三方库，本项目通过它安装 `GTest`。

**使用 vcpkg 安装 GTest:**

```bash
# 1. 克隆并初始化 vcpkg
git clone https://github.com/microsoft/vcpkg.git
# Windows
.\vcpkg\bootstrap-vcpkg.bat
# Linux/macOS
./vcpkg/bootstrap-vcpkg.sh

# 2. 安装 GTest (以 Windows x64 为例)
.\vcpkg\vcpkg install gtest:x64-windows
```
*请将 `x64-windows` 替换为您的目标平台，如 `x64-linux` 或 `x64-osx`。*

---

## 快速开始

### 1. 克隆项目

```bash
git clone <您的仓库地址>
cd <项目目录>
```

### 2. 编译项目

我们推荐使用“源码外构建”(out-of-source build)，这可以保持源码目录的整洁。

```bash
# 1. 创建构建目录
mkdir build
cd build

# 2. 使用 CMake 生成构建文件
# 将 <path-to-vcpkg> 替换为您的 vcpkg 实际路径
cmake .. -DCMAKE_TOOLCHAIN_FILE="<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake"

# 3. 编译项目
cmake --build .
```

### 3. 运行测试

编译成功后，测试程序将生成在 `build` 目录中。

#### 方式一：使用 CTest

在 `build` 目录下执行：

```bash
# -C Debug 是为了确保在 Debug 模式下运行，如果您的构建配置是 Release，请相应修改
ctest -C Debug
```

#### 方式二：直接运行测试程序

测试程序的可执行文件通常位于 `build/Debug` 目录下。

```bash
# Windows
.\Debug\test_executor.exe

# Linux/macOS
./test_executor
```
所有测试通过，即代表所有功能均按预期工作。

---

## 使用示例 (How to Integrate)

本项目提供 `Executor` 类作为核心逻辑组件。您可以在自己的 C++ 项目中包含 `executor.h` 并链接 `executor.cpp` 来使用它。

以下是一个简单的示例，展示如何在 `main` 函数中使用 `Executor`：

```cpp
#include "executor.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    Executor executor;

    // 初始状态: (0,0), North
    std::cout << "Initial: Pos(" << executor.getX() << "," << executor.getY() << "), Dir(" << executor.getHeading() << ")" << std::endl;

    // 执行指令序列
    std::vector<std::string> commands = {"F", "M", "R", "M", "B", "M"};
    for (const std::string& cmd : commands) {
        // executeCommand 接收 char, 所以我们需要将 string 的第一个字符传入
        if (!cmd.empty()) {
            executor.executeCommand(cmd[0]);
        }
        std::cout << "Command: " << cmd << " -> Pos(" << executor.getX() << "," << executor.getY() << "), Dir(" << executor.getHeading() << ")" << std::endl;
    }

    // 最终状态
    std::cout << "Final: Pos(" << executor.getX() << "," << executor.getY() << "), Dir(" << executor.getHeading() << ")" << std::endl;

    return 0;
}
```

将上述代码保存为 `main.cpp`，并在您的 `CMakeLists.txt` 中添加或修改为：

```cmake
# 添加可执行文件
add_executable(my_vehicle_app main.cpp executor.cpp)

# 链接必要的库
# 假设 Executor 相关的源文件编译为库 (例如通过 add_library)
# 如果 executor.cpp 直接作为源文件加入 my_vehicle_app，则不需要 target_link_libraries Executor
# 如果 Executor 是一个独立的库，请确保它已被正确定义，例如:
# add_library(Executor executor.cpp executor.h)
# 之后再链接:
# target_link_libraries(my_vehicle_app PRIVATE Executor)

# 简单的做法是将所有源文件直接添加到可执行文件
# (如果 Executor.cpp 不作为单独的库存在)
target_sources(my_vehicle_app PRIVATE executor.cpp executor.h) # 添加源文件
```
*请根据您的实际 `CMakeLists.txt` 配置调整。*

---

## 代码结构

```
.
├── executor.h          # 定义 Executor 类的接口和成员，管理智能车状态和行为。
├── executor.cpp        # 实现 Executor 类的所有方法，包含指令解析和执行逻辑。
├── ExecutorTest.cpp    # 包含使用 Google Test 框架编写的单元测试，验证 Executor 功能的正确性。
├── CMakeLists.txt      # 项目的 CMake 构建脚本，负责编译配置、依赖管理和测试集成。
├── README.md           # 本项目说明文档。
├── LICENSE             # 项目的许可证信息。
└── vehicle_behavior.h  # 可能包含智能车相关的枚举、常量或共享结构定义。
```

---

## 许可证

本项目采用 [MIT 许可证](LICENSE)。详情请参阅 `LICENSE` 文件。