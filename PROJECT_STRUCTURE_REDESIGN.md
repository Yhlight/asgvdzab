# CHTL编译器项目结构重新设计

## 🎯 设计原则

您的建议非常正确！我已经重新设计了整个项目结构，采用：

1. **大驼峰命名法**：所有文件名使用PascalCase
2. **模块化分离**：CHTL和CHTL JS各自独立的完整模块
3. **清晰的功能划分**：每个编译器都有完整的Common、Lexer、Parser、Generator、Loader结构

## 📁 新的项目结构

```
CHTL-Compiler/
├── Include/                    # 统一头文件目录
│   ├── CHTL/                  # CHTL编译器头文件
│   │   ├── Common.hpp         # 公共定义和工具
│   │   ├── Lexer.hpp          # 词法分析器
│   │   ├── Parser.hpp         # 语法分析器
│   │   ├── ASTNodes.hpp       # AST节点定义
│   │   ├── ASTVisitor.hpp     # 访问者模式
│   │   ├── Generator.hpp      # 代码生成器
│   │   └── Loader.hpp         # 模块加载器
│   │
│   ├── CHTLJS/                # CHTL JS编译器头文件
│   │   ├── Common.hpp         # 公共定义和工具
│   │   ├── Lexer.hpp          # 词法分析器
│   │   ├── Parser.hpp         # 语法分析器
│   │   ├── ASTNodes.hpp       # AST节点定义
│   │   ├── ASTVisitor.hpp     # 访问者模式
│   │   ├── Generator.hpp      # 代码生成器
│   │   └── Loader.hpp         # 模块加载器
│   │
│   ├── Common/                # 跨编译器共用组件
│   │   ├── ErrorHandler.hpp   # 错误处理
│   │   ├── FileSystem.hpp     # 文件系统操作
│   │   ├── Logger.hpp         # 日志系统
│   │   └── Utilities.hpp      # 通用工具函数
│   │
│   └── Shared/                # 共享的第三方集成
│       ├── ANTLR.hpp          # ANTLR集成
│       ├── Packaging.hpp      # 打包系统（ZIP等）
│       └── ModuleSystem.hpp   # 模块系统（CMOD/CJMOD）
│
├── CHTL/                      # CHTL编译器实现
│   ├── Common/                # CHTL通用模块
│   │   ├── Common.cpp         # 基础数据结构实现
│   │   ├── StringUtils.cpp    # 字符串工具
│   │   ├── FileUtils.cpp      # 文件工具
│   │   ├── HtmlUtils.cpp      # HTML工具
│   │   ├── CssUtils.cpp       # CSS工具
│   │   └── DebugUtils.cpp     # 调试工具
│   │
│   ├── Lexer/                 # CHTL词法分析器
│   │   ├── Lexer.cpp          # 主词法分析器
│   │   ├── Token.cpp          # Token处理
│   │   ├── LexerFactory.cpp   # 词法分析器工厂
│   │   └── TokenUtils.cpp     # Token工具函数
│   │
│   ├── Parser/                # CHTL语法分析器
│   │   ├── Parser.cpp         # 主语法分析器
│   │   ├── ASTNodes.cpp       # AST节点实现
│   │   ├── ASTVisitor.cpp     # 访问者实现
│   │   ├── ParserFactory.cpp  # 语法分析器工厂
│   │   └── SemanticAnalyzer.cpp # 语义分析器
│   │
│   ├── Generator/             # CHTL代码生成器
│   │   ├── HtmlGenerator.cpp  # HTML代码生成
│   │   ├── CssGenerator.cpp   # CSS代码生成
│   │   ├── JsGenerator.cpp    # JavaScript代码生成
│   │   ├── SourceMapGenerator.cpp # 源码映射生成
│   │   └── CodeOptimizer.cpp  # 代码优化器
│   │
│   └── Loader/                # CHTL模块加载器
│       ├── ModuleLoader.cpp   # 模块加载器
│       ├── ImportResolver.cpp # 导入解析器
│       ├── NamespaceManager.cpp # 命名空间管理
│       ├── TemplateSystem.cpp # 模板系统
│       └── ConstraintSystem.cpp # 约束系统
│
├── CHTLJS/                    # CHTL JS编译器实现
│   ├── Common/                # CHTL JS通用模块
│   │   ├── Common.cpp         # 基础数据结构
│   │   ├── JSUtils.cpp        # JavaScript工具
│   │   ├── DOMUtils.cpp       # DOM操作工具
│   │   └── EventUtils.cpp     # 事件处理工具
│   │
│   ├── Lexer/                 # CHTL JS词法分析器
│   │   ├── JSLexer.cpp        # JavaScript词法分析器
│   │   ├── EnhancedSelector.cpp # 增强选择器词法
│   │   ├── JSToken.cpp        # JavaScript Token
│   │   └── SelectorToken.cpp  # 选择器Token
│   │
│   ├── Parser/                # CHTL JS语法分析器
│   │   ├── JSParser.cpp       # JavaScript语法分析器
│   │   ├── JSASTNodes.cpp     # JavaScript AST节点
│   │   ├── SelectorParser.cpp # 选择器解析器
│   │   └── ExpressionParser.cpp # 表达式解析器
│   │
│   ├── Generator/             # CHTL JS代码生成器
│   │   ├── JSCodeGenerator.cpp # JavaScript代码生成
│   │   ├── DOMGenerator.cpp   # DOM操作代码生成
│   │   ├── EventGenerator.cpp # 事件处理代码生成
│   │   └── AnimationGenerator.cpp # 动画代码生成
│   │
│   └── Loader/                # CHTL JS模块加载器
│       ├── JSModuleLoader.cpp # JavaScript模块加载
│       ├── CJMODLoader.cpp    # CJMOD加载器
│       ├── RuntimeLoader.cpp  # 运行时加载器
│       └── InterfaceSystem.cpp # 接口系统
│
├── Tests/                     # 测试目录
│   ├── CHTL/                  # CHTL测试
│   │   ├── LexerTests/
│   │   ├── ParserTests/
│   │   ├── GeneratorTests/
│   │   ├── LoaderTests/
│   │   └── IntegrationTests/
│   │
│   ├── CHTLJS/                # CHTL JS测试
│   │   ├── LexerTests/
│   │   ├── ParserTests/
│   │   ├── GeneratorTests/
│   │   ├── LoaderTests/
│   │   └── IntegrationTests/
│   │
│   └── Common/                # 通用测试
│       ├── ErrorHandlerTests/
│       ├── FileSystemTests/
│       └── UtilityTests/
│
├── Examples/                  # 示例代码
│   ├── BasicCHTL/            # 基础CHTL示例
│   ├── AdvancedCHTL/         # 高级CHTL示例
│   ├── TemplateSystem/       # 模板系统示例
│   ├── NamespaceSystem/      # 命名空间示例
│   └── IntegratedProjects/   # 完整项目示例
│
├── Tools/                     # 开发工具
│   ├── Compiler/             # 编译器主程序
│   ├── Formatter/            # 代码格式化工具
│   ├── Linter/               # 代码检查工具
│   ├── LanguageServer/       # 语言服务器（IDE支持）
│   └── PackageManager/       # 包管理器
│
├── Documentation/            # 文档
│   ├── LanguageReference/    # 语言参考
│   ├── CompilerGuide/        # 编译器指南
│   ├── APIReference/         # API参考
│   ├── TutorialGuide/        # 教程指南
│   └── ContributionGuide/    # 贡献指南
│
└── Build/                    # 构建相关
    ├── CMake/                # CMake脚本
    ├── Scripts/              # 构建脚本
    ├── Dependencies/         # 依赖管理
    └── Packaging/            # 打包配置
```

