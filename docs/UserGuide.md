# CHTL 用户使用手册

## 目录

1. [简介](#简介)
2. [安装](#安装)
3. [快速开始](#快速开始)
4. [基础语法](#基础语法)
5. [高级特性](#高级特性)
6. [命令行使用](#命令行使用)
7. [最佳实践](#最佳实践)
8. [故障排除](#故障排除)

## 简介

CHTL (Custom HyperText Language) 是一个创新的Web开发语言，旨在简化HTML、CSS和JavaScript的编写。它提供了更简洁的语法、强大的模板系统和智能的编译优化。

### 主要特性

- **简洁语法**: 减少样板代码，提高开发效率
- **模板系统**: 支持元素、样式和变量模板
- **局部样式**: 组件级别的样式隔离
- **增强JS**: CHTL JS提供更直观的DOM操作
- **模块化**: 内置导入和命名空间支持
- **优化输出**: 自动压缩和优化生成的代码

## 安装

### 系统要求

- Java 17 或更高版本
- 操作系统: Windows, macOS, Linux

### 安装步骤

1. 下载最新版本的CHTL编译器
2. 解压到目标目录
3. 将bin目录添加到系统PATH
4. 验证安装：

```bash
chtl --version
```

## 快速开始

### 第一个CHTL程序

创建文件 `hello.chtl`:

```chtl
// hello.chtl
body {
    h1 {
        text { Hello, CHTL! }
        style {
            color: #007bff;
            text-align: center;
        }
    }
    
    p {
        text { Welcome to CHTL programming. }
    }
}
```

编译：

```bash
chtl hello.chtl
```

这将生成 `hello.html` 文件。

## 基础语法

### 1. 注释

```chtl
// 单行注释

/* 
   多行注释
   可以跨越多行
*/
```

### 2. 元素

```chtl
div {
    // 元素内容
}

// 自闭合标签
img {
    src: "image.png";
    alt: "图片";
}
```

### 3. 属性

```chtl
div {
    id: "myDiv";           // 使用冒号
    class = "container";   // 使用等号（CE对等式）
    data-value: "123";
}
```

### 4. 文本节点

```chtl
p {
    text { 这是一段文本 }
    text { "带引号的文本" }
    text { '单引号文本' }
}
```

### 5. 局部样式

```chtl
div {
    style {
        // 内联样式
        color: red;
        font-size: 16px;
        
        // 自动化类名
        .container {
            padding: 20px;
        }
        
        // 上下文推导
        & {
            border: 1px solid black;
        }
        &:hover {
            background: #f0f0f0;
        }
    }
}
```

### 6. 模板定义

#### 元素模板

```chtl
[Template] @Element Card {
    div {
        class: "card";
        h3 { text { 卡片标题 } }
        p { text { 卡片内容 } }
    }
}

// 使用模板
body {
    @Element Card;
}
```

#### 样式模板

```chtl
[Template] @Style Primary {
    color: #007bff;
    font-weight: bold;
}

// 使用样式模板
h1 {
    style {
        @Style Primary;
        font-size: 24px;
    }
}
```

#### 变量组模板

```chtl
[Template] @Var Colors {
    primary: "#007bff";
    secondary: "#6c757d";
    success: "#28a745";
}

// 使用变量
div {
    style {
        color: @Var Colors.primary;
        background: @Var Colors.secondary;
    }
}
```

## 高级特性

### 1. 自定义（Custom）

```chtl
// 自定义样式组（空值）
[Custom] @Style FlexCenter {
    display: flex;
    justify-content: ;
    align-items: ;
}

// 使用时填充值
div {
    style {
        @Style FlexCenter {
            justify-content: center;
            align-items: center;
        }
    }
}
```

### 2. 继承

```chtl
[Template] @Style BaseButton {
    padding: 10px 20px;
    border: none;
    cursor: pointer;
}

[Template] @Style PrimaryButton {
    inherit BaseButton;
    background: #007bff;
    color: white;
}
```

### 3. 命名空间

```chtl
[Namespace] components {
    [Template] @Element Button {
        button {
            class: "btn";
            text { 点击我 }
        }
    }
}

// 使用命名空间中的模板
body {
    @Element Button from components;
}
```

### 4. 导入

```chtl
// 导入其他CHTL文件
[Import] @Chtl from "./components.chtl" as comp;

// 导入CSS
[Import] @Style from "./styles.css";

// 导入JavaScript
[Import] @JavaScript from "./script.js";
```

### 5. 约束（Constraint）

```chtl
[Namespace] restricted {
    // 禁止使用模板
    except [Template];
    
    // 只允许特定元素
    except @Html div;
    except @Html span;
    
    p { text { 允许的内容 } }
}
```

### 6. CHTL JS

```chtl
div {
    id: "myDiv";
    
    script {
        // 增强选择器
        let div = {{#myDiv}};
        
        // 事件监听
        div->listen("click", () => {
            console.log("Clicked!");
        });
        
        // 事件委托
        div->delegate("click", ".item", (e) => {
            console.log("Item clicked");
        });
        
        // 动画
        div->animate({
            duration: 1000,
            from: { opacity: 0 },
            to: { opacity: 1 }
        });
    }
}
```

## 命令行使用

### 基本用法

```bash
chtl [选项] <源文件>
```

### 常用选项

#### 输出控制
```bash
# 指定输出文件
chtl -o output.html input.chtl

# 生产模式（最大优化）
chtl --production app.chtl

# 开发模式（调试友好）
chtl --development app.chtl
```

#### 调试选项
```bash
# 启用调试模式
chtl -d input.chtl

# 显示词法分析结果
chtl --show-tokens input.chtl

# 显示语法树
chtl --show-ast input.chtl

# 详细输出
chtl -v input.chtl
```

#### 监视模式
```bash
# 自动重新编译
chtl --watch input.chtl
```

#### 性能选项
```bash
# 设置并行任务数
chtl -j 4 input.chtl

# 设置编译超时
chtl --timeout 120 input.chtl
```

### 完整示例

```bash
# 生产构建，输出到dist目录，禁用Source Map
chtl --production -o dist/index.html --no-source-map app.chtl

# 开发模式，启用所有调试信息
chtl --development -d --show-tokens --show-ast --verbose test.chtl

# 监视模式，自动编译
chtl --watch --development src/main.chtl
```

## 最佳实践

### 1. 项目结构

```
my-project/
├── src/
│   ├── main.chtl
│   ├── components/
│   │   ├── header.chtl
│   │   └── footer.chtl
│   ├── styles/
│   │   └── theme.chtl
│   └── scripts/
│       └── app.chtl
├── dist/
└── chtl.config.json
```

### 2. 模块化开发

```chtl
// components/button.chtl
[Namespace] components {
    [Template] @Element Button {
        button {
            class: "btn";
            style {
                @Style ButtonStyle;
            }
            text { ${text} }
        }
    }
    
    [Template] @Style ButtonStyle {
        padding: 10px 20px;
        border-radius: 4px;
        cursor: pointer;
    }
}
```

### 3. 样式管理

```chtl
// styles/theme.chtl
[Template] @Var Theme {
    // 颜色
    primaryColor: "#007bff";
    secondaryColor: "#6c757d";
    
    // 间距
    spacing: "8px";
    spacingLarge: "16px";
    
    // 字体
    fontFamily: "Arial, sans-serif";
}
```

### 4. 性能优化

- 使用模板减少重复代码
- 合理使用命名空间避免污染
- 启用生产模式优化输出
- 使用局部样式减少全局CSS

## 故障排除

### 常见错误

#### 1. 语法错误

**错误**: `期望 ';'`

**解决**: 检查样式属性是否缺少分号

```chtl
// 错误
style {
    color: red
    background: blue;
}

// 正确
style {
    color: red;
    background: blue;
}
```

#### 2. 未定义引用

**错误**: `未定义的模板: Button`

**解决**: 确保模板已定义或正确导入

```chtl
// 确保已定义
[Template] @Element Button {
    // ...
}

// 或从其他文件导入
[Import] @Chtl from "./components.chtl";
```

#### 3. 循环依赖

**错误**: `检测到循环依赖`

**解决**: 重新组织代码结构，避免文件间的循环引用

### 调试技巧

1. 使用 `-d` 选项查看详细错误信息
2. 使用 `--show-ast` 检查语法树结构
3. 使用 `--verbose` 获取完整的编译过程信息
4. 检查生成的HTML确认输出是否正确

### 获取帮助

- 查看帮助: `chtl --help`
- 查看版本: `chtl --version`
- 访问官方文档: https://chtl-lang.org/docs
- 提交问题: https://github.com/chtl-lang/chtl/issues

## 附录

### A. 完整语法参考

请参阅 [CHTL语法文档.md](../CHTL语法文档.md)

### B. API文档

运行以下命令生成API文档：

```bash
chtl --generate-api-docs
```

### C. 示例项目

访问 https://github.com/chtl-lang/examples 获取完整示例项目。