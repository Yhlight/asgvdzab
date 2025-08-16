# ANTLR手动编译完整指南

## 🎯 概述

这个指南提供了多种方法来手动编译和设置ANTLR，解决CHTL项目中遇到的ANTLR版本兼容性和网络依赖问题。

## 📋 可用的解决方案

### 1. 完整手动编译（推荐用于生产环境）

**脚本：** `scripts/build_antlr_manual.sh` (Linux/Mac) 或 `scripts/build_antlr_windows.bat` (Windows)

**特点：**
- 从源码编译ANTLR工具和C++运行时
- 完全控制版本和配置
- 生成本地优化的库文件
- 支持所有ANTLR功能

**适用场景：**
- 生产环境部署
- 需要特定ANTLR版本
- 网络受限环境
- 性能敏感应用

### 2. 快速设置（推荐用于开发环境）

**脚本：** `scripts/quick_antlr_setup.sh`

**特点：**
- 多种安装方案可选
- 快速启动和运行
- 包含预编译工具下载
- 提供简化运行时

**适用场景：**
- 快速开发和测试
- 原型开发
- 教学和学习

### 3. 现代ANTLR API实现

**文件：** `include/compilers/modern_antlr_css_compiler.hpp`

**特点：**
- 使用最新ANTLR4 API
- 避免deprecated方法
- 现代C++17设计
- 完整错误处理

## 🚀 使用方法

### 方案A：完整编译（Linux）

```bash
# 运行完整编译脚本
./scripts/build_antlr_manual.sh

# 编译完成后更新项目配置
# 编辑 CMakeLists.txt，设置:
# set(ANTLR4_ROOT "/workspace/antlr-manual-install")
```

### 方案B：完整编译（Windows）

```cmd
REM 运行Windows编译脚本
scripts\build_antlr_windows.bat

REM 编译完成后更新项目配置
REM 编辑 CMakeLists.txt，设置:
REM set(ANTLR4_ROOT "C:\path\to\antlr-manual-install")
```

### 方案C：快速设置

```bash
# 运行快速设置脚本
./scripts/quick_antlr_setup.sh

# 选择合适的方案：
# 1) 下载预编译工具 (推荐)
# 2) 使用系统包管理器
# 3) 创建最小兼容实现
# 4) 全部尝试
```

## 📝 项目集成

### 1. 更新CMakeLists.txt

```cmake
# 根据你选择的方案设置ANTLR路径
set(ANTLR4_ROOT "/workspace/antlr-manual-install")  # 完整编译
# 或
set(ANTLR4_ROOT "/workspace/antlr-quick")           # 快速设置

# 查找ANTLR库
find_library(ANTLR4_LIB 
    NAMES antlr4-runtime
    PATHS ${ANTLR4_ROOT}/lib
    NO_DEFAULT_PATH
)

# 设置头文件路径
set(ANTLR4_INCLUDE_DIR "${ANTLR4_ROOT}/include")

# 如果没找到完整运行时，使用简化版本
if(NOT ANTLR4_LIB)
    include(${ANTLR4_ROOT}/FindANTLR4.cmake)
endif()

# 链接到项目
target_link_libraries(your_target ${ANTLR4_LIB})
target_include_directories(your_target PRIVATE ${ANTLR4_INCLUDE_DIR})
```

### 2. 使用现代ANTLR编译器

```cpp
#include "compilers/modern_antlr_css_compiler.hpp"

// 创建现代ANTLR CSS编译器
ModernANTLRCSSCompiler compiler;

// 配置选项
CSSCompileOptions options;
options.extractSelectors = true;
options.validateStandardCompliance = true;

// 编译CSS
std::string cssCode = ".test { color: red; }";
auto result = compiler.compileCSS(cssCode, options);

if (result.success) {
    std::cout << "选择器数量: " << result.selectors.size() << std::endl;
    for (const auto& selector : result.selectors) {
        std::cout << "选择器: " << selector.originalText 
                  << " (权重: " << selector.specificity << ")" << std::endl;
    }
}
```

## 🔧 生成ANTLR代码

### 重新生成CSS和JavaScript解析器

```bash
# 使用手动编译的ANTLR工具
cd /workspace

# 生成CSS解析器
./antlr-manual-install/antlr4 grammars/CSS3.g4 \
    -Dlanguage=Cpp \
    -o generated/antlr/css/ \
    -package chtl

# 生成JavaScript解析器  
./antlr-manual-install/antlr4 grammars/JavaScript.g4 \
    -Dlanguage=Cpp \
    -o generated/antlr/js/ \
    -package chtl
```

## 🐛 故障排除

### 编译错误

#### 1. "undefined reference to ANTLR symbols"
**解决方案：**
- 确保ANTLR库路径正确
- 检查库文件是否存在
- 验证链接配置

#### 2. "segmentation fault in tree->getText()"
**解决方案：**
- 使用现代ANTLR编译器
- 检查ANTLR版本兼容性
- 考虑使用简化运行时

#### 3. "utfcpp clone failed"
**解决方案：**
- 手动下载utfcpp
- 使用离线编译模式
- 跳过运行时子目录编译

### 版本兼容性

#### ANTLR工具版本 vs C++运行时版本
```bash
# 检查ANTLR工具版本
java -jar antlr4.jar -version

# 检查生成代码兼容性
grep -r "ANTLR_VERSION" generated/antlr/
```

#### C++标准版本
```cmake
# 确保使用C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

## 📊 性能对比

| 方案 | 编译时间 | 运行时性能 | 功能完整性 | 维护成本 |
|------|----------|------------|------------|----------|
| 完整编译 | 15-30分钟 | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ |
| 系统包管理器 | 2-5分钟 | ⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| 简化运行时 | 1分钟 | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐ |
| 现代API | - | ⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ |

## 🎯 最佳实践

### 开发环境
1. 使用快速设置脚本
2. 选择预编译工具方案
3. 使用现代ANTLR编译器
4. 启用详细调试输出

### 生产环境
1. 从源码完整编译
2. 固定ANTLR版本
3. 进行全面测试
4. 监控性能指标

### CI/CD环境
1. 缓存编译结果
2. 使用Docker容器
3. 并行编译优化
4. 自动化测试验证

## 📚 相关文档

- [ANTLR官方文档](https://www.antlr.org/)
- [ANTLR C++运行时文档](https://github.com/antlr/antlr4/tree/master/runtime/Cpp)
- [CMake FindANTLR4模块](https://cmake.org/cmake/help/latest/module/FindANTLR.html)
- [C++17标准特性](https://en.cppreference.com/w/cpp/17)

## 🤝 支持

如果遇到问题，请：

1. 检查这个指南的故障排除部分
2. 查看生成的README文件
3. 运行验证脚本检查环境
4. 启用详细日志输出进行调试

---

*最后更新：由CHTL项目自动生成*