# CHTL编译器 - 基于C++17的超文本语言编译器

CHTL（C++ Hypertext Template Language）是一个基于C++17实现的超文本语言编译器，提供了更符合开发者编写HTML代码的方式。

## 项目架构

```
┌─────────────────────────────────────────────────────────────────┐
│                         CHTL源代码                               │
└─────────────────────────────┬───────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                    ChtlUnifiedScanner                           │
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

## 项目结构

```
ChtlCompiler/
├── CMakeLists.txt              # 根CMake配置
├── README.md                   # 项目说明
├── CHTL语法文档.md            # CHTL语法文档
├── Include/                    # 头文件目录
│   ├── Common/                 # 公共头文件
│   │   ├── FragmentTypes.h     # 代码片段类型定义
│   │   └── CompilerInterface.h # 编译器接口定义
│   ├── Scanner/                # 扫描器头文件
│   │   ├── UnifiedScanner.h    # 统一扫描器
│   │   └── Token.h             # Token定义
│   ├── Dispatcher/             # 调度器头文件
│   │   └── CompilerDispatcher.h # 编译器调度器
│   ├── Compilers/              # 编译器头文件
│   │   ├── Chtl/              # CHTL编译器
│   │   ├── ChtlJs/            # CHTL JS编译器
│   │   ├── Css/               # CSS编译器
│   │   └── Js/                # JavaScript编译器
│   ├── Merger/                 # 结果合并器头文件
│   │   └── ResultMerger.h      # 结果合并器
│   └── Utils/                  # 工具类头文件
├── Src/                        # 源文件目录
│   ├── CMakeLists.txt          # 源文件CMake配置
│   ├── Main.cpp                # 主程序入口
│   ├── Common/                 # 公共实现
│   ├── Scanner/                # 扫描器实现
│   ├── Dispatcher/             # 调度器实现
│   ├── Compilers/              # 编译器实现
│   ├── Merger/                 # 结果合并器实现
│   └── Utils/                  # 工具类实现
├── Examples/                   # 示例文件
│   └── Example.chtl            # CHTL语法示例
└── antlr-prebuilt/             # ANTLR预编译文件
```

## 核心组件

### 1. ChtlUnifiedScanner (精准代码切割器)
负责将CHTL源代码精确切割成不同类型的代码片段：
- CHTL片段
- CHTL JS片段
- CSS片段
- JavaScript片段

### 2. CompilerDispatcher (编译器调度器)
负责将不同类型的代码片段分发给对应的编译器进行编译：
- 支持并行编译
- 支持依赖解析
- 支持多种调度策略

### 3. 编译器系列
- **CHTL编译器** (手写): 处理CHTL语法，包括元素节点、属性、模板等
- **CHTL JS编译器** (手写): 处理CHTL JS语法，包括增强选择器、虚对象等
- **CSS编译器** (基于ANTLR): 处理标准CSS语法
- **JavaScript编译器** (基于ANTLR): 处理标准JavaScript语法

### 4. ResultMerger (结果合并器)
将各编译器的输出结果合并成最终的HTML文件。

## 编译要求

- C++17 或更高版本
- CMake 3.16 或更高版本
- ANTLR4运行时库

## 编译步骤

```bash
# 1. 创建构建目录
mkdir Build && cd Build

# 2. 配置项目
cmake ..

# 3. 编译项目
make -j4

# 4. 运行编译器
./Bin/ChtlCompiler ../Examples/Example.chtl -o Output.html
```

## 使用方法

### 基本用法
```bash
ChtlCompiler Input.chtl -o Output.html
```

### 命令行选项
- `-o, --output <文件>`: 指定输出文件 (默认: output.html)
- `-d, --debug`: 启用调试模式
- `-p, --parallel`: 启用并行编译
- `-m, --minify`: 压缩输出
- `-t, --threads <数量>`: 指定线程数 (默认: 4)
- `-h, --help`: 显示帮助信息

### 示例
```bash
# 基本编译
ChtlCompiler Example.chtl

# 并行编译并压缩输出
ChtlCompiler Example.chtl -p -m -o Min.html

# 调试模式
ChtlCompiler Example.chtl --debug
```

## 特性

### CHTL语法特性
- 元素节点和属性
- 局部样式块和脚本块
- 模板系统 (样式组模板、元素模板、变量组模板)
- 自定义系统
- 原始嵌入
- 命名空间
- 导入系统
- 配置组

### CHTL JS语法特性
- 增强选择器 `{{selector}}`
- 虚对象 `vir`
- 增强监听器 `listen`
- 事件委托 `delegate`
- 动画系统 `animate`

### 编译器特性
- 精准代码切割
- 并行编译支持
- 多种调度策略
- 结果合并优化
- 错误处理和报告
- 性能统计

## 命名规范

项目采用大驼峰命名法 (PascalCase)：
- **文件夹**: `Include/`, `Src/`, `Common/`, `Scanner/` 等
- **头文件**: `FragmentTypes.h`, `CompilerInterface.h`, `UnifiedScanner.h` 等
- **源文件**: `Main.cpp`, `FragmentTypes.cpp`, `CompilerDispatcher.cpp` 等
- **类名**: `ChtlUnifiedScanner`, `CompilerDispatcher`, `FragmentContainer` 等
- **方法名**: `GetType()`, `SetContent()`, `CreateScanner()` 等
- **变量名**: `Type_`, `Content_`, `Location_` 等 (成员变量使用下划线后缀)
- **命名空间**: `Chtl`

## 开发状态

当前项目处于基础架构建立阶段，已完成：

✅ 项目基础架构设计  
✅ 核心接口定义  
✅ CMake构建系统配置  
✅ 主程序框架  
✅ 大驼峰命名法规范化

待完成：
- [ ] 统一扫描器实现
- [ ] 编译器调度器实现
- [ ] CHTL编译器实现
- [ ] CHTL JS编译器实现
- [ ] CSS/JS编译器实现
- [ ] 结果合并器实现
- [ ] 单元测试
- [ ] 性能优化

## 许可证

MIT License

## 贡献

欢迎提交Issue和Pull Request来改进项目。

## 联系方式

CHTL Team - chtl@example.com