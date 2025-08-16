# CHTL语法约束器指南

## 🎯 概述

CHTL约束器系统为CHTL语言提供了严格的语法边界控制，确保不同语境下只能使用合适的语法元素。这个系统帮助维护代码的清晰性和安全性。

## 📋 约束器类型

### 1. 全局样式块约束器 (CHtlGlobalStyleConstraint)

**作用域**: 全局样式块(`<style>`标签内容)

**允许的语法元素**:
- ✅ **模板变量使用**: `ThemeColor(primaryColor)`, `@Var ThemeColor(primaryColor)`
- ✅ **自定义变量使用**: `[Custom] @Var MyVar(value)`, `MyVar[specialization]`
- ✅ **自定义变量特例化**: `MyVar[dark]`, `MyVar[light]`
- ✅ **模板样式组**: `@Style DefaultButton`
- ✅ **自定义样式组**: `[Custom] @Style MyButton`
- ✅ **无值样式组**: 样式组不需要提供值的情况
- ✅ **自定义样式组特例化**: `@Style MyButton[dark]`
- ✅ **delete属性**: `delete color, background`
- ✅ **delete继承**: `delete @Style OldTheme`
- ✅ **继承**: `inherit @Style BaseTheme`
- ✅ **生成器注释**: `-- 这是生成器注释`
- ✅ **全缀名**: `ui.theme.PrimaryColor`
- ✅ **原始嵌入**: `[Origin] @Style { /* CSS代码 */ }`
- ✅ **从命名空间获取**: `ThemeColor from ui.theme`
- ✅ **基本CSS语法**: CSS选择器和属性

**禁止的语法元素**:
- ❌ **局部样式特性**: `&:hover`, `&:focus`, `@media`, `@keyframes`
- ❌ **嵌套样式块**: `style { ... }`
- ❌ **伪类/伪元素**: `:hover`, `::before`, `::after` (应在局部样式使用)

### 2. CHTL JavaScript块约束器 (CHtlJSConstraint)

**作用域**: CHTL JavaScript块(`script`标签内容)

**允许的语法元素**:
- ✅ **模板变量和自定义变量**: `ThemeColor(primaryColor)`
- ✅ **生成器注释**: `-- 这是生成器注释`
- ✅ **原始JavaScript嵌入**: `[Origin] @JavaScript { /* JS代码 */ }`
- ✅ **从命名空间获取**: `ConfigVar from ui.settings`
- ✅ **全缀名访问**: `ui.components.PrimaryButton`
- ✅ **纯JavaScript代码**: 标准JavaScript语法
- ✅ **CHTL DOM访问**: `chtl.getElementById()`, `chtl.getElementByClass()`
- ✅ **CHTL状态绑定**: 绑定到CHTL状态系统

**禁止的语法元素**:
- ❌ **危险API**: `eval()`, `Function()`, `document.write()`, `innerHTML`, `outerHTML`
- ❌ **CHTL元素定义**: `@Element`, `[Template]`, `[Custom]`
- ❌ **CHTL样式语法**: `style { }`, `@Style` (除变量引用外)
- ❌ **CHTL文本块**: `text { }`

## 🔧 使用方法

### 基本用法

```cpp
#include "constraints/chtl_global_style_constraint.hpp"
#include "constraints/chtl_js_constraint.hpp"

using namespace chtl;

// 验证全局样式块
CHtlGlobalStyleConstraint styleConstraint;
std::string globalCSS = ".container { color: ThemeColor(primary); }";
auto result = styleConstraint.validateGlobalStyleBlock(globalCSS);

if (result.isValid) {
    std::cout << "样式语法有效" << std::endl;
} else {
    for (const auto& violation : result.violations) {
        std::cout << "错误: " << violation.message << std::endl;
    }
}

// 验证JavaScript块
CHtlJSConstraint jsConstraint;
std::string jsCode = "const theme = ThemeColor(primary); chtl.getElementById('test');";
auto jsResult = jsConstraint.validateCHtlJSBlock(jsCode);

if (jsResult.isValid) {
    std::cout << "JavaScript语法有效" << std::endl;
}
```

