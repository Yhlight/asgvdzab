# CHTL 编译器

基于C++17实现的CHTL（Compact HTML Template Language）编译器，按照指定架构设计。

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

## 当前实现状态

### 已完成组件
- ✅ **CHTLUnifiedScanner** - 精准代码切割器
  - 支持识别CHTL、局部style、局部script、[Origin]块等
  - 正确处理注释和字符串
  - 支持错误恢复

- ✅ **CompilerDispatcher** - 编译器调度器
  - 支持并行编译
  - 编译缓存机制
  - 统计信息收集
  - 动态编译器注册

- ✅ **CHTLCompiler** - CHTL手写编译器
  - 完整的词法分析器
  - 递归下降语法分析器
  - HTML代码生成
  - 局部样式处理（自动生成类名）
  - 支持无引号字面量

- ✅ **主程序入口**
  - 命令行接口
  - 文件读写
  - 智能HTML结构合并

### 待实现组件
- ⏳ **CHTLJSCompiler** - CHTL JS手写编译器
  - 增强选择器 {{}}
  - 虚对象(vir)语法
  - 事件绑定语法糖

- ⏳ **CSSCompiler** - 基于ANTLR的CSS编译器
- ⏳ **JavaScriptCompiler** - 基于ANTLR的JavaScript编译器
- ⏳ **ResultMerger** - 结果合并器

## 编译和使用

### 环境要求
- C++17 兼容的编译器
- CMake 3.16+
- (可选) ANTLR4 运行时库

### 编译步骤
```bash
cd /workspace/chtl-compiler
mkdir build && cd build
cmake ..
make -j4
```

### 使用方法
```bash
./chtlc input.chtl [output.html]
```

如果不指定输出文件，将自动生成同名的.html文件。

### 示例

#### 简单示例 (simple.chtl)
```chtl
div {
    text {
        Hello World
    }
}
```

#### 复杂示例 (test.chtl)
```chtl
html {
    head {
        // 这是注释，不会出现在HTML中
    }
    
    body {
        div {
            id: main-container;
            class: container;
            
            style {
                width: 100%;
                max-width: 1200px;
                margin: 0 auto;
                padding: 20px;
            }
            
            h1 {
                text {
                    Welcome to CHTL
                }
            }
        }
    }
}
```

## 特性支持

### 已支持的CHTL特性
- HTML元素和嵌套结构
- 属性（支持无引号字面量）
- text块
- 局部style块（自动生成类名）
- 单行注释 (//) 和多行注释 (/**/)
- CE对等式（: 和 = 等价）

### 待支持的特性
- 模板 ([Template])
- 自定义 ([Custom])
- 导入 ([Import])
- 原始嵌入 ([Origin])
- 命名空间
- 局部script块
- CHTL JS增强语法

## 项目结构
```
chtl-compiler/
├── include/          # 头文件
│   ├── scanner/      # 扫描器
│   ├── compiler/     # 各种编译器
│   ├── dispatcher/   # 调度器
│   ├── merger/       # 合并器
│   └── utils/        # 工具类
├── src/              # 源文件
│   ├── scanner/
│   ├── compiler/
│   │   ├── chtl/
│   │   ├── chtljs/
│   │   ├── css/
│   │   └── javascript/
│   ├── dispatcher/
│   ├── merger/
│   └── main.cpp
├── tests/            # 测试文件
├── examples/         # 示例文件
├── build/            # 构建目录
└── CMakeLists.txt    # CMake配置
```

## 开发说明

每个编译器都实现了`ICompiler`接口，提供统一的编译方法。编译器之间相互独立，通过`CompilerDispatcher`协调工作。

局部style块由CHTL编译器处理，生成的样式会被添加到全局样式中。全局style块将由CSS编译器处理（待实现）。

script块的内容将由CHTL JS编译器、JavaScript编译器共同管理（待实现）。

## 许可证
MIT