# ANTLR4 预编译包变更日志

## 版本 4.13.1 (2025-08-16)

### 新功能
- ✨ 提供完整的ANTLR4 4.13.1预编译包
- ✨ 包含C++运行时库（静态库和动态库）
- ✨ 包含ANTLR4工具JAR文件
- ✨ 提供跨平台启动脚本（Linux/Windows）
- ✨ 包含完整的头文件和CMake配置

### 技术规格
- **ANTLR版本**: 4.13.1
- **编译平台**: Linux x86_64 (Ubuntu)
- **编译器**: Clang 20.1.2
- **C++标准**: C++17
- **Java兼容性**: Java 8+
- **CMake兼容性**: CMake 3.10+

### 文件结构
```
antlr-prebuilt/
├── bin/                    # 工具可执行文件
│   ├── antlr4             # Linux启动脚本
│   ├── antlr4.bat         # Windows启动脚本
│   └── antlr4.jar         # ANTLR4工具JAR文件
├── lib/                    # 运行时库
│   ├── libantlr4-runtime.a         # 静态库
│   ├── libantlr4-runtime.so        # 动态库符号链接
│   └── libantlr4-runtime.so.4.13.1 # 动态库实际文件
├── include/                # 头文件
│   └── antlr4-runtime/     # C++运行时头文件
├── doc/                    # 文档
├── ANTLR4Config.cmake      # CMake配置文件
├── README.md               # 使用说明
├── VERSION                 # 版本信息
└── CHANGELOG.md            # 本文件
```

### 集成说明
此预编译包专为CHTL编译器项目优化，支持：
- CSS语法解析器生成
- JavaScript语法解析器生成
- 与C++17项目的无缝集成
- CMake构建系统支持

### 使用方法
1. 确保已安装Java 8+
2. 将`antlr-prebuilt/bin`添加到PATH环境变量
3. 设置`LD_LIBRARY_PATH`包含`antlr-prebuilt/lib`目录
4. 在CMake中使用`find_package(ANTLR4)`

### 验证
预编译包已通过以下测试：
- ✅ CHTL编译器集成测试
- ✅ CSS语法解析器生成测试
- ✅ JavaScript语法解析器生成测试
- ✅ 跨平台兼容性测试
- ✅ 性能基准测试

### 许可证
ANTLR4遵循BSD 3-Clause许可证
预编译包构建脚本遵循MIT许可证