## 🔄 迁移计划

### 第一阶段：结构重组
1. ✅ 创建新的目录结构
2. ✅ 采用大驼峰命名法重命名所有文件
3. ✅ 创建 CHTL/Common.hpp 基础框架
4. ✅ 创建 CHTL/Lexer.hpp 词法分析器框架
5. ✅ 创建 CHTL/ASTNodes.hpp AST节点框架

### 第二阶段：CHTL编译器完整实现
1. 🔄 实现 CHTL/Common/ 模块所有组件
2. 🔄 实现 CHTL/Lexer/ 模块所有组件
3. 🔄 实现 CHTL/Parser/ 模块所有组件
4. ⏳ 实现 CHTL/Generator/ 模块所有组件
5. ⏳ 实现 CHTL/Loader/ 模块所有组件

### 第三阶段：CHTL JS编译器实现
1. ⏳ 设计 CHTLJS/Common/ 模块框架
2. ⏳ 实现 CHTLJS/Lexer/ 增强选择器支持
3. ⏳ 实现 CHTLJS/Parser/ JavaScript语法扩展
4. ⏳ 实现 CHTLJS/Generator/ DOM和事件生成
5. ⏳ 实现 CHTLJS/Loader/ CJMOD系统

### 第四阶段：系统集成和工具
1. ⏳ 集成两个编译器的统一接口
2. ⏳ 开发编译器主程序
3. ⏳ 开发IDE支持工具
4. ⏳ 完善测试系统
5. ⏳ 编写完整文档

## 🎯 核心优势

### 1. **清晰的模块分离**
- CHTL和CHTL JS各自独立
- 每个编译器都有完整的五大模块
- 避免了混杂和耦合

### 2. **专业的命名规范**
- 统一使用大驼峰命名法
- 文件名直观反映功能
- 符合C++项目标准

### 3. **可扩展的架构**
- 每个模块职责单一
- 易于添加新功能
- 支持独立开发和测试

### 4. **标准的目录布局**
- Include/ 包含所有头文件
- 源码按编译器和模块组织
- Tests/ 对应源码结构
- 支持现代C++项目惯例

## 💡 与原架构对比

| 方面 | 原架构 | 新架构 |
|------|--------|--------|
| 命名规范 | 混合（snake_case/camelCase） | 统一大驼峰（PascalCase） |
| 文件组织 | 按功能类型混合 | 按编译器清晰分离 |
| 模块化程度 | 部分模块化 | 完全模块化 |
| 可维护性 | 中等 | 高 |
| 可扩展性 | 有限 | 优秀 |
| 专业程度 | 原型级别 | 产品级别 |

这个新结构完全解决了您提到的问题，使CHTL编译器项目更加专业、清晰和可维护！

## 🚀 下一步工作

现在我们有了清晰的架构，可以开始：

1. **迁移现有AST代码**到新的CHTL模块结构
2. **实现完整的CHTL编译器**各个模块
3. **设计CHTL JS编译器**的独立架构
4. **建立统一的构建系统**
5. **创建完整的测试框架**

这个架构为CHTL编译器的长期发展奠定了坚实的基础！