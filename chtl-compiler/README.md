# CHTL编译器

基于C++17实现的CHTL（超文本语言）编译器，用于将CHTL源代码编译为标准HTML。

## 架构概览

```
┌─────────────────────────────────────────────────────────────────┐
│                         CHTL源代码                               │
└─────────────────────────────┬───────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                    CHTLUnifiedScanner                           │
│                   (精准代码切割器)                               │
└─────────────────────────────┬───────────────────────────────────┘
                              │
                              ▼
        ┌──────────────┬──────────────┬──────────────┬────────────┐
        │   CHTL片段   │ CHTL JS片段  │   CSS片段    │   JS片段   │
        └──────┬───────┴──────┬───────┴──────┬───────┴──────┬─────┘
               │              │              │              │
               ▼              ▼              ▼              ▼
┌─────────────────────────────────────────────────────────────────┐
│                    CompilerDispatcher                           │
│                   (编译器调度器)                                 │
└──────┬──────────────┬──────────────┬──────────────┬────────────┘
       │              │              │              │
       ▼              ▼              ▼              ▼
┌─────────────┐ ┌─────────────┐ ┌─────────────┐ ┌─────────────┐
│    CHTL     │ │   CHTL JS   │ │     CSS     │ │JavaScript   │
│  Compiler   │ │  Compiler   │ │  Compiler   │ │  Compiler   │
│  (手写)     │ │  (手写)     │ │  (ANTLR)    │ │  (ANTLR)    │
└─────────────┘ └─────────────┘ └─────────────┘ └─────────────┘
       │              │              │              │
       └──────────────┴──────────────┴──────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                      编译结果合并                                │
│                     (HTML输出)                                   │
└─────────────────────────────────────────────────────────────────┘
```

## 编译器对应关系

- **局部style** → CHTL编译器
- **全局style** → CSS编译器  
- **script** → 由CHTL、CHTL JS、JS编译器共同管理

## 主要特性

- ✅ 支持CHTL完整语法
- ✅ 局部样式块自动处理
- ✅ 增强的JavaScript语法（CHTL JS）
- ✅ 自动类名生成
- ✅ 模板系统（@Style, @Element, @Var）
- ✅ 并行编译支持
- ✅ HTML输出格式化/压缩

## 构建要求

- C++17兼容的编译器（GCC 7+, Clang 5+, MSVC 2017+）
- CMake 3.16+
- POSIX线程库

## 快速开始

### 构建编译器

```bash
# 克隆仓库后
cd chtl-compiler

# 使用构建脚本
./build.sh

# 或手动构建
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### 使用编译器

```bash
# 基本用法
./chtl-compiler input.chtl

# 指定输出文件
./chtl-compiler -o output.html input.chtl

# 压缩输出
./chtl-compiler -m input.chtl

# 查看帮助
./chtl-compiler -h
```

## 示例

创建一个 `hello.chtl` 文件：

```chtl
html {
    head {
        title: "Hello CHTL";
    }
    
    body {
        div {
            style {
                .container {
                    max-width: 800px;
                    margin: 0 auto;
                }
            }
            
            h1 {
                text { "欢迎使用CHTL！" }
            }
        }
    }
}
```

编译：

```bash
./chtl-compiler hello.chtl
```

## 项目结构

```
chtl-compiler/
├── include/          # 头文件
│   ├── scanner/      # 扫描器
│   ├── dispatcher/   # 调度器
│   ├── compilers/    # 各个编译器
│   └── merger/       # 合并器
├── src/              # 源代码
│   ├── scanner/      # 扫描器实现
│   ├── dispatcher/   # 调度器实现
│   ├── compilers/    # 编译器实现
│   │   ├── chtl/     # CHTL编译器
│   │   ├── chtl_js/  # CHTL JS编译器
│   │   ├── css/      # CSS编译器
│   │   └── javascript/# JavaScript编译器
│   └── merger/       # 合并器实现
├── examples/         # 示例文件
├── tests/            # 测试文件
└── build/            # 构建输出目录
```

## 开发状态

当前版本为1.0.0，实现了基础的CHTL编译功能。CSS和JavaScript编译器目前使用简化版本，后续将集成完整的ANTLR语法分析器。

## 许可证

MIT License