### 高级用法

```cpp
// 提供上下文信息进行更精确的验证
std::vector<std::string> availableNamespaces = {"ui", "components", "theme"};
std::vector<std::string> availableTemplateVars = {"ThemeColor", "FontSize"};
std::vector<std::string> availableCustomVars = {"UserTheme", "CustomColor"};
std::vector<std::string> availableStyleGroups = {"DefaultButton", "CardStyle"};

auto result = styleConstraint.validateGlobalStyleBlock(
    globalCSS,
    availableNamespaces,
    availableTemplateVars,
    availableCustomVars,
    availableStyleGroups
);

// 获取使用统计
std::cout << "使用的模板变量: " << result.usedTemplateVariables.size() << std::endl;
std::cout << "使用的样式组: " << result.usedStyleGroups.size() << std::endl;
std::cout << "继承关系: " << result.inheritances.size() << std::endl;
```

## 📊 验证结果

### 全局样式块结果 (GlobalStyleConstraintResult)

```cpp
struct GlobalStyleConstraintResult {
    bool isValid;                                   // 是否通过验证
    std::vector<ConstraintViolation> violations;    // 违规列表
    std::set<std::string> usedTemplateVariables;    // 使用的模板变量
    std::set<std::string> usedCustomVariables;      // 使用的自定义变量
    std::set<std::string> usedStyleGroups;          // 使用的样式组
    std::set<std::string> usedNamespaces;           // 使用的命名空间
    std::set<std::string> deletedProperties;        // 删除的属性
    std::set<std::string> deletedInheritances;      // 删除的继承
    std::set<std::string> inheritances;             // 继承关系
};
```

### JavaScript块结果 (CHtlJSConstraintResult)

```cpp
struct CHtlJSConstraintResult {
    bool isValid;                                      // 是否通过验证
    std::vector<CHtlJSConstraintViolation> violations; // 违规列表
    std::set<std::string> usedTemplateVariables;       // 使用的模板变量
    std::set<std::string> usedCustomVariables;         // 使用的自定义变量
    std::set<std::string> usedNamespaces;              // 使用的命名空间
    std::set<std::string> accessedDOMElements;         // 访问的DOM元素
    std::set<std::string> boundStates;                 // 绑定的状态
};
```

## 🚨 约束违规类型

### 全局样式块违规

- `FORBIDDEN_ELEMENT`: 禁止的元素
- `INVALID_SYNTAX`: 无效语法
- `MISSING_NAMESPACE`: 缺失命名空间
- `INVALID_VARIABLE_REFERENCE`: 无效变量引用
- `INVALID_STYLE_GROUP_REFERENCE`: 无效样式组引用
- `MALFORMED_FROM_CLAUSE`: 格式错误的from子句
- `NESTED_STYLE_BLOCK`: 嵌套样式块
- `LOCAL_STYLE_FEATURE`: 局部样式块特性

### JavaScript块违规

- `FORBIDDEN_CHTL_ELEMENT`: 禁止的CHTL元素
- `INVALID_VARIABLE_REFERENCE`: 无效变量引用
- `MALFORMED_FROM_CLAUSE`: 格式错误的from子句
- `INVALID_DOM_ACCESS`: 无效的DOM访问
- `FORBIDDEN_API`: 禁止的API调用
- `MISSING_NAMESPACE`: 缺失命名空间

## 📝 最佳实践

### 1. 全局样式块

**推荐做法**:
```css
/* ✅ 全局样式块 - 推荐 */
.container {
    color: ThemeColor(primary);
    background: CustomColor(userBg);
}

@Style DefaultButton;
inherit @Style BaseTheme;
delete border, margin;

-- 全局样式配置注释

[Origin] @Style {
    /* 原始CSS代码 */
    * { box-sizing: border-box; }
}

ButtonStyle from ui.components;
```

