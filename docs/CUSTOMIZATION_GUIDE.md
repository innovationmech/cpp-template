# cpp-template 定制指南

本指南详细说明如何根据不同项目需求定制 cpp-template，包括项目结构调整、依赖管理配置、构建系统定制等方面。

## 目录

- [快速开始](#快速开始)
- [项目结构定制](#项目结构定制)
- [依赖管理定制](#依赖管理定制)
- [构建系统定制](#构建系统定制)
- [模块系统定制](#模块系统定制)
- [测试框架定制](#测试框架定制)
- [文档系统定制](#文档系统定制)
- [CI/CD 定制](#cicd-定制)
- [常见定制场景](#常见定制场景)

## 快速开始

### 1. 克隆模板

```bash
git clone <your-template-repo> my-new-project
cd my-new-project
```

### 2. 基本定制步骤

1. **更新项目信息**：修改 `CMakeLists.txt` 中的项目名称和版本
2. **替换占位符**：使用查找替换功能更新所有 `cpp-template` 引用
3. **配置依赖**：根据需求选择和配置依赖管理方法
4. **调整结构**：根据项目需求调整目录结构
5. **更新文档**：修改 README.md 和相关文档

## 项目结构定制

### 标准结构

```
cpp-template/
├── libs/                    # 核心库
├── src/                     # 主应用程序
├── modules/                 # 可选模块
├── third_party/            # 第三方依赖
├── examples/               # 示例代码
├── tests/                  # 测试代码
├── docs/                   # 文档
├── scripts/                # 构建脚本
└── .github/                # CI/CD 配置
```

### 定制选项

#### 1. 简化结构（小型项目）

```
my-project/
├── src/                    # 源代码
├── include/                # 头文件
├── tests/                  # 测试
├── third_party/           # 依赖
└── CMakeLists.txt
```

**实现步骤**：
1. 删除不需要的目录（`libs/`, `modules/`, `examples/`）
2. 将核心代码移动到 `src/` 和 `include/`
3. 简化 `CMakeLists.txt`

#### 2. 库项目结构

```
my-library/
├── include/my-library/     # 公共头文件
├── src/                    # 实现文件
├── tests/                  # 测试
├── examples/               # 使用示例
├── docs/                   # API 文档
└── CMakeLists.txt
```

**实现步骤**：
1. 重组头文件到 `include/` 目录
2. 配置 CMake 为库项目
3. 添加安装和导出配置

#### 3. 多模块项目结构

```
my-complex-project/
├── core/                   # 核心模块
├── modules/
│   ├── module-a/
│   ├── module-b/
│   └── module-c/
├── apps/                   # 多个应用程序
│   ├── app1/
│   └── app2/
├── shared/                 # 共享组件
└── CMakeLists.txt
```

## 依赖管理定制

### 1. vcpkg 配置

#### 基本配置

**vcpkg.json**：
```json
{
  "name": "my-project",
  "version": "1.0.0",
  "dependencies": [
    "fmt",
    "spdlog",
    "nlohmann-json",
    "gtest"
  ]
}
```

#### 高级配置

```json
{
  "name": "my-project",
  "version": "1.0.0",
  "dependencies": [
    {
      "name": "boost",
      "features": ["system", "filesystem", "thread"]
    },
    {
      "name": "opencv",
      "version>=": "4.5.0"
    }
  ],
  "builtin-baseline": "2023.04.15",
  "overrides": [
    {
      "name": "fmt",
      "version": "9.1.0"
    }
  ]
}
```

### 2. Git Submodules 配置

#### 添加子模块

```bash
# 添加新的子模块
git submodule add https://github.com/fmtlib/fmt.git third_party/fmt
git submodule add https://github.com/gabime/spdlog.git third_party/spdlog
```

#### CMake 集成

```cmake
# 添加子模块
add_subdirectory(third_party/fmt)
add_subdirectory(third_party/spdlog)

# 链接到目标
target_link_libraries(my-target PRIVATE fmt::fmt spdlog::spdlog)
```

### 3. 手动依赖管理

#### 创建查找模块

**cmake/FindMyLibrary.cmake**：
```cmake
find_path(MYLIBRARY_INCLUDE_DIR
    NAMES mylibrary.h
    PATHS ${MYLIBRARY_ROOT}/include
)

find_library(MYLIBRARY_LIBRARY
    NAMES mylibrary
    PATHS ${MYLIBRARY_ROOT}/lib
)

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

## 构建系统定制

### 1. CMake 配置选项

```cmake
# 项目选项
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_TESTING "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(ENABLE_COVERAGE "Enable code coverage" OFF)
option(ENABLE_SANITIZERS "Enable sanitizers" OFF)

# 编译器特定选项
if(MSVC)
    option(USE_STATIC_RUNTIME "Use static runtime" OFF)
endif()
```

### 2. 编译器配置

```cmake
# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译器警告
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 调试信息
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=1)
endif()
```

### 3. 安装配置

```cmake
# 安装目标
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# 安装头文件
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# 生成配置文件
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)
```

## 模块系统定制

### 1. 创建新模块

#### 模块结构

```
modules/my-module/
├── include/my-module/
│   └── my_module.h
├── src/
│   └── my_module.cpp
├── tests/
│   └── test_my_module.cpp
└── CMakeLists.txt
```

#### 模块 CMakeLists.txt

```cmake
# 创建模块库
add_library(my-module
    src/my_module.cpp
)

# 设置包含目录
target_include_directories(my-module
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 链接依赖
target_link_libraries(my-module
    PUBLIC core
    PRIVATE ${REQUIRED_LIBRARIES}
)

# 添加测试
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
```

### 2. 模块接口标准化

#### 基础模块接口

```cpp
// include/my-module/my_module.h
#pragma once

#include "core/module_interface.h"
#include <string>
#include <memory>

class MyModule : public ModuleInterface {
public:
    explicit MyModule(const std::string& name);
    ~MyModule() override = default;
    
    // ModuleInterface implementation
    std::string getName() const override;
    std::string process(const std::string& input) override;
    ModuleStatistics getStatistics() const override;
    
    // Module-specific methods
    void configure(const Configuration& config);
    bool isReady() const;
    
private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
};
```

## 测试框架定制

### 1. Google Test 配置

```cmake
# 查找或获取 Google Test
find_package(GTest QUIET)
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)
endif()

# 创建测试
function(add_unit_test test_name)
    add_executable(${test_name} ${ARGN})
    target_link_libraries(${test_name}
        PRIVATE
            GTest::gtest_main
            ${PROJECT_NAME}
    )
    gtest_discover_tests(${test_name})
endfunction()
```

### 2. 自定义测试宏

```cpp
// tests/test_helpers.h
#pragma once

#include <gtest/gtest.h>
#include <memory>

// 测试夹具基类
class ModuleTestBase : public ::testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;
    
    std::unique_ptr<Core> core_;
};

// 参数化测试宏
#define INSTANTIATE_MODULE_TEST(TestClass, ModuleType) \
    INSTANTIATE_TEST_SUITE_P(ModuleType, TestClass, \
        ::testing::Values(std::make_shared<ModuleType>()))
```

### 3. 性能测试集成

```cmake
# Google Benchmark
find_package(benchmark QUIET)
if(benchmark_FOUND)
    function(add_benchmark bench_name)
        add_executable(${bench_name} ${ARGN})
        target_link_libraries(${bench_name}
            PRIVATE
                benchmark::benchmark
                ${PROJECT_NAME}
        )
    endfunction()
endif()
```

## 文档系统定制

### 1. Doxygen 配置

```cmake
# 查找 Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    # 配置 Doxygen
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN NO)
    
    doxygen_add_docs(docs
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        COMMENT "Generate API documentation"
    )
endif()
```

### 2. Sphinx 集成

```python
# docs/conf.py
project = 'My Project'
copyright = '2024, My Name'
author = 'My Name'

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.viewcode',
    'breathe',  # For C++ documentation
]

# Breathe configuration
breathe_projects = {
    "myproject": "../build/docs/xml"
}
breathe_default_project = "myproject"
```

## CI/CD 定制

### 1. GitHub Actions 配置

```yaml
# .github/workflows/ci.yml
name: CI

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main ]

jobs:
  build:
    strategy:
      matrix:
        os: [ubuntu-latest, windows-latest, macos-latest]
        build_type: [Debug, Release]
        
    runs-on: ${{ matrix.os }}
    
    steps:
    - uses: actions/checkout@v3
      with:
        submodules: recursive
        
    - name: Install vcpkg
      run: |
        git clone https://github.com/Microsoft/vcpkg.git
        ./vcpkg/bootstrap-vcpkg.sh
        
    - name: Configure CMake
      run: |
        cmake -B build -S . \
          -DCMAKE_BUILD_TYPE=${{ matrix.build_type }} \
          -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake
          
    - name: Build
      run: cmake --build build --config ${{ matrix.build_type }}
      
    - name: Test
      run: ctest --test-dir build --config ${{ matrix.build_type }}
```

### 2. 代码质量检查

```yaml
  code-quality:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v3
    
    - name: Run clang-format
      run: |
        find . -name '*.cpp' -o -name '*.h' | xargs clang-format --dry-run --Werror
        
    - name: Run clang-tidy
      run: |
        cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
        clang-tidy src/*.cpp -p build
```

## 常见定制场景

### 1. Web 服务项目

**依赖添加**：
```json
{
  "dependencies": [
    "crow",
    "nlohmann-json",
    "spdlog",
    "openssl"
  ]
}
```

**项目结构**：
```
web-service/
├── src/
│   ├── controllers/
│   ├── models/
│   ├── services/
│   └── main.cpp
├── config/
├── tests/
└── docker/
```

### 2. 游戏引擎项目

**依赖配置**：
```json
{
  "dependencies": [
    "sdl2",
    "opengl",
    "glew",
    "glm",
    "assimp"
  ]
}
```

**模块结构**：
```
game-engine/
├── engine/
│   ├── core/
│   ├── graphics/
│   ├── audio/
│   ├── physics/
│   └── input/
├── game/
└── tools/
```

### 3. 科学计算项目

**依赖配置**：
```json
{
  "dependencies": [
    "eigen3",
    "openblas",
    "hdf5",
    "boost-math"
  ]
}
```

**特殊配置**：
```cmake
# 启用 OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()

# 数学优化
target_compile_definitions(${PROJECT_NAME} PRIVATE
    EIGEN_USE_BLAS
    EIGEN_USE_LAPACKE
)
```

## 最佳实践

### 1. 版本管理

- 使用语义化版本控制
- 在 CMakeLists.txt 中维护版本信息
- 使用 Git 标签标记发布版本

### 2. 代码组织

- 保持头文件和实现文件分离
- 使用命名空间避免冲突
- 遵循一致的代码风格

### 3. 依赖管理

- 优先使用包管理器（vcpkg）
- 固定依赖版本以确保可重现构建
- 定期更新依赖项

### 4. 测试策略

- 编写单元测试覆盖核心功能
- 添加集成测试验证模块交互
- 使用持续集成自动运行测试

### 5. 文档维护

- 保持 README.md 更新
- 编写 API 文档
- 提供使用示例

## 故障排除

### 常见问题

1. **vcpkg 依赖安装失败**
   - 检查网络连接
   - 更新 vcpkg 到最新版本
   - 检查依赖项名称是否正确

2. **CMake 配置错误**
   - 检查 CMake 版本要求
   - 验证工具链文件路径
   - 清理构建目录重新配置

3. **链接错误**
   - 检查库依赖顺序
   - 验证库文件是否存在
   - 检查架构匹配（32位/64位）

4. **测试失败**
   - 检查测试数据路径
   - 验证环境变量设置
   - 查看详细错误信息

### 调试技巧

```bash
# 详细构建输出
cmake --build build --verbose

# CMake 调试信息
cmake -B build -S . --debug-output

# 测试详细输出
ctest --test-dir build --verbose
```

## 总结

本指南涵盖了 cpp-template 的主要定制方面，从基本的项目配置到高级的 CI/CD 集成。根据具体项目需求，选择合适的定制选项，并遵循最佳实践，可以快速构建出高质量的 C++ 项目。

记住定制的关键原则：
- 保持简单，只添加必要的功能
- 遵循标准和最佳实践
- 维护良好的文档
- 定期更新和维护

如有问题或需要进一步的定制帮助，请参考项目文档或提交 Issue。