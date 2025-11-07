# C++ 智能车指令执行器 (Executor)

本项目是一个基于 C++ 实现的智能车控制系统核心组件——指令执行器。它能够解析并执行一系列复杂的移动和转向指令，支持多种状态（如加速、倒车）的切换与叠加。

项目采用CMake进行构建管理，并使用Google Test框架进行单元测试，确保代码质量与功能的正确性。

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
  - `M`: 前进2格（逐格移动）。
  - `L`: 先前进1格，再左转90度。
  - `R`: 先前进1格，再右转90度。

- **倒车状态 (`B` 激活)**:
  - `M`: **后退**1格，朝向不变。
  - `L`: **右转**90度（与常规行为相反）。
  - `R`: **左转**90度（与常规行为相反）。

- **加速与倒车叠加状态 (`F` 和 `B` 均激活)**:
  - `M`: **后退**2格（逐格移动）。
  - `L`: 先**后退**1格，再**右转**90度。
  - `R`: 先**后退**1格，再**左转**90度。

### 4. 特殊指令

- `TR` (**掉头**):
  - **普通状态**: 左转90度 → 前进1格 → 左转90度。
  - **加速状态**: 前进1格 → 左转90度 → 前进1格 → 左转90度。
  - **注意**: `TR` 指令不受倒车状态 (`B`) 影响。

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

## 代码结构

```
.
├── executor.h          # Executor 类的头文件，定义接口和成员
├── executor.cpp        # Executor 类的实现文件，包含所有指令逻辑
├── ExecutorTest.cpp    # 单元测试文件，使用 GTest 框架
├── CMakeLists.txt      # CMake 构建脚本
└── README.md           # 本文档
```
