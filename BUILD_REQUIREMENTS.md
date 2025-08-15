# CHTL Compiler Build Requirements

## Overview

This document outlines the requirements for building the CHTL compiler from source.

## System Requirements

### Operating Systems
- **Windows**: Windows 10 or later
- **macOS**: macOS 10.14 (Mojave) or later
- **Linux**: Ubuntu 18.04+, CentOS 7+, or any modern Linux distribution

### Hardware Requirements
- **RAM**: Minimum 4GB, recommended 8GB
- **Disk Space**: At least 2GB free space
- **CPU**: x64 architecture

## Software Requirements

### Core Dependencies

#### 1. Java Development Kit (JDK)
- **Version**: Java 17 or higher
- **Download**: 
  - Oracle JDK: https://www.oracle.com/java/technologies/downloads/
  - OpenJDK: https://adoptium.net/
  - Amazon Corretto: https://aws.amazon.com/corretto/

**Verification**:
```bash
java --version
# Should output: java 17.x.x or higher
```

#### 2. Apache Maven
- **Version**: 3.6.0 or higher
- **Download**: https://maven.apache.org/download.cgi

**Verification**:
```bash
mvn --version
# Should show Maven 3.6.0 or higher
```

### Platform-Specific Requirements

#### Windows
- **PowerShell**: Version 5.1 or higher (for build scripts)
- **Command Prompt**: Required for batch scripts

#### macOS
- **Xcode Command Line Tools**: Required for development tools
  ```bash
  xcode-select --install
  ```

#### Linux
- **Build Essential**: Basic compilation tools
  ```bash
  # Ubuntu/Debian
  sudo apt-get install build-essential
  
  # CentOS/RHEL
  sudo yum groupinstall "Development Tools"
  ```

### Optional Dependencies

#### For VSCode Extension Development
- **Node.js**: Version 16.x or higher
- **npm**: Version 8.x or higher
- **VSCode**: Latest stable version

**Installation**:
```bash
# Download from https://nodejs.org/
node --version  # Should be 16.x or higher
npm --version   # Should be 8.x or higher
```

## Environment Setup

### 1. Set JAVA_HOME
The JAVA_HOME environment variable must point to your JDK installation.

**Windows**:
```batch
setx JAVA_HOME "C:\Program Files\Java\jdk-17"
setx PATH "%PATH%;%JAVA_HOME%\bin"
```

**macOS/Linux**:
```bash
export JAVA_HOME=/usr/lib/jvm/java-17-openjdk
export PATH=$JAVA_HOME/bin:$PATH

# Add to ~/.bashrc or ~/.zshrc for persistence
echo 'export JAVA_HOME=/usr/lib/jvm/java-17-openjdk' >> ~/.bashrc
echo 'export PATH=$JAVA_HOME/bin:$PATH' >> ~/.bashrc
```

### 2. Configure Maven
Ensure Maven can find the JDK:

**Verify Maven Java version**:
```bash
mvn --version
# Should show: Java version: 17.x.x
```

## Build Process

### Quick Build
```bash
# Clone the repository
git clone <repository-url>
cd chtl-compiler

# Build the compiler
mvn clean package

# The compiled JAR will be in target/
```

### Platform-Specific Build Scripts

**Windows**:
```batch
scripts\windows\build-compiler.bat
```

**Linux/macOS**:
```bash
./scripts/linux/build-compiler.sh
# or
./scripts/mac/build-compiler.sh
```

## Troubleshooting

### Common Issues

1. **Java version mismatch**
   - Error: `Unsupported class file major version`
   - Solution: Ensure Java 17+ is installed and JAVA_HOME is set correctly

2. **Maven not found**
   - Error: `mvn: command not found`
   - Solution: Install Maven and add to PATH

3. **Out of memory during build**
   - Error: `java.lang.OutOfMemoryError`
   - Solution: Increase Maven memory:
     ```bash
     export MAVEN_OPTS="-Xmx2048m -XX:MaxPermSize=512m"
     ```

4. **Permission denied on Linux/macOS**
   - Error: `Permission denied`
   - Solution: Make scripts executable:
     ```bash
     chmod +x scripts/linux/*.sh
     ```

### Build Verification

After successful build, verify the installation:

```bash
# Check if JAR was created
ls target/chtl-compiler*.jar

# Run the compiler
java -jar target/chtl-compiler.jar --version
```

## IDE Setup

### IntelliJ IDEA
1. Import as Maven project
2. Set Project SDK to Java 17
3. Enable annotation processing

### Eclipse
1. Install m2e plugin
2. Import as Maven project
3. Set compiler compliance to Java 17

### VS Code
1. Install Java Extension Pack
2. Install Maven for Java
3. Open folder and let VS Code configure the project

## CI/CD Requirements

For automated builds:

### GitHub Actions
```yaml
- uses: actions/setup-java@v3
  with:
    java-version: '17'
    distribution: 'temurin'
```

### Jenkins
- Install JDK 17
- Install Maven plugin
- Configure Maven installation

### Docker
```dockerfile
FROM maven:3.8-openjdk-17 AS build
WORKDIR /app
COPY . .
RUN mvn clean package
```

## Module Development Requirements

For CJMOD development:
- CHTL Compiler (built from source)
- Java IDE with Maven support
- Basic understanding of Java interfaces

## Testing Requirements

- **JUnit**: Version 5.x (included in pom.xml)
- **Memory**: Additional 2GB for running full test suite
- **Time**: Full test suite takes ~5-10 minutes

## Security Considerations

- Always verify downloaded JDK/Maven from official sources
- Use HTTPS for all downloads
- Verify checksums when available
- Keep dependencies updated

## Getting Help

If you encounter build issues:

1. Check this document first
2. Review the [FAQ](docs/FAQ.md)
3. Search existing issues on GitHub
4. Create a new issue with:
   - OS and version
   - Java version (`java --version`)
   - Maven version (`mvn --version`)
   - Complete error message
   - Steps to reproduce

## Next Steps

After successfully building the compiler:

1. Run tests: `mvn test`
2. Build modules: See [Module Development Guide](docs/CJMOD_DEVELOPMENT_GUIDE.md)
3. Try examples: See [examples/](examples/) directory
4. Read the [CHTL Syntax Documentation](CHTL语法文档.md)