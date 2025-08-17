# CHTL (C++ HyperText Language) 编译器

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![CMake](https://img.shields.io/badge/CMake-3.15+-green.svg)](https://cmake.org/)

CHTL是一种现代的超文本语言，提供更符合开发者编写习惯的HTML生成方式。本项目实现了完整的CHTL编译器，支持编译CHTL代码到HTML、CSS和JavaScript。

## 🚀 特性

### CHTL编译器
- **现代语法**：类似CSS的嵌套语法，支持无引号字面量
- **模板系统**：`[Template] @Style`、`@Element`、`@Var`支持
- **自定义组件**：`[Custom]`系统，支持参数化和特例化
- **命名空间**：`[Namespace]`系统，支持模块化开发
- **导入系统**：灵活的`[Import]`语法，支持文件和模块导入
- **样式增强**：局部样式块、自动类名/ID、上下文推导
- **约束系统**：`except`关键字，支持精确的语法约束

### CHTL JS编译器
- **增强选择器**：`{{选择器}}`语法
- **链式操作**：`->`操作符支持
- **事件系统**：`listen`、`delegate`语法
- **动画支持**：`animate`语法
- **模块扩展**：CJMOD系统，支持C++扩展

## 📁 项目结构

```
CHTL-Compiler/
├── Include/              # 统一头文件目录
│   ├── CHTL/            # CHTL编译器头文件
│   └── CHTLJS/          # CHTL JS编译器头文件
│
├── CHTL/                # CHTL编译器实现
│   ├── Common/          # 通用组件
│   ├── Lexer/           # 词法分析器
│   ├── Parser/          # 语法分析器
│   ├── Generator/       # 代码生成器
│   └── Loader/          # 模块加载器
│
├── CHTLJS/              # CHTL JS编译器实现
│   ├── Common/          # 通用组件
│   ├── Lexer/           # 词法分析器
│   ├── Parser/          # 语法分析器
│   ├── Generator/       # 代码生成器
│   └── Loader/          # 模块加载器
│
├── Tests/               # 测试代码
├── Examples/            # 示例代码
├── Documentation/       # 项目文档
├── Tools/              # 开发工具
└── ThirdParty/         # 第三方依赖
```

## 🛠️ 构建要求

- **C++17** 或更高版本
- **CMake 3.15** 或更高版本
- **支持的编译器**：
  - GCC 7.0+
  - Clang 6.0+
  - MSVC 2017+

## 📦 快速开始

### 1. 克隆仓库
```bash
git clone https://github.com/your-org/chtl-compiler.git
cd chtl-compiler
```

### 2. 构建项目
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 3. 运行测试
```bash
ctest --verbose
```

### 4. 编译CHTL文件
```bash
./chtlc input.chtl -o output.html
```

## 📖 语法示例

### 基础HTML结构
```chtl
html {
    head {
        title { "CHTL示例" }
    }
    
    body {
        div {
            id: container;
            class: main-content;
            
            text { "Hello, CHTL!" }
            
            style {
                width: 100%;
                .highlighted {
                    background: yellow;
                }
                
                &:hover {
                    opacity: 0.8;
                }
            }
        }
    }
}
```

### 模板系统
```chtl
[Template] @Style ButtonStyle {
    background: blue;
    color: white;
    padding: 10px 20px;
    border-radius: 4px;
}

[Template] @Element Button {
    button {
        @Style ButtonStyle;
        text { "点击我" }
    }
}

body {
    @Element Button;
}
```

### 命名空间和导入
```chtl
[Namespace] ui.components {
    [Import] @Style from "theme.chtl" as theme;
    
    [Template] @Element Card {
        div {
            class: card;
            style {
                @Style theme.cardStyle;
            }
        }
    }
}
```

## 🔧 开发指南

### 添加新功能
1. 在相应的模块目录中添加头文件和源文件
2. 更新CMakeLists.txt
3. 添加相应的测试
4. 更新文档

### 测试
```bash
# 运行所有测试
cd build && ctest

# 运行特定测试
./Tests/CHTL/test_lexer
./Tests/CHTL/test_parser
```

### 调试
```bash
# 启用调试模式构建
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

## 📚 文档

- [语言参考](Documentation/LanguageReference/) - CHTL语法完整参考
- [编译器指南](Documentation/CompilerGuide/) - 编译器架构和实现
- [API参考](Documentation/APIReference/) - C++ API文档

## 🤝 贡献

欢迎贡献代码！请遵循以下步骤：

1. Fork本项目
2. 创建功能分支 (`git checkout -b feature/amazing-feature`)
3. 提交更改 (`git commit -m 'Add amazing feature'`)
4. 推送到分支 (`git push origin feature/amazing-feature`)
5. 创建Pull Request

## 📄 许可证

本项目采用MIT许可证 - 查看 [LICENSE.txt](LICENSE.txt) 文件了解详情。

## 🙏 致谢

- ANTLR项目 - 用于CSS和JavaScript解析
- 所有贡献者和测试者

## 📞 联系

- 项目主页：https://github.com/your-org/chtl-compiler
- 问题反馈：https://github.com/your-org/chtl-compiler/issues
- 文档：https://chtl-compiler.readthedocs.io/

---

**CHTL编译器** - 让HTML编写更加直观和高效！
