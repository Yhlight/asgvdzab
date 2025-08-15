# CHTL编译器构建说明

## 快速开始

### Windows用户

1. **使用分阶段构建（推荐）**
   ```cmd
   cd scripts\windows
   build-compiler-staged.bat
   ```

2. **使用完整构建脚本**
   ```cmd
   cd scripts
   build-all-complete.bat
   ```

3. **测试编译器**
   ```cmd
   cd scripts
   test-compiler.bat
   ```

### 构建选项

- `build-all-complete.bat` - 构建编译器和VSCode扩展
- `build-all-complete.bat --skip-vscode` - 仅构建编译器
- `build-all-complete.bat --help` - 显示帮助信息

## 构建脚本说明

### 1. **build-compiler-staged.bat**
分阶段构建脚本，按正确的依赖顺序编译：
- Stage 1: 核心接口和异常
- Stage 2: 抽象类和模型
- Stage 3: AST节点
- Stage 4: 扫描器和解析器
- Stage 5: 其余所有文件

### 2. **build-compiler-simple.bat**
简单构建脚本，一次性编译所有文件（可能有依赖问题）

### 3. **build-compiler-maven-skip-antlr.bat**
使用Maven但跳过ANTLR处理（需要Maven环境）

### 4. **create-stubs.bat**
创建ANTLR桩文件，绕过ANTLR语法错误

## 已知问题和解决方案

### 问题1: ANTLR语法错误
**症状**: `CSS3.g4`中的语法错误导致Maven构建失败

**解决方案**: 使用提供的构建脚本，它们会自动创建ANTLR桩文件

### 问题2: 编译顺序问题
**症状**: "找不到符号"错误

**解决方案**: 使用`build-compiler-staged.bat`，它按正确顺序编译

### 问题3: 缺少依赖
**症状**: 找不到ANTLR或SLF4J类

**解决方案**: 构建脚本会自动下载必要的依赖

## 构建产物

成功构建后，将生成：
- `target/chtl-compiler-1.0.jar` - 主编译器JAR
- `dist/` - 发布目录
  - `chtl-compiler-1.0.jar` - 编译器
  - `lib/` - 依赖库

## 运行编译器

```cmd
java -cp "target\chtl-compiler-1.0.jar;lib\*" com.chtl.cli.CHTLCLI --help
```

## 故障排除

1. **Java版本**: 需要Java 17或更高版本
2. **编码问题**: 源文件使用UTF-8编码
3. **路径问题**: 确保在项目根目录运行脚本
4. **权限问题**: 可能需要管理员权限下载依赖

## 开发提示

- ANTLR相关功能目前使用桩实现，CSS/JS验证功能受限
- 如需完整功能，需要修复ANTLR语法文件并重新生成解析器
- 模块系统完全可用，不依赖ANTLR