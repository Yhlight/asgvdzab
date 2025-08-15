# CHTL编译器构建要求

## 概述

本文档说明了从源代码构建CHTL编译器所需的环境要求。

## 系统要求

### 操作系统
- **Windows**: Windows 10或更高版本
- **macOS**: macOS 10.14 (Mojave)或更高版本
- **Linux**: Ubuntu 18.04+、CentOS 7+或任何现代Linux发行版

### 硬件要求
- **内存**: 最低4GB，推荐8GB
- **磁盘空间**: 至少2GB可用空间
- **CPU**: x64架构

## 软件要求

### 核心依赖

#### 1. Java开发工具包 (JDK)
- **版本**: Java 17或更高版本
- **下载地址**: 
  - Oracle JDK: https://www.oracle.com/java/technologies/downloads/
  - OpenJDK: https://adoptium.net/
  - Amazon Corretto: https://aws.amazon.com/corretto/

**验证安装**:
```bash
java --version
# 应该输出: java 17.x.x 或更高版本
```

#### 2. Apache Maven
- **版本**: 3.6.0或更高版本
- **下载地址**: https://maven.apache.org/download.cgi

**验证安装**:
```bash
mvn --version
# 应该显示 Maven 3.6.0 或更高版本
```

### 平台特定要求

#### Windows
- **PowerShell**: 5.1或更高版本（用于构建脚本）
- **命令提示符**: 批处理脚本所需

#### macOS
- **Xcode命令行工具**: 开发工具所需
  ```bash
  xcode-select --install
  ```

#### Linux
- **基础编译工具**: 基本编译工具
  ```bash
  # Ubuntu/Debian
  sudo apt-get install build-essential
  
  # CentOS/RHEL
  sudo yum groupinstall "Development Tools"
  ```

### 可选依赖

#### VSCode扩展开发
- **Node.js**: 16.x或更高版本
- **npm**: 8.x或更高版本
- **VSCode**: 最新稳定版

**安装**:
```bash
# 从 https://nodejs.org/ 下载
node --version  # 应该是 16.x 或更高版本
npm --version   # 应该是 8.x 或更高版本
```

## 环境设置

### 1. 设置JAVA_HOME
JAVA_HOME环境变量必须指向JDK安装目录。

**Windows**:
```batch
setx JAVA_HOME "C:\Program Files\Java\jdk-17"
setx PATH "%PATH%;%JAVA_HOME%\bin"
```

**macOS/Linux**:
```bash
export JAVA_HOME=/usr/lib/jvm/java-17-openjdk
export PATH=$JAVA_HOME/bin:$PATH

# 添加到 ~/.bashrc 或 ~/.zshrc 以便持久化
echo 'export JAVA_HOME=/usr/lib/jvm/java-17-openjdk' >> ~/.bashrc
echo 'export PATH=$JAVA_HOME/bin:$PATH' >> ~/.bashrc
```

### 2. 配置Maven
确保Maven能找到JDK：

**验证Maven使用的Java版本**:
```bash
mvn --version
# 应该显示: Java version: 17.x.x
```

## 构建过程

### 快速构建
```bash
# 克隆仓库
git clone <repository-url>
cd chtl-compiler

# 构建编译器
mvn clean package

# 编译后的JAR文件将在 target/ 目录下
```

### 平台特定构建脚本

**Windows**:
```batch
scripts\windows\build-compiler.bat
```

**Linux/macOS**:
```bash
./scripts/linux/build-compiler.sh
# 或
./scripts/mac/build-compiler.sh
```

## 故障排除

### 常见问题

1. **Java版本不匹配**
   - 错误: `Unsupported class file major version`
   - 解决方案: 确保安装了Java 17+并正确设置了JAVA_HOME

2. **找不到Maven**
   - 错误: `mvn: command not found`
   - 解决方案: 安装Maven并添加到PATH

3. **构建时内存不足**
   - 错误: `java.lang.OutOfMemoryError`
   - 解决方案: 增加Maven内存:
     ```bash
     export MAVEN_OPTS="-Xmx2048m -XX:MaxPermSize=512m"
     ```

4. **Linux/macOS上权限被拒绝**
   - 错误: `Permission denied`
   - 解决方案: 使脚本可执行:
     ```bash
     chmod +x scripts/linux/*.sh
     ```

### 构建验证

构建成功后，验证安装：

```bash
# 检查JAR文件是否创建
ls target/chtl-compiler*.jar

# 运行编译器
java -jar target/chtl-compiler.jar --version
```

## IDE设置

### IntelliJ IDEA
1. 作为Maven项目导入
2. 设置项目SDK为Java 17
3. 启用注解处理

### Eclipse
1. 安装m2e插件
2. 作为Maven项目导入
3. 设置编译器兼容性为Java 17

### VS Code
1. 安装Java扩展包
2. 安装Maven for Java
3. 打开文件夹，让VS Code配置项目

## CI/CD要求

自动化构建配置：

### GitHub Actions
```yaml
- uses: actions/setup-java@v3
  with:
    java-version: '17'
    distribution: 'temurin'
```

### Jenkins
- 安装JDK 17
- 安装Maven插件
- 配置Maven安装

### Docker
```dockerfile
FROM maven:3.8-openjdk-17 AS build
WORKDIR /app
COPY . .
RUN mvn clean package
```

## 模块开发要求

CJMOD开发需要：
- CHTL编译器（从源代码构建）
- 支持Maven的Java IDE
- 对Java接口的基本了解

## 测试要求

- **JUnit**: 5.x版本（包含在pom.xml中）
- **内存**: 运行完整测试套件需要额外2GB
- **时间**: 完整测试套件需要约5-10分钟

## 安全考虑

- 始终从官方来源验证下载的JDK/Maven
- 所有下载使用HTTPS
- 可用时验证校验和
- 保持依赖项更新

## 获取帮助

如果遇到构建问题：

1. 首先查看本文档
2. 查看[常见问题](docs/FAQ.md)
3. 搜索GitHub上的现有问题
4. 创建新问题时包含：
   - 操作系统和版本
   - Java版本（`java --version`）
   - Maven版本（`mvn --version`）
   - 完整错误信息
   - 重现步骤

## 下一步

成功构建编译器后：

1. 运行测试: `mvn test`
2. 构建模块: 参见[模块开发指南](docs/CJMOD_DEVELOPMENT_GUIDE.md)
3. 尝试示例: 参见[examples/](examples/)目录
4. 阅读[CHTL语法文档](CHTL语法文档.md)