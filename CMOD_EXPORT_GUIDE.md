# CMOD [Export]块使用指南

## 概述

`[Export]`块是CMOD模块info文件中的重要组成部分，用于声明模块对外暴露的接口。它有两个主要作用：

1. **明确模块接口**：指定哪些模板可以被外部使用
2. **优化查询性能**：编译器可以通过Export信息快速定位模块功能

## 重要说明

- **CMOD模块**：建议包含`[Export]`块（可选但推荐）
- **CJMOD模块**：**不应该**使用`[Export]`块（设计上不需要）

## [Export]块语法

```chtl
[Export]
{
    // 导出变量模板
    @Var 变量名1, 变量名2, 变量名3;
    
    // 导出样式模板
    @Style 样式名1, 样式名2;
    
    // 导出元素模板
    @Element 元素名1, 元素名2, 元素名3,
             元素名4, 元素名5;
}
```

## 使用方式

### 1. 手动提供（推荐）

开发者可以手动编写`[Export]`块，明确指定要导出的模板：

```chtl
// info/MyModule.chtl
[Info]
{
    name = "MyModule";
    version = "1.0.0";
    // ... 其他信息
}

[Export]
{
    // 只导出这些指定的模板
    @Var ThemeColors, ThemeFonts;
    @Style BaseStyle, CardStyle;
    @Element PageContainer, Header, Footer;
}
```

**优势**：
- 精确控制模块的公开API
- 隐藏内部实现细节
- 提高模块封装性
- 优化编译性能

### 2. 系统自动生成

如果开发者没有提供`[Export]`块，CHTL编译器会在编译时自动生成，将模块中的所有模板都导出。

**注意**：
- 自动生成会暴露所有模板（包括内部使用的）
- 可能影响模块封装性
- 建议在发布前手动添加`[Export]`块

## 完整示例

### Chtholly模块的info文件

```chtl
// Chtholly CMOD模块信息文件
[Info]
{
    name = "Chtholly";
    version = "1.0.0";
    description = "珂朵莉主题模块 - 世界上最幸福的女孩";
    author = "CHTL Team";
    license = "MIT";
    dependencies = "";
    category = "theme";
    minCHTLVersion = "1.0.0";
    maxCHTLVersion = "2.0.0";
}

// 手动指定导出的模板
[Export]
{
    // 导出的变量模板
    @Var ChthollyColors, ChthollyFonts, ChthollySpacing;
    
    // 导出的样式模板
    @Style ChthollyBase, ChthollyCard, ChthollyButton;
    
    // 导出的元素模板
    @Element ChthollyPage, ChthollyTitle, ChthollyCard, 
             ChthollyButton, ChthollyGallery;
}
```

## 最佳实践

1. **始终手动编写Export**：不要依赖自动生成
2. **最小化导出**：只导出必要的公开API
3. **分组组织**：按类型（@Var、@Style、@Element）分组
4. **添加注释**：说明每组导出的用途
5. **版本兼容**：升级时保持已导出接口的兼容性

## 打包工具行为

### CMOD打包脚本

- 检查是否存在`[Export]`块
- 如果不存在，提示建议添加
- 如果存在，显示"将只导出指定的模板"

### CJMOD打包脚本

- 检查是否存在`[Export]`块
- 如果存在，报错并要求移除
- CJMOD通过Java接口定义功能，不需要Export机制

## 总结

`[Export]`块是CMOD模块的重要特性，通过合理使用可以：

- 提高模块的封装性和安全性
- 优化编译器的查询性能
- 为模块使用者提供清晰的API文档
- 便于模块版本管理和升级

记住：**CMOD使用[Export]，CJMOD不使用[Export]**。