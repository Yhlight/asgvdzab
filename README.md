# CHTL编译器

基于C++17实现的CHTL（超文本模板语言）编译器，提供更符合开发者习惯的HTML编写方式。

## 📋 项目概述

CHTL编译器采用模块化架构设计，包含以下核心组件：

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

## ✅ 已实现功能

### 核心组件
- ✅ **CHTLUnifiedScanner** - 精准代码切割器，识别和分割不同类型的代码片段
- ✅ **CompilerDispatcher** - 编译器调度器，管理编译流程和依赖关系
- ✅ **CHTLCompiler** - 手写CHTL编译器，处理核心语法和局部样式
- ✅ **主程序入口** - 完整的命令行接口和文件处理
- ✅ **测试套件** - 验证各组件功能的完整测试

### CHTL语法支持
- ✅ 基本HTML元素定义
- ✅ 元素属性设置（CE对等式：`:`和`=`等价）
- ✅ 嵌套元素结构
- ✅ 文本节点（`text {}`）
- ✅ 局部样式块（`style {}`）
  - ✅ 内联样式自动处理
  - ✅ 类选择器和ID选择器支持
  - ✅ 自动生成类名
- ✅ 注释处理（`//`、`/* */`）
- ✅ 字符串字面量（双引号、单引号、无修饰）

## 🚧 待实现功能

- ⏳ **CHTL JS编译器** - 处理CHTL JS增强语法
- ⏳ **CSS编译器** - 基于ANTLR的全局CSS处理
- ⏳ **JavaScript编译器** - 基于ANTLR的标准JS处理
- ⏳ **高级编译结果合并** - 更智能的HTML、CSS、JS合并策略
- ⏳ **模板系统** - `[Template] @Style/Element/Var` 支持
- ⏳ **自定义系统** - `[Custom] @Style/Element/Var` 支持
- ⏳ **导入系统** - `[Import]` 模块导入功能
- ⏳ **命名空间** - `[Namespace]` 支持
- ⏳ **配置系统** - `[Configuration]` 编译配置

## 🔧 编译和使用

### 系统要求
- C++17 兼容编译器（GCC 7+, Clang 5+, MSVC 2017+）
- CMake 3.16+

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
./build/bin/chtl_compiler example.chtl

# 指定输出文件
./build/bin/chtl_compiler -o output.html example.chtl

# 启用调试模式
./build/bin/chtl_compiler -d -v example.chtl

# 显示统计信息
./build/bin/chtl_compiler --stats example.chtl

# 查看帮助
./build/bin/chtl_compiler --help
```

### 运行测试
```bash
./build/bin/chtl_tests
```

## 📝 CHTL语法示例

### 基本元素
```chtl
div
{
    id: container;
    class: main-wrapper;
    
    text
    {
        "Hello, CHTL!"
    }
}
```

### 嵌套结构
```chtl
html
{
    head
    {
        text { "页面标题" }
    }
    
    body
    {
        div
        {
            class: header;
            
            h1
            {
                text { "欢迎" }
            }
            
            p
            {
                text { "这是一个CHTL示例" }
            }
        }
    }
}
```

### 局部样式
```chtl
div
{
    style
    {
        width: 300px;
        height: 200px;
        background-color: #f0f0f0;
        
        .title
        {
            font-size: 18px;
            color: #333;
        }
    }
    
    div
    {
        class: title;
        text { "标题文本" }
    }
}
```

## 🏗️ 项目结构

```
├── include/          # 头文件目录
│   ├── common/       # 通用类型定义
│   ├── scanner/      # 扫描器接口
│   ├── dispatcher/   # 调度器接口
│   └── compilers/    # 编译器接口
├── src/             # 源代码目录
│   ├── common/      # 通用实现
│   ├── scanner/     # 扫描器实现
│   ├── dispatcher/  # 调度器实现
│   ├── compilers/   # 编译器实现
│   └── main.cpp     # 主程序入口
├── tests/           # 测试用例
├── examples/        # 示例文件
└── CMakeLists.txt   # 构建配置
```

## 📄 许可证

MIT开源协议

## 🤝 贡献

欢迎提交Issue和Pull Request来完善CHTL编译器！

## 📊 开发状态

- **当前版本**: v1.0.0-alpha
- **完成度**: 核心功能已实现，高级功能开发中
- **稳定性**: Alpha阶段，适合测试和开发使用