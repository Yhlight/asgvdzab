# CHTL编译器

CHTL（C++ HyperText Language）是基于C++语言实现的超文本语言，旨在提供更符合开发者习惯的HTML编写方式。

## 项目结构

```
ChtlCompiler/
├── CMakeLists.txt       # CMake配置文件
├── Include/             # 头文件目录
│   ├── AST/            # AST节点定义
│   ├── Common/         # 公共组件（状态管理、上下文等）
│   ├── Generator/      # 代码生成器
│   ├── Lexer/          # 词法分析器
│   └── Parser/         # 语法分析器
├── Src/                # 源代码目录
│   ├── AST/            # AST节点实现
│   ├── Common/         # 公共组件实现
│   ├── Generator/      # 代码生成器实现
│   ├── Lexer/          # 词法分析器实现
│   ├── Parser/         # 语法分析器实现
│   └── Main.cpp        # 主程序入口
├── Tests/              # 测试代码
├── Examples/           # 示例代码
└── Build/              # 构建输出目录（自动生成）
```

## 编译方法

### 依赖要求
- C++17或更高版本的编译器
- CMake 3.10或更高版本

### 编译步骤

```bash
cd ChtlCompiler
mkdir -p Build
cd Build
cmake ..
make -j4
```

### 运行测试

```bash
cd Build
ctest
# 或直接运行
./Bin/TestLexer
```

## 使用方法

```bash
./Bin/Chtlc input.chtl
```

## CHTL语法特性

### 基本语法
- 文本节点：`text { "内容" }`
- 元素节点：`div { id: main; class: container; }`
- 属性支持CE对等式：`:` 和 `=` 完全等价
- 支持三种字符串：双引号、单引号、无引号字面量

### 局部样式块
```chtl
div {
    style {
        .box {           // 自动添加类名
            width: 300px;
        }
        &:hover {        // 上下文推导
            color: red;
        }
    }
}
```

### 模板系统
```chtl
[Template] @Style DefaultText {
    color: black;
    line-height: 1.6;
}

div {
    style {
        @Style DefaultText;  // 使用模板
    }
}
```

### CHTL JS扩展
```chtl
script {
    {{box}}->listen({
        click: () => {
            {{button[0]}}->textContent = "Clicked!";
        }
    });
}
```

## 架构特点

1. **模块化设计**：词法分析、语法分析、AST、代码生成完全解耦
2. **RAII状态管理**：使用RAII模式确保状态一致性和异常安全
3. **严格遵循语法文档**：所有实现都基于CHTL语法文档，无私自扩展
4. **完整的错误处理**：详细的错误报告和恢复机制

## 开源协议

MIT License