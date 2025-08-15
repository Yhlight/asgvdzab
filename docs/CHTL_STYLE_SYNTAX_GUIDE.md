# CHTL样式语法指南

## 全局样式 vs 局部样式

### 全局样式块

全局样式块是直接在CHTL文件顶层定义的 `style { }` 块：

```chtl
style {
    .container {
        width: 100%;
        background: ThemeColor(primary);  // ✅ 可以使用变量
    }
}
```

**全局样式可以使用的CHTL功能：**
- ✅ 变量函数调用：`ThemeColor(primary)`
- ✅ 标准CSS语法

**全局样式不能使用的功能：**
- ❌ `&` 符号（上下文推导）
- ❌ 自动添加类名
- ❌ 其他局部样式特有功能

### 局部样式块

局部样式块是在元素内部定义的 `style { }` 块：

```chtl
div {
    style {
        // 内联样式
        color: ThemeColor(primary);  // ✅ 可以使用变量
        
        // 类选择器 - 自动添加到元素
        .box {
            width: 300px;
        }
        
        // 使用&符号
        &:hover {  // ✅ 局部样式特有功能
            background: ThemeColor(hover);
        }
    }
}
```

**局部样式特有功能：**
- ✅ 内联样式（直接写属性）
- ✅ 自动添加类名到元素
- ✅ `&` 符号（上下文推导）
- ✅ 变量函数调用

## 变量使用语法

### 正确的变量使用方式

```chtl
// 定义变量组
[Template] @Var ThemeColor {
    primary: "#007bff";
    secondary: "#6c757d";
    background: "#f8f9fa";
}

// 在样式中使用 - 函数调用形式
style {
    body {
        color: ThemeColor(primary);      // ✅ 正确
        background: ThemeColor(background); // ✅ 正确
    }
}
```

### 错误的变量使用方式

```chtl
style {
    body {
        color: @Var ThemeColor.primary;    // ❌ 错误 - 不支持点号语法
        background: @Var ThemeColor(primary); // ❌ 不必要 - @Var前缀是多余的
    }
}
```

## 关键区别总结

| 功能 | 全局样式 | 局部样式 |
|------|----------|----------|
| 变量函数调用 | ✅ 支持 | ✅ 支持 |
| 标准CSS语法 | ✅ 支持 | ✅ 支持 |
| 内联样式 | ❌ 不支持 | ✅ 支持 |
| `&` 符号 | ❌ 不支持 | ✅ 支持 |
| 自动添加类名 | ❌ 不支持 | ✅ 支持 |

## 扫描器行为

增强的PrecisionScanner会：

1. **在所有CSS上下文中**识别变量函数调用（如 `ThemeColor(primary)`）
2. **仅在局部样式中**处理 `&` 符号和其他特殊功能
3. **精确切割**CHTL和CSS片段，保持语义完整性
4. **正确区分**全局和局部样式上下文