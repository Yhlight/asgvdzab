# ANTLR集成指南

## 选择的方案

根据你的环境，我们提供了多种ANTLR集成方案：

### 方案1：预编译工具 + 手动运行时
- ANTLR工具：预编译JAR
- C++运行时：手动编译或使用简化版本
- 适用于：所有环境

### 方案2：系统包管理器
- 完整的系统安装
- 适用于：有root权限的Linux/macOS

### 方案3：最小实现
- 仅提供接口兼容性
- 适用于：快速原型开发

## 在CHTL项目中使用

### 更新CMakeLists.txt：

```cmake
# 设置ANTLR路径
set(ANTLR4_ROOT "/workspace/antlr-quick")

# 方案1/2：查找完整运行时
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(ANTLR4 antlr4-runtime)
endif()

if(NOT ANTLR4_FOUND)
    find_library(ANTLR4_LIB 
        NAMES antlr4-runtime
        PATHS ${ANTLR4_ROOT}/lib /usr/local/lib /usr/lib
    )
    find_path(ANTLR4_INCLUDE_DIR 
        NAMES antlr4-runtime.h
        PATHS ${ANTLR4_ROOT}/include /usr/local/include /usr/include
    )
endif()

# 方案3：使用简化运行时
if(NOT ANTLR4_LIB)
    include(${ANTLR4_ROOT}/FindANTLR4.cmake)
endif()
```

### 生成代码：

```bash
# 使用下载的ANTLR工具
/workspace/antlr-quick/antlr4 grammars/CSS3.g4 -Dlanguage=Cpp -o generated/antlr/css/
/workspace/antlr-quick/antlr4 grammars/JavaScript.g4 -Dlanguage=Cpp -o generated/antlr/js/
```

## 故障排除

### 如果遇到编译错误：
1. 检查C++标准设置（需要C++17）
2. 确保ANTLR版本兼容
3. 考虑使用简化运行时（方案3）

### 如果生成的代码有问题：
1. 检查语法文件
2. 确保ANTLR工具版本正确
3. 查看生成代码的API兼容性
