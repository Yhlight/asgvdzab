# CHTL编译器 IDE配置指南

## 问题诊断

如果您在IDE中无法编译项目，可能是由于以下原因：

1. **缺少外部依赖**：项目依赖ANTLR、SLF4J等库
2. **Java版本不兼容**：需要Java 17或更高版本
3. **编码问题**：源文件使用UTF-8编码

## 快速解决方案

### 方法1：使用提供的编译脚本

**Windows用户：**
```
compile-for-ide.bat
```

**Linux/Mac用户：**
```bash
./compile-for-ide.sh
```

这些脚本会：
- 检查依赖
- 分阶段编译项目
- 生成JAR文件
- 提供详细的错误信息

### 方法2：手动配置IDE

#### IntelliJ IDEA

1. **打开项目**
   - File → Open → 选择项目根目录

2. **设置项目SDK**
   - File → Project Structure → Project
   - Project SDK: 选择Java 17或更高版本
   - Project language level: 17

3. **标记源代码目录**
   - 右键点击 `src/main/java` → Mark Directory as → Sources Root
   - 右键点击 `src/test/java` → Mark Directory as → Test Sources Root

4. **添加依赖（如果没有Maven）**
   - File → Project Structure → Libraries
   - 点击 + → Java → 选择下载的JAR文件
   - 需要的依赖：
     - antlr4-runtime-4.13.1.jar
     - slf4j-api-2.0.9.jar
     - logback-classic-1.4.11.jar

5. **运行配置**
   - Run → Edit Configurations
   - 添加 Application
   - Main class: `com.chtl.cli.CHTLCLI`
   - Program arguments: `--help`

#### Eclipse

1. **导入项目**
   - File → Import → General → Existing Projects into Workspace

2. **设置Java版本**
   - 右键项目 → Properties → Java Build Path → Libraries
   - 确保使用Java 17或更高版本

3. **添加源文件夹**
   - Java Build Path → Source
   - Add Folder → 选择 `src/main/java`

4. **添加外部JAR**
   - Java Build Path → Libraries → Add External JARs
   - 添加所需的依赖JAR文件

#### Visual Studio Code

1. **安装Java扩展包**
   - 安装 "Extension Pack for Java"

2. **配置工作区**
   创建 `.vscode/settings.json`：
   ```json
   {
     "java.project.sourcePaths": ["src/main/java"],
     "java.project.outputPath": "target/classes",
     "java.project.referencedLibraries": [
       "lib/**/*.jar"
     ]
   }
   ```

3. **创建启动配置**
   创建 `.vscode/launch.json`：
   ```json
   {
     "version": "0.2.0",
     "configurations": [
       {
         "type": "java",
         "name": "Launch CHTL CLI",
         "request": "launch",
         "mainClass": "com.chtl.cli.CHTLCLI",
         "args": "--help"
       }
     ]
   }
   ```

## 依赖下载

如果缺少依赖，可以从以下位置下载：

1. **ANTLR Runtime**
   - https://www.antlr.org/download/antlr4-runtime-4.13.1.jar

2. **SLF4J API**
   - https://repo1.maven.org/maven2/org/slf4j/slf4j-api/2.0.9/slf4j-api-2.0.9.jar

3. **Logback Classic**
   - https://repo1.maven.org/maven2/ch/qos/logback/logback-classic/1.4.11/logback-classic-1.4.11.jar

4. **Logback Core**
   - https://repo1.maven.org/maven2/ch/qos/logback/logback-core/1.4.11/logback-core-1.4.11.jar

将下载的JAR文件放在项目根目录的 `lib/` 文件夹中。

## 使用Maven（推荐）

如果您的系统安装了Maven：

```bash
# 下载依赖并编译
mvn clean compile

# 打包
mvn package

# 跳过测试
mvn package -DskipTests
```

## 常见问题

### 1. "找不到符号"错误
- 确保所有依赖都已添加到classpath
- 检查Java版本是否为17或更高

### 2. 编码错误
- 确保IDE使用UTF-8编码
- IntelliJ: Settings → Editor → File Encodings → 全部设为UTF-8

### 3. 内存不足
- 增加JVM内存：`-Xmx2G -Xms512M`

### 4. 模块找不到
- 确保 `src/main/java/com/chtl/module/` 目录存在
- 运行时添加模块路径：`-Dchtl.module.path=./module`

## 测试运行

编译成功后，测试运行：

```bash
# 显示帮助
java -cp "target/classes:lib/*" com.chtl.cli.CHTLCLI --help

# Windows
java -cp "target\classes;lib\*" com.chtl.cli.CHTLCLI --help
```

## 调试技巧

1. **启用详细日志**
   ```bash
   java -Dlogback.debug=true -cp ... com.chtl.cli.CHTLCLI
   ```

2. **单步调试**
   - 在IDE中设置断点
   - 使用Debug模式运行

3. **查看编译错误**
   - 检查 `compile_errors_*.txt` 文件
   - 确保没有语法错误

如果仍有问题，请检查：
- Java版本：`java -version`
- 项目结构是否完整
- 所有依赖是否已下载