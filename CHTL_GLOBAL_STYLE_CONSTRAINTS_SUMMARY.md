# CHTL全局样式块约束器

## 🎯 约束器作用

根据CHTL语法文档，明确全局样式块中**允许**和**禁止**的语法元素，确保语法边界清晰。

## ✅ 允许的语法元素

### 1. 模板变量使用
```chtl
/* 正确格式：VariableGroupName(variableName) */
.container {
    color: ThemeColor(primaryColor);
    background: ThemeColor(tableColor);
}
```

### 2. 自定义变量使用和特例化
```chtl
/* 基本使用 */
.text {
    color: CustomColor(userColor);
}

/* 特例化：指定具体值 */
.special {
    color: ThemeColor(tableColor = rgb(255, 192, 203));
}
```

### 3. 模板样式组和自定义样式组
```chtl
/* 样式组引用 */
@Style DefaultButton;

/* 全缀名样式组 */
[Template] @Style BaseTheme;
[Custom] @Style UserTheme;
```

### 4. 无值样式组
```chtl
/* 样式组包含需要填入值的属性 */
@Style TextSet {
    color: red;
    font-size: 16px;
}
```

### 5. 自定义样式组特例化
```chtl
/* 样式组的特例化操作 */
@Style CustomButton {
    delete border, margin;
}
```

### 6. Delete操作
```chtl
/* 删除属性 */
delete line-height, border;

/* 删除继承 */
delete @Style OldTheme;
```

### 7. 继承
```chtl
/* 显式继承 */
inherit @Style BaseTheme;

/* 或直接引用（组合式继承） */
@Style BaseTheme;
```

### 8. 生成器注释
```chtl
-- 这是生成器注释，会被生成器识别和处理
```

### 9. 全缀名
```chtl
/* 避免命名冲突的完整限定名 */
[Custom] @Style MyButton;
[Template] @Element Box;
```

### 10. 原始嵌入
```chtl
/* 原始CSS代码，直接输出 */
[Origin] @Style {
    /* 任何CSS代码 */
    * { box-sizing: border-box; }
    body { margin: 0; }
}
```

### 11. 从命名空间获取（导入）
```chtl
/* 注意：主要用于文件导入 */
ButtonStyle from ui/components.chtl;
```

### 12. 基本CSS语法
```chtl
/* 标准CSS选择器和属性 */
.container {
    width: 100%;
    margin: 0 auto;
}

#header {
    background: blue;
}
```

## ❌ 禁止的语法元素

### 1. 局部样式特性
```chtl
/* ❌ 禁止：伪类应在局部样式使用 */
.button:hover {
    background: red;
}

/* ❌ 禁止：简化伪类语法 */
&:focus {
    border: 1px solid blue;
}

/* ❌ 禁止：媒体查询应在局部使用 */
@media (max-width: 768px) {
    .container { width: 100%; }
}

/* ❌ 禁止：动画关键帧 */
@keyframes fadeIn {
    from { opacity: 0; }
    to { opacity: 1; }
}
```

### 2. 嵌套样式块
```chtl
/* ❌ 禁止：全局样式中不允许嵌套style块 */
style {
    color: green;
}
```

### 3. 伪元素
```chtl
/* ❌ 禁止：伪元素应在局部样式使用 */
.element::before {
    content: "";
}
```

## 🔍 语法验证示例

### 正确的全局样式块
```chtl
/* ✅ 正确的CHTL全局样式语法 */
.main-container {
    color: ThemeColor(primaryColor);
    background: CustomColor(userBg);
}

#header {
    font-size: 24px;
}

@Style DefaultButton;
[Template] @Style BaseTheme;
[Custom] @Style UserTheme;

inherit @Style BaseTheme;
delete border, margin;
delete @Style OldTheme;

-- 全局样式配置注释

[Origin] @Style {
    /* 原始CSS代码 */
    * { box-sizing: border-box; }
}
```

### 错误的全局样式块
```chtl
/* ❌ 错误的全局样式语法 */
.button:hover {  /* 伪类应在局部使用 */
    background: red;
}

&:focus {  /* 局部样式特性 */
    border: 1px solid red;
}

@media (max-width: 768px) {  /* 媒体查询 */
    .container { width: 100%; }
}

style {  /* 嵌套样式块 */
    color: green;
}
```

## 📊 约束验证结果

约束器会提供详细的验证结果，包括：

- **isValid**: 是否通过验证
- **violations**: 违规详情列表（类型、消息、位置）
- **usedTemplateVariables**: 使用的模板变量
- **usedCustomVariables**: 使用的自定义变量
- **usedStyleGroups**: 使用的样式组
- **deletedProperties**: 删除的属性
- **deletedInheritances**: 删除的继承
- **inheritances**: 继承关系

## 🚀 使用方法

```cpp
#include "constraints/chtl_global_style_constraint.hpp"

using namespace chtl;

CHtlGlobalStyleConstraint constraint;
std::string styleContent = "/* CHTL全局样式内容 */";

auto result = constraint.validateGlobalStyleBlock(styleContent);

if (result.isValid) {
    std::cout << "✓ 全局样式语法有效" << std::endl;
} else {
    std::cout << "✗ 发现语法错误:" << std::endl;
    for (const auto& violation : result.violations) {
        std::cout << "  - " << violation.message 
                  << " (第" << violation.line << "行)" << std::endl;
    }
}
```

这个约束器确保CHTL全局样式块严格遵循语法规范，维护代码的一致性和安全性。