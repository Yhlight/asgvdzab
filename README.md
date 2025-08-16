# CHTL - C++ HyperText Language

CHTL是一个基于C++17实现的超文本语言编译器，旨在提供一种更符合开发者习惯的HTML编写方式。

## 特性

- 🚀 **现代C++17实现** - 使用最新的C++标准开发
- 📝 **直观的语法** - 类似CSS的语法，无需繁琐的引号
- 🎨 **局部样式块** - 在元素内部直接编写样式
- 🧩 **模块化设计** - 支持CMOD和CJMOD模块系统
- ⚡ **高性能** - 精准的代码切割和编译

## 项目状态

**当前版本**: v1.0.0 (开发中)

本项目正在从Java版本迁移到C++17实现，以获得更好的性能和更低的资源占用。

### 已完成功能

- ✅ 基础项目结构
- ✅ CHTLUnifiedScanner（精准代码切割器）
- ✅ 编译器调度器框架
- ✅ 命令行界面

### 开发中功能

- 🚧 Token系统和词法分析器
- 🚧 AST节点定义
- 🚧 解析器和生成器
- 🚧 状态机和上下文管理
- 🚧 Import系统
- 🚧 CMOD/CJMOD模块系统

## 构建项目

### 依赖要求

- C++17兼容的编译器（GCC 7+, Clang 5+, MSVC 2017+）
- Make工具

### 编译步骤

```bash
# 克隆项目
git clone https://github.com/yourusername/chtl.git
cd chtl

# 编译项目
make

# 运行测试
./chtl --help
```

## 使用方法

```bash
# 基本用法
./chtl input.chtl

# 指定输出目录
./chtl -o output_dir input.chtl

# 启用调试模式
./chtl --debug input.chtl

# 查看版本
./chtl --version
```

## CHTL语法示例

```chtl
html {
    head {
        title {
            text { "Welcome to CHTL" }
        }
    }
    
    body {
        div {
            id: main-container;
            
            style {
                .container {
                    width: 100%;
                    max-width: 1200px;
                    margin: 0 auto;
                }
                
                &:hover {
                    background-color: #f0f0f0;
                }
            }
            
            text { "Hello, CHTL!" }
        }
    }
}
```

## 项目结构

```
chtl/
├── src/                    # 源代码目录
│   ├── scanner/           # 扫描器实现
│   ├── chtl/             # CHTL编译器
│   ├── chtljs/           # CHTL JS编译器
│   ├── css/              # CSS编译器
│   ├── javascript/       # JavaScript编译器
│   ├── dispatcher/       # 编译器调度器
│   ├── common/           # 公共组件
│   ├── utils/            # 工具类
│   ├── cmod/             # CMOD模块系统
│   ├── cjmod/            # CJMOD模块系统
│   └── module/           # 官方模块
├── test/                  # 测试目录
├── docs/                  # 文档目录
└── README.md             # 本文件
```

## 贡献指南

我们欢迎所有形式的贡献！请查看 [CONTRIBUTING.md](CONTRIBUTING.md) 了解如何参与项目开发。

## 许可证

本项目采用 MIT 许可证。详见 [LICENSE](LICENSE) 文件。

## 联系方式

- 项目主页: [https://github.com/yourusername/chtl](https://github.com/yourusername/chtl)
- 问题反馈: [Issues](https://github.com/yourusername/chtl/issues)
- 讨论区: [Discussions](https://github.com/yourusername/chtl/discussions)