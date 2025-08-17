# CHTL编译器项目

基于架构图创建的CHTL（Compact HTML Template Language）编译器。该编译器将CHTL源代码转换为标准HTML输出。

## 项目架构

```
CHTL源代码
    ↓
CHTLUnifiedScanner（精准代码切割器）
    ↓
编译器调度器（CompilerDispatcher）
    ↓
┌─────────────┬──────────────┬─────────────┬──────────────────┐
│CHTL编译器   │CHTL JS编译器 │CSS编译器    │JavaScript编译器  │
│(手写)       │(手写)        │(简化版)     │(简化版)         │
└─────────────┴──────────────┴─────────────┴──────────────────┘
    ↓
编译结果合并器（ResultMerger）
    ↓
HTML输出
```

## 实现状态

### ✅ 已完成组件

1. **CHTLUnifiedScanner（精准代码切割器）**
   - 将CHTL源代码切割成不同类型的代码片段
   - 支持CHTL、CHTL JS、CSS、JavaScript、原始HTML片段
   - 智能识别局部style和script块

2. **CompilerDispatcher（编译器调度器）**
   - 并行编译支持
   - 编译结果缓存
   - 自动选择合适的编译器
   - 统计信息收集

3. **CHTL编译器（手写）**
   - 完整的词法分析器
   - 递归下降语法分析器
   - HTML代码生成器
   - 支持特性：
     - 元素节点和属性
     - 文本块
     - 局部样式（自动生成类名）
     - 无引号字面量
     - CE等价（冒号等号互换）

4. **CHTL JS编译器**
   - 支持增强选择器 `{{selector}}`
   - 箭头操作符 `->`
   - 特殊函数：`listen`、`delegate`、`animate`
   - 虚对象 `vir` 声明

5. **CSS编译器（简化版）**
   - 基础CSS处理
   - 注释移除
   - 格式化/最小化
   - 简单优化

6. **JavaScript编译器（简化版）**
   - 基础JavaScript处理
   - 注释移除
   - 格式化/最小化
   - 保留字符串内容

7. **ResultMerger（结果合并器）**
   - 智能合并各编译器输出
   - 自动检测HTML结构
   - 样式和脚本插入
   - HTML美化输出

## 构建和使用

### 构建项目

```bash
mkdir build
cd build
cmake ..
make -j4
```

### 使用编译器

```bash
./chtlc input.chtl [output.html]
```

如果不指定输出文件，将生成与输入文件同名的.html文件。

## 示例

### 输入（test.chtl）
```chtl
html {
    body {
        div {
            class: container;
            id: main-container;
            
            h1 { text { Welcome to CHTL } }
            
            div {
                class: box;
                style {
                    .box {
                        background-color: #f0f0f0;
                        padding: 15px;
                    }
                }
                text { This is a styled box }
            }
        }
    }
}
```

### 输出（test.html）
```html
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CHTL Output</title>
    <style>
    .box {
        background-color: #f0f0f0;
        padding: 15px;
    }
    </style>
</head>
<body>
    <div class="container" id="main-container">
        <h1>Welcome to CHTL</h1>
        <div class="box">This is a styled box</div>
    </div>
</body>
</html>
```

## 支持的CHTL特性

- ✅ 元素节点
- ✅ 属性（CE等价）
- ✅ 文本块
- ✅ 局部样式
- ✅ 无引号字面量
- ✅ 注释
- ✅ 增强选择器（CHTL JS）
- ✅ 箭头操作符（CHTL JS）
- ⏳ 模板系统
- ⏳ 自定义定义
- ⏳ 配置组
- ⏳ 导入系统
- ⏳ 命名空间

## 待优化事项

1. **完整ANTLR集成**
   - 实现更强大的CSS/JavaScript解析
   - 支持更复杂的语法转换

2. **CHTL JS优化**
   - 实现真正的预处理器模式
   - 只转换CHTL JS特有语法

3. **高级特性支持**
   - 模板系统
   - 自定义定义
   - CMOD/CJMOD模块系统

## 项目结构

```
chtl-compiler/
├── include/              # 头文件
│   ├── scanner/         # 扫描器
│   ├── compiler/        # 各编译器接口
│   ├── dispatcher/      # 调度器
│   ├── merger/          # 合并器
│   └── utils/           # 工具类
├── src/                 # 源代码
│   ├── scanner/         # 扫描器实现
│   ├── compiler/        # 各编译器实现
│   ├── dispatcher/      # 调度器实现
│   ├── merger/          # 合并器实现
│   └── main.cpp         # 主程序
├── tests/               # 测试代码
├── examples/            # 示例文件
├── grammar/             # ANTLR语法文件
└── build/               # 构建目录
```

## 许可证

本项目遵循MIT许可证。