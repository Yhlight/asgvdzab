# CHTL (Custom HyperText Language) 编译器项目

[![Java Version](https://img.shields.io/badge/Java-17-blue.svg)](https://openjdk.java.net/projects/jdk/17/)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![License](https://img.shields.io/badge/license-MIT-green.svg)]()
[![Status](https://img.shields.io/badge/status-完成_v1.0-success.svg)]()

## 📋 项目简介

CHTL（Custom HyperText Language）是一种创新的前端开发语言，旨在提供更高效、更直观的Web开发体验。本项目是CHTL编译器的Java实现版本，提供完整的语言解析、编译和优化功能。

**🎉 项目状态：已完成 v1.0** - 编译器已实现所有核心功能，可以投入实际使用！

### 🎯 核心特性

- **统一语法**：HTML、CSS、JavaScript统一在一个文件中，支持模块化开发
- **增强选择器**：`{{}}` 语法提供强大的DOM操作，`{{&}}` 在局部script块中自引用
- **CHTL JS**：扩展的JavaScript语法
  - 支持 `->` 操作符（等价于 `.`）
  - 无序和可选参数
  - Unquoted literals（如 `easing: ease-in-out`）
  - 支持所有标准DOM事件
- **模板系统**：
  - `[Template]`：不可特例化的模板
  - `[Custom]`：可特例化的自定义，支持 `delete`/`insert` 操作
- **模块系统**：
  - CMOD：CHTL模板打包，支持`[Export]`声明（可选）
  - CJMOD：Java扩展模块，禁止使用`[Export]`
- **高级特性**：
  - `[Configuration]`：编译器配置和关键字自定义
  - `[Origin]`：原始HTML/CSS/JS嵌入
  - `[Namespace]`：命名空间管理
  - `[Constraint]`：约束系统
- **智能优化**：自动代码优化、压缩和性能提升
- **开发工具**：VSCode插件支持，提供语法高亮、自动补全、实时检查

## 🚀 快速开始

### 环境要求

- Java 17 或更高版本
- Maven 3.6+
- Node.js 14+（用于VSCode插件）

### 安装

```bash
# 克隆项目
git clone https://github.com/your-org/chtl-compiler.git
cd chtl-compiler

# 编译项目
mvn clean install

# 运行编译器
java -jar target/chtl-compiler.jar input.chtl -o output/
```

### 简单示例

```chtl
[Template] @MyButton {
    <button class="btn my-button">
        <slot/>
    </button>
    
    style {
        .my-button {
            background: #007bff;
            color: white;
            padding: 10px 20px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
        }
        
        .my-button:hover {
            background: #0056b3;
        }
    }
    
    script {
        // 使用增强选择器绑定事件
        {{.my-button}}.listen('click', function() {
            this.animate({
                scale: [1, 0.95, 1],
                duration: 200,
                easing: ease-out
            });
        });
    }
}

// 使用模板
@MyButton { 点击我 }
```

## 📁 项目结构

```
chtl-compiler/
├── src/
│   ├── main/
│   │   ├── java/           # Java源代码
│   │   └── resources/      # 资源文件
│   │       └── module/     # CHTL模块
│   └── test/               # 测试代码
├── test/                   # 测试文件和资源
├── docs/                   # 文档
├── vscode-chtl/           # VSCode插件
└── scripts/               # 构建和部署脚本
```

## 🔧 开发指南

### 构建项目

```bash
# 完整构建
mvn clean package

# 跳过测试构建
mvn clean package -DskipTests

# 运行测试
mvn test

# 生成文档
mvn javadoc:javadoc
```

### 运行测试

```bash
# 运行所有测试
./scripts/run-tests.sh

# 运行特定测试
java -cp "src/main/java:test/java" TestPrecisionScanner
```

## 📚 文档

- [CHTL语法文档](CHTL语法文档.md) - 完整的语言规范
- [API文档](docs/api/) - Java API参考
- [开发者指南](docs/developer-guide.md) - 贡献代码指南
- [CJMOD开发指南](CJMOD_DEVELOPER_GUIDE.md) - CJMOD模块开发
- [CMOD Export指南](CMOD_EXPORT_GUIDE.md) - CMOD [Export]块使用
- [CHTL语法示例](CHTL_SYNTAX_EXAMPLES.md) - 语法使用示例
- [Chtholly模块](src/main/java/com/chtl/module/Chtholly/README.md) - 官方主题模块
- [未来规划](FUTURE_ROADMAP.md) - 项目发展路线图

## 🤝 贡献

欢迎贡献代码！请查看[贡献指南](CONTRIBUTING.md)了解如何参与项目开发。

### 开发流程

1. Fork项目
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 创建Pull Request

## 🏆 项目完成状态

### ✅ 已完成功能（100%）
- 词法分析器和语法分析器
- 完整的AST实现
- 代码生成器（HTML/CSS/JavaScript）
- 模块系统（CMOD/CJMOD）
- 命令行工具和VSCode插件
- 所有语法特性实现
- 跨平台构建脚本

### 📊 测试通过
- 所有CHTL语法特征测试 ✅
- 模块系统测试 ✅
- 代码生成测试 ✅
- 集成测试 ✅

## 📄 许可证

本项目采用MIT许可证 - 查看[LICENSE](LICENSE)文件了解详情。

## 🙏 致谢

感谢所有贡献者和支持者，是你们让CHTL变得更好！

## 📞 联系方式

- 项目主页：[https://github.com/your-org/chtl-compiler](https://github.com/your-org/chtl-compiler)
- 问题反馈：[Issues](https://github.com/your-org/chtl-compiler/issues)
- 邮件：chtl-dev@example.com

---

⚠️ **重要提示**：本项目当前为Java实现版本，计划在未来迁移到C++17重新实现。详见[未来规划文档](FUTURE_ROADMAP.md)。