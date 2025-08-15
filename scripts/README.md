# CHTL 构建脚本

本目录包含用于构建CHTL编译器、模块和VSCode扩展的脚本。

## 目录结构

```
scripts/
├── build-all.bat        # Windows统一构建脚本
├── build-all.sh         # Unix/Linux/macOS统一构建脚本
├── windows/             # Windows平台专用脚本
│   ├── build-compiler.bat
│   ├── build-vscode-extension.bat
│   ├── package-cmod.bat
│   ├── package-cjmod.bat
│   └── package-unified.bat
├── linux/               # Linux平台专用脚本
│   ├── build-compiler.sh
│   ├── build-vscode-extension.sh
│   ├── package-cmod.sh
│   ├── package-cjmod.sh
│   └── package-unified.sh
└── macos/               # macOS平台专用脚本
    ├── build-compiler.sh
    ├── build-vscode-extension.sh
    ├── package-cmod.sh
    ├── package-cjmod.sh
    └── package-unified.sh
```

## 使用说明

### 统一构建

使用统一构建脚本可以一次性构建所有组件：

```bash
# Windows
scripts\build-all.bat

# Linux/macOS
./scripts/build-all.sh
```

### 单独构建

#### 编译器构建

```bash
# Windows
scripts\windows\build-compiler.bat

# Linux
./scripts/linux/build-compiler.sh

# macOS
./scripts/macos/build-compiler.sh
```

#### VSCode扩展构建

```bash
# Windows
scripts\windows\build-vscode-extension.bat

# Linux
./scripts/linux/build-vscode-extension.sh

# macOS
./scripts/macos/build-vscode-extension.sh
```

#### 模块打包

```bash
# 打包CMOD模块
scripts\windows\package-cmod.bat <module-path>

# 打包CJMOD模块
scripts\windows\package-cjmod.bat <module-path>

# 打包统一模块（包含CMOD和CJMOD）
scripts\windows\package-unified.bat <module-path>
```

## 注意事项

1. **脚本语言**：所有脚本使用英文编写以确保跨平台兼容性
2. **环境要求**：
   - Java 17+
   - Maven 3.6+
   - Node.js 14+（用于VSCode扩展）
3. **权限**：Linux/macOS脚本需要执行权限（`chmod +x`）
4. **路径**：脚本应从项目根目录执行

## 环境变量

脚本支持以下环境变量：

- `JAVA_HOME` - Java安装目录
- `MAVEN_HOME` - Maven安装目录
- `NODE_HOME` - Node.js安装目录
- `CHTL_MODULE_PATH` - 模块搜索路径

## 故障排除

如果遇到问题：

1. 检查环境要求是否满足
2. 确保在项目根目录执行脚本
3. 查看脚本输出的错误信息
4. 参考`docs/BUILD_REQUIREMENTS.md`获取详细的构建环境配置说明