**避免做法**:
```css
/* ❌ 全局样式块 - 避免 */
.button:hover {  /* 伪类应在局部使用 */
    background: red;
}

@media (max-width: 768px) {  /* 媒体查询应在局部使用 */
    .container { width: 100%; }
}

style {  /* 不允许嵌套样式块 */
    color: blue;
}
```

### 2. JavaScript块

**推荐做法**:
```javascript
// ✅ CHTL JavaScript块 - 推荐
function initApp() {
    const theme = ThemeColor(primary);
    const button = chtl.getElementById('main-button');
    
    // 安全的DOM操作
    button.textContent = 'Click me';
    button.addEventListener('click', handleClick);
    
    // 使用命名空间
    const config = AppConfig from ui.settings;
}

-- JavaScript配置注释

[Origin] @JavaScript {
    // 原始JavaScript代码
    window.chtlConfig = { version: '1.0' };
}
```

**避免做法**:
```javascript
// ❌ CHTL JavaScript块 - 避免
function dangerous() {
    eval(userInput);  // 禁止eval
    element.innerHTML = userContent;  // 危险的innerHTML
    
    // 不允许在JavaScript中定义CHTL元素
    @Element Button {
        div { text { "Invalid" } }
    }
}
```

## 🔧 扩展约束器

### 添加自定义约束

```cpp
class CustomCHtlConstraint : public CHtlGlobalStyleConstraint {
public:
    CustomCHtlConstraint() {
        // 添加自定义禁止模式
        forbiddenLocalPatterns_.insert("@import");  // 禁止@import
        forbiddenLocalPatterns_.insert("@charset"); // 禁止@charset
    }
    
    bool validateCustomSyntax(const std::string& syntax) {
        // 自定义验证逻辑
        return syntax.find("custom:") == 0;
    }
};
```

## 🎯 集成到编译器

```cpp
// 在CHTL编译器中集成约束器
class CHtlCompiler {
private:
    CHtlGlobalStyleConstraint styleConstraint_;
    CHtlJSConstraint jsConstraint_;
    
public:
    CompileResult compile(const CHtlSource& source) {
        CompileResult result;
        
        // 验证全局样式块
        for (const auto& styleBlock : source.globalStyleBlocks) {
            auto styleResult = styleConstraint_.validateGlobalStyleBlock(styleBlock.content);
            if (!styleResult.isValid) {
                for (const auto& violation : styleResult.violations) {
                    result.errors.push_back(violation.message);
                }
            }
        }
        
        // 验证JavaScript块
        for (const auto& jsBlock : source.scriptBlocks) {
            auto jsResult = jsConstraint_.validateCHtlJSBlock(jsBlock.content);
            if (!jsResult.isValid) {
                for (const auto& violation : jsResult.violations) {
                    result.errors.push_back(violation.message);
                }
            }
        }
        
        return result;
    }
};
```

## 📈 性能优化

1. **缓存编译结果**: 对相同内容的验证结果进行缓存
2. **增量验证**: 只验证修改的部分
3. **并行验证**: 同时验证多个块
4. **预编译模式**: 预先验证常用模式

## 🔍 调试约束器

```cpp
// 启用详细日志
#define CHTL_CONSTRAINT_DEBUG 1

// 获取详细违规信息
auto result = styleConstraint.validateGlobalStyleBlock(css);
for (const auto& violation : result.violations) {
    std::cout << "违规类型: " << (int)violation.type << std::endl;
    std::cout << "位置: 第" << violation.line << "行第" << violation.column << "列" << std::endl;
    std::cout << "元素: " << violation.element << std::endl;
    std::cout << "消息: " << violation.message << std::endl;
}
```

这个约束器系统为CHTL提供了强大的语法边界控制，确保了代码的安全性和一致性。通过明确定义每个语境下允许和禁止的语法元素，开发者可以更好地理解和使用CHTL语言。