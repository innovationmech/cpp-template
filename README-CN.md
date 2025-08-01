# cpp-template

一个综合的 C++ 项目模板，展示了使用 CMake 构建系统的现代 C++ 开发实践。此模板为 C++ 项目提供了一个坚实的基础，具有多种依赖管理策略、全面的测试和跨平台兼容性。

## 目录

- [特性](#特性)
- [项目结构](#项目结构)
- [先决条件](#先决条件)
- [快速开始](#快速开始)
- [依赖管理](#依赖管理)
- [构建项目](#构建项目)
- [测试](#测试)
- [开发环境设置](#开发环境设置)
- [自定义](#自定义)
- [故障排除](#故障排除)
- [贡献](#贡献)

## 特性

- **现代 C++ 标准**: C++17+ 并显式指定版本
- **模块化架构**: 清晰的职责分离和分层结构
- **多种依赖管理**: 支持 vcpkg、系统包、Git 子模块和手动依赖
- **全面的测试**: Google Test 集成，包含单元测试和集成测试
- **跨平台**: 在 macOS、Linux 和 Windows 上运行
- **CMake 最佳实践**: 分层构建系统和自定义模块
- **开发工具**: 代码格式化、静态分析和 IDE 集成

## 项目结构

```
cpp-template/
├── CMakeLists.txt              # 顶层构建配置
├── CMakePresets.json           # CMake 预设，用于不同配置
├── README.md                   # 此文档
├── vcpkg.json                  # vcpkg 清单文件
├── .gitmodules                 # Git 子模块配置
├── scripts/
│   └── setup-dev-env.sh        # 开发环境设置
├── src/                        # 应用程序源代码
│   ├── CMakeLists.txt
│   ├── main.cpp                # 应用程序入口点
│   └── modules/                # 应用程序模块
│       ├── config_manager.cpp
│       └── data_processor.cpp
├── include/                    # 公共头文件
│   └── cpp-template/
│       ├── cpp-template.h      # 主公共头文件
│       ├── core.h              # 核心功能
│       └── utils.h             # 工具函数
├── libs/                       # 内部项目库
│   ├── CMakeLists.txt
│   └── core/                   # 示例内部库
│       ├── CMakeLists.txt
│       ├── include/core/
│       └── src/
├── tests/                      # 单元和集成测试
│   ├── CMakeLists.txt
│   └── unit/
│       ├── basic_test.cpp
│       ├── core_test.cpp
│       ├── utils_test.cpp
│       └── integration_test.cpp
├── third_party/                # 外部依赖项
│   ├── CMakeLists.txt
│   ├── manual/                 # 手动管理的依赖项
│   │   └── mathutils/          # 示例手动依赖项
│   └── submodules/             # Git 子模块依赖项
└── cmake/                      # 自定义 CMake 模块
    ├── CompilerOptions.cmake   # 编译器配置
    ├── VcpkgHelpers.cmake      # vcpkg 集成工具
    ├── FindCustomLib.cmake     # 示例查找模块
    ├── VcpkgGuide.md          # vcpkg 使用文档
    ├── GitSubmodulesGuide.md  # Git 子模块文档
    ├── ManualDependenciesGuide.md # 手动依赖项文档
    └── SystemPackagesGuide.md # 系统包文档
```

## 先决条件

### 必需要求

- **CMake 3.20 或更高版本**: 构建系统生成器
- **C++17 兼容编译器**: GCC 7+、Clang 5+、MSVC 2017+ 或 Apple Clang 10+
- **Git**: 版本控制和子模块管理

### 平台特定要求

#### macOS
```bash
# 安装 Xcode 命令行工具
xcode-select --install

# 安装 Homebrew（推荐）
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 安装必要工具
brew install cmake git ninja
```

#### Ubuntu/Debian
```bash
# 更新软件包列表
sudo apt update

# 安装必要工具
sudo apt install build-essential cmake git ninja-build pkg-config
```

#### Fedora/RHEL/CentOS
```bash
# 安装开发工具
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake git ninja-build pkgconfig
```

#### Windows
```powershell
# 安装 Chocolatey（以管理员身份运行）
Set-ExecutionPolicy Bypass -Scope Process -Force
[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

# 安装必要工具
choco install cmake git ninja visualstudio2022buildtools
```

## 快速开始

### 1. 克隆仓库

```bash
# 使用子模块克隆
git clone --recurse-submodules https://github.com/your-username/cpp-template.git
cd cpp-template

# 或者分别克隆和初始化子模块
git clone https://github.com/your-username/cpp-template.git
cd cpp-template
git submodule update --init --recursive
```

### 2. 设置开发环境

```bash
# 运行设置脚本（macOS/Linux）
./scripts/setup-dev-env.sh

# 或者手动安装依赖项（参见开发环境设置部分）
```

### 3. 构建项目

```bash
# 基本构建
cmake -B build -S .
cmake --build build

# 或使用预设（如果可用）
cmake --preset default
cmake --build build
```

### 4. 运行测试

```bash
# 运行所有测试
cmake --build build --target test

# 或直接使用 ctest
cd build
ctest --output-on-failure
```

### 5. 运行应用程序

```bash
# 运行示例应用程序
./build/src/cpp-template
```

## 依赖管理

此项目支持四种不同的依赖管理方法。您可以根据需要使用这些方法的任意组合。

### 1. vcpkg（推荐）

vcpkg 是一个 C++ 包管理器，为多个平台提供预构建库。

#### 安装

```bash
# 克隆 vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# 启动 vcpkg
./bootstrap-vcpkg.sh  # Linux/macOS
# 或者
.\bootstrap-vcpkg.bat  # Windows
```

#### 使用

```bash
# 使用 vcpkg 工具链配置
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

# 构建项目
cmake --build build
```

#### 添加依赖项

编辑 `vcpkg.json` 来添加新依赖项：

```json
{
  "dependencies": [
    "existing-package",
    "new-package",
    {
      "name": "versioned-package",
      "version>=": "1.0.0"
    }
  ]
}
```

有关 vcpkg 的详细使用方法，请参见 [cmake/VcpkgGuide.md](cmake/VcpkgGuide.md)。

### 2. 系统包管理器

使用系统级包管理器来处理常见库。

#### macOS (Homebrew)

```bash
# 安装常见的 C++ 库
brew install boost openssl zlib curl sqlite postgresql

# 配置和构建
cmake -B build -S .
cmake --build build
```

#### Ubuntu/Debian

```bash
# 安装开发库
sudo apt install libboost-all-dev libssl-dev zlib1g-dev libcurl4-openssl-dev libsqlite3-dev libpq-dev

# 配置和构建
cmake -B build -S .
cmake --build build
```

有关系统包的详细使用方法，请参见 [cmake/SystemPackagesGuide.md](cmake/SystemPackagesGuide.md)。

### 3. Git 子模块

使用 Git 子模块来处理基于源代码的依赖项。

```bash
# 添加新的子模块
git submodule add https://github.com/nlohmann/json.git third_party/submodules/json

# 更新所有子模块
git submodule update --remote --merge

# 使用子模块构建
cmake -B build -S .
cmake --build build
```

有关子模块的详细使用方法，请参见 [cmake/GitSubmodulesGuide.md](cmake/GitSubmodulesGuide.md)。

### 4. 手动依赖项

将库直接放在 `third_party/manual/` 目录中。

```bash
# 添加手动依赖项
mkdir -p third_party/manual/mylib
# 将库文件复制到 third_party/manual/mylib/

# 使用手动依赖项构建
cmake -B build -S .
cmake --build build
```

有关手动依赖项管理的详细信息，请参见 [cmake/ManualDependenciesGuide.md](cmake/ManualDependenciesGuide.md)。

## 构建项目

### 构建配置

```bash
# 调试构建（默认）
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# 发布构建
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build

# 带调试信息的发布构建
cmake -B build -S . -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
```

### 构建选项

```bash
# 启用详细输出
cmake --build build --verbose

# 构建特定目标
cmake --build build --target cpp-template

# 并行构建
cmake --build build --parallel 4
```

### CMake 预设（可选）

如果配置了 `CMakePresets.json`：

```bash
# 列出可用的预设
cmake --list-presets

# 使用特定预设
cmake --preset vcpkg-release
cmake --build build
```

## 测试

### 运行测试

```bash
# 运行所有测试
cmake --build build --target test

# 使用详细输出运行测试
cd build
ctest --output-on-failure --verbose

# 运行特定测试
ctest -R "core_test"

# 并行运行测试
ctest --parallel 4
```

### 测试类别

- **单元测试**: 独立测试单个组件
- **集成测试**: 测试组件交互
- **系统测试**: 测试完整应用程序功能

### 添加新测试

1. 在 `tests/unit/` 中创建测试文件
2. 在 `tests/unit/CMakeLists.txt` 中添加测试
3. 遵循 Google Test 约定

示例:
```cpp
#include <gtest/gtest.h>
#include "your_component.h"

TEST(YourComponentTest, BasicFunctionality) {
    // 测试实现
    EXPECT_EQ(expected, actual);
}
```

## 开发环境设置

### 自动化设置

使用提供的设置脚本：

```bash
# macOS/Linux
./scripts/setup-dev-env.sh

# Windows (PowerShell 作为管理员)
.\scripts\setup-dev-env.ps1
```

### 手动设置

#### macOS

```bash
# 安装 Homebrew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 安装开发工具
brew install cmake git ninja boost openssl@3 zlib curl sqlite postgresql

# 安装 vcpkg（可选）
git clone https://github.com/Microsoft/vcpkg.git ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh
```

#### Linux (Ubuntu/Debian)

```bash
# 更新软件包列表
sudo apt update

# 安装构建工具
sudo apt install build-essential cmake git ninja-build pkg-config

# 安装常见库
sudo apt install libboost-all-dev libssl-dev zlib1g-dev libcurl4-openssl-dev libsqlite3-dev libpq-dev

# 安装 vcpkg（可选）
git clone https://github.com/Microsoft/vcpkg.git ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh
```

#### Windows

```powershell
# 安装 Chocolatey（作为管理员）
Set-ExecutionPolicy Bypass -Scope Process -Force
[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

# 安装开发工具
choco install cmake git ninja visualstudio2022buildtools

# 安装 vcpkg
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
```

### IDE 配置

#### Visual Studio Code

安装推荐的扩展：
- C/C++ Extension Pack
- CMake Tools
- GitLens

#### CLion

CLion 应该能自动检测 CMake 配置。

#### Visual Studio

在支持 CMake 的 Visual Studio 2019+ 中打开文件夹。

## 自定义

### 适应模板

1. **重命名项目**: 更新 `CMakeLists.txt` 和 `vcpkg.json`
2. **修改依赖项**: 编辑依赖配置文件
3. **更新结构**: 根据需要重新组织目录
4. **自定义构建**: 修改 CMake 配置

### 添加新组件

#### 内部库

```bash
# 创建新的库目录
mkdir -p libs/newlib/{include/newlib,src}

# 添加 CMakeLists.txt
# 更新 libs/CMakeLists.txt 以包含新库
```

#### 应用程序模块

```bash
# 创建新模块
mkdir -p src/modules/newmodule

# 添加源文件并更新 src/modules/CMakeLists.txt
```

### 配置选项

常见的自定义点：

- **C++ 标准**: 在主 `CMakeLists.txt` 中更改
- **编译器选项**: 修改 `cmake/CompilerOptions.cmake`
- **依赖项**: 更新相应的配置文件
- **测试**: 修改 `tests/CMakeLists.txt` 中的测试配置

## 故障排除

### 常见构建问题

#### CMake 配置失败

```bash
# 清除 CMake 缓存
rm -rf build/
cmake -B build -S .
```

#### 未找到依赖项

```bash
# 检查依赖项是否已安装
# 对于 vcpkg:
vcpkg list

# 对于系统包 (macOS):
brew list | grep package-name

# 对于系统包 (Ubuntu):
dpkg -l | grep package-name
```

#### 编译器错误

```bash
# 检查编译器版本
gcc --version
clang --version

# 确保支持 C++17
cmake -B build -S . -DCMAKE_CXX_STANDARD=17
```

### 平台特定问题

#### macOS

- **Xcode 命令行工具**: 运行 `xcode-select --install`
- **Homebrew 权限**: `sudo chown -R $(whoami) /opt/homebrew`
- **Apple Silicon**: 确保 ARM64 兼容性

#### Linux

- **缺少开发头文件**: 安装 `-dev` 软件包
- **权限问题**: 使用 `sudo` 安装系统软件包
- **编译器版本**: 确保 GCC 7+ 或 Clang 5+

#### Windows

- **Visual Studio 构建工具**: 大多数 C++ 包都需要
- **PowerShell 执行策略**: `Set-ExecutionPolicy RemoteSigned`
- **路径问题**: 安装后重启终端

### 调试信息

启用详细输出以进行调试：

```bash
# CMake 详细输出
cmake -B build -S . --debug-output

# 构建详细输出
cmake --build build --verbose

# 测试详细输出
cd build && ctest --verbose
```

### 获取帮助

1. **检查文档**: 查看 `cmake/` 目录中的指南
2. **搜索问题**: 在项目问题中查找类似问题
3. **启用调试输出**: 使用详细标志获取详细信息
4. **检查依赖项**: 验证所有必需的依赖项是否已安装

## 贡献

### 开发工作流程

1. Fork 仓库
2. 创建功能分支
3. 进行更改并添加测试
4. 确保所有测试通过
5. 提交拉取请求

### 代码标准

- 遵循 C++17 最佳实践
- 使用一致的命名约定
- 为新功能添加全面的测试
- 记录公共 API
- 遵循现有的代码风格

### 测试要求

- 所有新代码都必须有测试
- 测试必须在所有支持的平台上通过
- 为新功能添加集成测试
- 为关键路径添加性能测试

---

此模板为 C++ 项目提供了一个坚实的基础。根据您的具体需求和要求自定义它。有关特定方面的详细信息，请参阅 `cmake/` 目录中的文档文件。