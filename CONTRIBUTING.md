# 贡献指南

感谢您对CHTL编译器项目的关注！我们欢迎各种形式的贡献。

## 📋 贡献方式

### 1. 报告问题
- 使用GitHub Issues报告bug
- 提供详细的复现步骤
- 包含错误信息和环境信息

### 2. 提出新功能
- 先在Issues中讨论
- 说明功能的使用场景
- 考虑向后兼容性

### 3. 提交代码
- Fork项目并创建分支
- 编写清晰的提交信息
- 确保测试通过
- 提交Pull Request

## 🔧 开发环境设置

```bash
# 1. Fork并克隆项目
git clone https://github.com/YOUR_USERNAME/chtl-compiler.git
cd chtl-compiler

# 2. 创建功能分支
git checkout -b feature/your-feature-name

# 3. 安装依赖
mvn install

# 4. 运行测试
mvn test
```

## 📝 代码规范

### Java代码规范
- 使用4个空格缩进
- 遵循Java命名约定
- 添加适当的注释
- 保持方法简洁（不超过50行）

### 提交信息格式
```
<type>(<scope>): <subject>

<body>

<footer>
```

类型：
- feat: 新功能
- fix: 修复bug
- docs: 文档更新
- style: 代码格式
- refactor: 重构
- test: 测试相关
- chore: 构建工具或辅助工具

## 🧪 测试要求

- 所有新功能必须有对应的测试
- 保持测试覆盖率在80%以上
- 测试应该快速且独立
- 使用描述性的测试名称

## 📄 文档要求

- 更新相关文档
- 添加JavaDoc注释
- 提供使用示例
- 更新CHANGELOG

## 🚀 Pull Request流程

1. 确保代码符合规范
2. 所有测试通过
3. 更新相关文档
4. 填写PR模板
5. 等待代码审查
6. 根据反馈修改
7. 合并到主分支

## 🤝 行为准则

- 尊重所有贡献者
- 保持专业和友好
- 接受建设性批评
- 帮助新贡献者

## 📞 联系方式

- GitHub Issues：技术问题
- 邮件列表：chtl-dev@example.com
- Discord：实时讨论

再次感谢您的贡献！