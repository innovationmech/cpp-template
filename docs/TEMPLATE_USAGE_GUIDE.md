# cpp-template 使用指南

本指南详细说明如何使用 cpp-template 创建新的 C++ 项目，包括模板获取、定制配置、构建验证等完整流程。

## 目录

- [快速开始](#快速开始)
- [模板获取](#模板获取)
- [项目定制](#项目定制)
- [依赖配置](#依赖配置)
- [构建验证](#构建验证)
- [开发工作流](#开发工作流)
- [最佳实践](#最佳实践)
- [故障排除](#故障排除)

## 快速开始

### 30秒快速创建项目

```bash
# 1. 克隆模板
git clone https://github.com/your-username/cpp-template.git my-new-project
cd my-new-project

# 2. 替换项目名称
python scripts/replace_placeholders.py my-new-project

# 3. 初始化依赖
git submodule update --init --recursive

# 4. 构建项目
mkdir build && cd build
cmake ..
cmake --build .

# 5. 运行测试
ctest
```

### 验证安装

```bash
# 运行主程序
./build/src/my-new-project

# 运行示例
./build/examples/usage_scenarios

# 查看依赖演示
./build/examples/dependency_demo
```

## 模板获取

### 方法一：直接克隆（推荐）

```bash
# 克隆最新版本
git clone https://github.com/your-username/cpp-template.git my-project
cd my-project

# 移除原始 Git 历史
rm -rf .git
git init
git add .
git commit -m "Initial commit from cpp-template"
```

### 方法二：使用 GitHub 模板

1. 访问 [cpp-template GitHub 页面](https://github.com/your-username/cpp-template)
2. 点击 "Use this template" 按钮
3. 创建新仓库
4. 克隆新创建的仓库

### 方法三：下载压缩包

```bash
# 下载并解压
wget https://github.com/your-username/cpp-template/archive/main.zip
unzip main.zip
mv cpp-template-main my-project
cd my-project
```

## 项目定制

### 自动化定制（推荐）

使用提供的脚本自动替换所有占位符：

```bash
# 基本用法
python scripts/replace_placeholders.py my-awesome-project

# 高级选项
python scripts/replace_placeholders.py my-project \
    --project-root /path/to/project \
    --no-backup

# 预览更改（不实际修改文件）
python scripts/replace_placeholders.py my-project --dry-run
```

脚本会自动处理：
- 项目名称替换（cpp-template → my-project）
- 命名空间更新（cpp_template → my_project）
- 文档更新
- 目录结构调整
- CMake 配置更新

### 手动定制

如果需要更精细的控制，可以手动进行定制：

#### 1. 更新项目基本信息

**CMakeLists.txt**：
```cmake
# 更新项目声明
project(my-awesome-project 
    VERSION 1.0.0 
    LANGUAGES CXX
    DESCRIPTION "My awesome C++ project"
    HOMEPAGE_URL "https://github.com/myusername/my-awesome-project"
)
```

#### 2. 更新文档

**README.md**：
```markdown
# My Awesome Project

A brief description of what your project does.

## Features

- Feature 1
- Feature 2
- Feature 3
```

#### 3. 更新包配置

**vcpkg.json**：
```json
{
  "name": "my-awesome-project",
  "version": "1.0.0",
  "description": "My awesome C++ project",
  "dependencies": [
    "fmt",
    "spdlog",
    "nlohmann-json",
    "gtest"
  ]
}
```

#### 4. 更新命名空间

在源代码中更新命名空间：

```cpp
// 从
namespace cpp_template {
    // ...
}

// 改为
namespace my_awesome_project {
    // ...
}
```

## 依赖配置

### 选择依赖管理策略

cpp-template 支持多种依赖管理方法，根据项目需求选择：

#### 1. vcpkg（推荐用于大多数项目）

**优点**：
- 包管理简单
- 版本控制良好
- 跨平台支持
- 大量可用包

**配置**：
```bash
# 安装 vcpkg
git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh

# 构建项目
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

#### 2. Git Submodules（适合需要特定版本的项目）

**优点**：
- 精确版本控制
- 源码可见
- 离线构建

**配置**：
```bash
# 添加子模块
git submodule add https://github.com/nlohmann/json.git third_party/json
git submodule update --init --recursive

# 构建项目
cmake -B build -S .
cmake --build build
```

#### 3. 手动依赖（适合自定义库）

**优点**：
- 完全控制
- 自定义修改
- 简单集成

**配置**：
将库文件放入 `third_party/manual/` 目录，并在 CMakeLists.txt 中配置。

#### 4. 系统包管理器（适合系统级依赖）

**优点**：
- 系统集成
- 自动更新
- 减少构建时间

**配置**：
```bash
# macOS
brew install fmt nlohmann-json

# Ubuntu
sudo apt-get install libfmt-dev nlohmann-json3-dev

# 构建项目
cmake -B build -S .
cmake --build build
```

### 混合依赖策略

可以组合使用多种依赖管理方法：

```cmake
# CMakeLists.txt 示例

# 优先使用 vcpkg
find_package(fmt CONFIG QUIET)
if(NOT fmt_FOUND)
    # 回退到系统包管理器
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(fmt REQUIRED fmt)
endif()

# 使用 Git 子模块作为备选
if(NOT fmt_FOUND)
    add_subdirectory(third_party/fmt)
endif()
```

## 构建验证

### 基本构建测试

```bash
# 清理构建
rm -rf build
mkdir build && cd build

# 配置项目
cmake ..

# 构建所有目标
cmake --build .

# 运行测试
ctest --verbose
```

### 多配置构建测试

```bash
# Debug 构建
cmake -B build-debug -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug

# Release 构建
cmake -B build-release -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build-release

# 运行性能测试
./build-release/examples/usage_scenarios
```

### 跨平台构建验证

#### Linux/macOS
```bash
# GCC
export CC=gcc CXX=g++
cmake -B build-gcc -S .
cmake --build build-gcc

# Clang
export CC=clang CXX=clang++
cmake -B build-clang -S .
cmake --build build-clang
```

#### Windows
```cmd
REM Visual Studio
cmake -B build-vs -S . -G "Visual Studio 16 2019"
cmake --build build-vs --config Release

REM MinGW
cmake -B build-mingw -S . -G "MinGW Makefiles"
cmake --build build-mingw
```

### 依赖验证

```bash
# 验证所有依赖管理方法
./build/examples/dependency_demo

# 检查链接的库
ldd ./build/src/my-project  # Linux
otool -L ./build/src/my-project  # macOS
```

## 开发工作流

### 1. 项目初始化

```bash
# 创建项目
git clone cpp-template my-project
cd my-project

# 定制项目
python scripts/replace_placeholders.py my-project

# 初始化 Git
rm -rf .git
git init
git add .
git commit -m "Initial commit"
```

### 2. 开发环境设置

```bash
# 设置开发构建
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON

# 配置 IDE（可选）
cmake -B build -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

### 3. 日常开发循环

```bash
# 增量构建
cmake --build build

# 运行测试
ctest --test-dir build

# 运行特定测试
ctest --test-dir build -R "test_core"

# 调试运行
gdb ./build/src/my-project
```

### 4. 添加新功能

#### 添加新模块

```bash
# 创建模块目录
mkdir -p modules/new-module/{include/new-module,src,tests}

# 创建 CMakeLists.txt
cat > modules/new-module/CMakeLists.txt << 'EOF'
add_library(new-module
    src/new_module.cpp
)

target_include_directories(new-module
    PUBLIC include
    PRIVATE src
)

target_link_libraries(new-module
    PUBLIC core
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
EOF
```

#### 添加新依赖

```bash
# 使用 vcpkg
echo '    "new-dependency"' >> vcpkg.json

# 或添加 Git 子模块
git submodule add https://github.com/user/repo.git third_party/repo

# 更新 CMakeLists.txt
echo 'find_package(new-dependency CONFIG REQUIRED)' >> CMakeLists.txt
```

### 5. 发布准备

```bash
# 运行完整测试套件
ctest --test-dir build --verbose

# 构建发布版本
cmake -B build-release -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build-release

# 运行性能测试
./build-release/examples/usage_scenarios

# 生成文档（如果配置了 Doxygen）
cmake --build build --target docs
```

## 最佳实践

### 1. 项目组织

- **保持模块化**：将功能分解为独立的模块
- **清晰的接口**：定义明确的 API 边界
- **一致的命名**：遵循项目命名约定
- **文档同步**：代码变更时同步更新文档

### 2. 依赖管理

- **固定版本**：在生产项目中固定依赖版本
- **最小依赖**：只添加必要的依赖
- **定期更新**：定期检查和更新依赖
- **安全审计**：关注依赖的安全更新

### 3. 构建配置

- **多配置支持**：支持 Debug/Release 构建
- **编译器警告**：启用并修复编译器警告
- **静态分析**：集成静态分析工具
- **持续集成**：设置 CI/CD 流水线

### 4. 测试策略

- **测试驱动**：先写测试，后写实现
- **覆盖率目标**：维持高测试覆盖率
- **性能测试**：包含性能回归测试
- **集成测试**：测试组件间交互

### 5. 版本控制

- **语义化版本**：使用语义化版本号
- **变更日志**：维护详细的变更记录
- **标签发布**：使用 Git 标签标记发布
- **分支策略**：采用合适的分支策略

## 故障排除

### 常见问题

#### 1. CMake 配置失败

**问题**：CMake 找不到依赖
```
CMake Error: Could not find package XYZ
```

**解决方案**：
```bash
# 检查 vcpkg 工具链
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg.cmake

# 或安装系统依赖
sudo apt-get install libxyz-dev  # Ubuntu
brew install xyz                 # macOS
```

#### 2. 编译错误

**问题**：C++ 标准不兼容
```
error: 'auto' type specifier is a C++11 extension
```

**解决方案**：
```cmake
# 确保 CMakeLists.txt 中设置了正确的 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

#### 3. 链接错误

**问题**：找不到库文件
```
undefined reference to 'function_name'
```

**解决方案**：
```cmake
# 检查库链接顺序
target_link_libraries(my-target
    PRIVATE
        dependency1
        dependency2
)
```

#### 4. 测试失败

**问题**：测试运行时错误
```
Segmentation fault in test
```

**解决方案**：
```bash
# 使用调试器
gdb --args ./build/tests/test_name

# 或使用 Valgrind
valgrind --tool=memcheck ./build/tests/test_name
```

### 调试技巧

#### 1. 详细构建输出

```bash
# CMake 详细输出
cmake -B build -S . --debug-output

# Make 详细输出
cmake --build build --verbose

# 或直接使用 make
make -C build VERBOSE=1
```

#### 2. 依赖分析

```bash
# 查看链接的库
ldd ./build/src/my-project

# 查看符号
nm ./build/libs/core/libcore.a

# 查看依赖树
cmake --build build --target help
```

#### 3. 测试调试

```bash
# 运行单个测试
ctest --test-dir build -R "specific_test" --verbose

# 并行测试
ctest --test-dir build -j4

# 失败时停止
ctest --test-dir build --stop-on-failure
```

### 性能优化

#### 1. 编译优化

```cmake
# Release 构建优化
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# 链接时优化
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
```

#### 2. 并行构建

```bash
# 使用多核构建
cmake --build build --parallel $(nproc)

# 或指定核心数
cmake --build build -j8
```

#### 3. 缓存优化

```bash
# 使用 ccache
export CC="ccache gcc"
export CXX="ccache g++"
cmake -B build -S .
```

## 进阶主题

### 1. 自定义 CMake 模块

创建 `cmake/FindMyLibrary.cmake`：

```cmake
# 查找自定义库的 CMake 模块
find_path(MYLIBRARY_INCLUDE_DIR mylibrary.h)
find_library(MYLIBRARY_LIBRARY mylibrary)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MyLibrary
    REQUIRED_VARS MYLIBRARY_LIBRARY MYLIBRARY_INCLUDE_DIR
)

if(MyLibrary_FOUND AND NOT TARGET MyLibrary::MyLibrary)
    add_library(MyLibrary::MyLibrary UNKNOWN IMPORTED)
    set_target_properties(MyLibrary::MyLibrary PROPERTIES
        IMPORTED_LOCATION "${MYLIBRARY_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${MYLIBRARY_INCLUDE_DIR}"
    )
endif()
```

### 2. 交叉编译支持

```cmake
# 工具链文件示例 (toolchain-arm.cmake)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

set(CMAKE_FIND_ROOT_PATH /usr/arm-linux-gnueabihf)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
```

使用：
```bash
cmake -B build-arm -S . -DCMAKE_TOOLCHAIN_FILE=toolchain-arm.cmake
```

### 3. 包装和分发

```cmake
# 添加到主 CMakeLists.txt
include(CPack)
set(CPACK_PACKAGE_NAME "my-project")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "My awesome project")
set(CPACK_GENERATOR "TGZ;ZIP")
```

生成包：
```bash
cmake --build build --target package
```

## 总结

cpp-template 提供了一个完整的 C++ 项目起始点，包含：

- **现代 C++ 实践**：C++17 标准，模块化架构
- **灵活的依赖管理**：支持多种依赖管理策略
- **完整的测试框架**：单元测试和集成测试
- **跨平台支持**：Windows、Linux、macOS
- **详细的文档**：使用指南和最佳实践
- **自动化工具**：占位符替换和构建脚本

通过遵循本指南，您可以快速创建高质量的 C++ 项目，并建立良好的开发工作流。记住定期更新依赖、维护测试覆盖率，并遵循最佳实践以确保项目的长期成功。

如有问题或需要帮助，请查阅项目文档或提交 Issue。