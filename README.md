# CHTL编译器

基于C++17实现的CHTL（超文本语言）编译器，支持完整的CHTL语法并生成标准HTML输出。

## 项目架构

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

## 项目结构

```
CHTL_Compiler/
├── CMakeLists.txt              # CMake构建配置
├── README.md                   # 项目说明
├── test.chtl                   # 测试CHTL文件
├── CHTL语法文档.md             # CHTL语法文档
├── include/                    # 头文件目录
│   ├── common/                 # 通用头文件
│   │   ├── Types.h            # 基础类型定义
│   │   ├── Token.h            # Token类定义
│   │   ├── AST.h              # AST节点定义
│   │   └── Utils.h            # 工具函数
│   ├── scanner/               # 扫描器
│   │   └── CHTLUnifiedScanner.h
│   ├── dispatcher/            # 调度器
│   │   └── CompilerDispatcher.h
│   ├── compilers/             # 编译器
│   │   ├── CHTLCompiler.h     # CHTL编译器
│   │   ├── CHTLJSCompiler.h   # CHTL JS编译器
│   │   ├── CSSCompiler.h      # CSS编译器
│   │   └── JavaScriptCompiler.h # JS编译器
│   └── merger/                # 合并器
│       └── ResultMerger.h
└── src/                       # 源文件目录
    ├── main.cpp               # 主程序
    ├── common/                # 通用源文件
    ├── scanner/               # 扫描器实现
    ├── dispatcher/            # 调度器实现
    ├── compilers/             # 编译器实现
    └── merger/                # 合并器实现
```

## 核心组件

### 1. CHTLUnifiedScanner (精准代码切割器)
- 负责词法分析和代码片段切割
- 支持识别CHTL、CHTL JS、CSS、JavaScript代码片段
- 处理注释、字符串、嵌套结构

### 2. CompilerDispatcher (编译器调度器)
- 管理四个编译器实例
- 根据代码片段类型分发到对应编译器
- 收集编译结果和错误信息

### 3. 编译器组件
- **CHTLCompiler**: 处理CHTL语法，生成HTML和CSS
- **CHTLJSCompiler**: 处理CHTL JS语法，生成JavaScript
- **CSSCompiler**: 处理CSS代码（可扩展ANTLR支持）
- **JavaScriptCompiler**: 处理纯JavaScript代码（可扩展ANTLR支持）

### 4. ResultMerger (结果合并器)
- 合并各编译器的输出
- 生成完整的HTML页面
- 支持代码优化和格式化

## 编译和运行

### 编译项目
```bash
mkdir build
cd build
cmake ..
make
```

### 运行编译器
```bash
# 基本用法
./chtl_compiler input.chtl

# 指定输出文件
./chtl_compiler -o output.html input.chtl

# 启用调试模式
./chtl_compiler -d input.chtl

# 运行内置测试
./chtl_compiler --test

# 输出详细信息
./chtl_compiler --dump-tokens --dump-fragments --dump-ast -d input.chtl
```

### 命令行选项
- `-o, --output <文件>`: 指定输出文件 (默认: output.html)
- `-d, --debug`: 启用调试模式
- `-m, --minify`: 压缩输出
- `-f, --format`: 格式化输出
- `-h, --help`: 显示帮助信息
- `--dump-tokens`: 输出Token信息
- `--dump-fragments`: 输出代码片段信息
- `--dump-ast`: 输出AST信息
- `--test`: 运行内置测试

## CHTL语法特性

### 支持的功能
- ✅ 元素节点和文本节点
- ✅ 属性定义（支持CE对等式，:和=等价）
- ✅ 局部样式块（style{}）
- ✅ 局部脚本块（script{}）
- ✅ 自动化类名/ID生成
- ✅ 上下文推导（&选择器）
- ✅ 模板系统（@Style、@Element、@Var）
- ✅ 自定义系统（[Custom]）
- ✅ 原始嵌入（[Origin]）
- ✅ 导入系统（[Import]）
- ✅ 命名空间（[Namespace]）
- ✅ 配置组（[Configuration]）
- ✅ CHTL JS增强选择器（{{selector}}）
- ✅ CHTL JS增强监听器（listen）
- ✅ CHTL JS事件委托（delegate）
- ✅ CHTL JS虚对象（vir）
- ✅ CHTL JS动画系统（animate）

### 注释支持
- `//` 单行注释（不会出现在生成的HTML中）
- `/* */` 多行注释（不会出现在生成的HTML中）
- `--` 生成器注释（会被生成器识别）

## 测试

项目包含完整的测试用例，展示了CHTL的各种语法特性：

```bash
# 运行内置测试
./chtl_compiler --test

# 编译测试文件
./chtl_compiler test.chtl
```

测试文件包含：
- 模板定义和使用
- 自定义变量组
- 局部样式块和自动类名生成
- CHTL JS增强选择器
- 事件监听和委托
- 虚对象系统

## 扩展性

### 集成ANTLR
CSS编译器和JavaScript编译器预留了ANTLR集成接口，可以通过以下方式扩展：

1. 添加ANTLR依赖到CMakeLists.txt
2. 创建CSS和JavaScript的语法文件
3. 实现对应的Visitor或Listener
4. 替换当前的简化实现

### 添加新的编译器
1. 继承基础编译器接口
2. 在CompilerDispatcher中注册新编译器
3. 在CHTLUnifiedScanner中添加新的代码片段识别逻辑

## 开发状态

当前版本实现了完整的CHTL编译器架构，所有核心功能都已实现并可以正常编译运行。每个编译器都有独立的文件集合，符合项目要求。

## 许可证

MIT License