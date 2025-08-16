# CHTL项目结构

```
CHTL/
├── src/                          # 源代码
│   ├── main/
│   │   ├── java/
│   │   │   └── com/chtl/
│   │   │       ├── ast/          # AST节点定义
│   │   │       ├── cli/          # 命令行工具
│   │   │       ├── cjmod/        # CJMOD系统
│   │   │       ├── cmod/         # CMOD系统
│   │   │       ├── compiler/     # 编译器核心
│   │   │       ├── context/      # 上下文管理
│   │   │       ├── error/        # 错误处理
│   │   │       ├── generator/    # 代码生成器
│   │   │       ├── model/        # 数据模型
│   │   │       ├── module/       # 官方模块源代码
│   │   │       │   └── Chtholly/ # 珂朵莉模块
│   │   │       ├── parser/       # 解析器
│   │   │       ├── scanner/      # 扫描器
│   │   │       └── test/         # 测试支持
│   │   └── resources/            # 资源文件
│   └── test/                     # 测试代码
│       └── java/
│           └── com/chtl/test/
├── scripts/                      # 构建脚本
│   ├── linux/                    # Linux脚本
│   ├── windows/                  # Windows脚本
│   └── macos/                    # macOS脚本
├── vscode-chtl/                  # VSCode插件
│   ├── src/                      # 插件源码
│   ├── syntaxes/                 # 语法定义
│   └── snippets/                 # 代码片段
├── docs/                         # 文档
│   ├── api/                      # API文档
│   ├── examples/                 # 示例代码
│   └── guides/                   # 使用指南
├── .github/                      # GitHub配置
│   └── workflows/                # CI/CD配置
├── pom.xml                       # Maven配置
├── README.md                     # 项目说明
├── CHTL语法文档.md              # 语法文档
└── CONTRIBUTING.md               # 贡献指南
```

## 核心模块说明

### 1. AST（抽象语法树）
- 定义所有语法节点类型
- 支持访问者模式
- 可序列化（用于模块系统）

### 2. 编译器核心
- 词法分析器（Lexer）
- 语法分析器（Parser）
- 代码生成器（Generator）
- 优化器（Optimizer）

### 3. 模块系统
- CMOD：CHTL模板模块
- CJMOD：JavaScript扩展模块
- 模块加载、打包、签名、版本管理

### 4. 工具链
- CLI命令行工具
- VSCode插件
- 构建脚本（跨平台）

### 5. 官方模块
- Chtholly（珂朵莉）：主题模块示例