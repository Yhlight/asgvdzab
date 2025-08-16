# CHTL Script块约束器

## 🎯 约束器作用

根据您的要求，CHTL Script块(非局部script)禁止使用任何CHTL语法，**特别允许**的存在是生成器注释(`--注释`)以及原始嵌入(类型标识无用，内容原样输出)。

## ❌ 禁止的CHTL语法

### 1. 模板变量和自定义变量
```javascript
// ❌ 禁止：模板变量
const color = ThemeColor(primaryColor);

// ❌ 禁止：@Var变量引用
const size = @Var FontSize(large);

// ❌ 禁止：变量特例化
const customColor = ThemeColor(tableColor = rgb(255, 0, 0));
```

### 2. 命名空间from子句
```javascript
// ❌ 禁止：命名空间from子句
const button = ButtonStyle from ui.components;
```

### 3. 样式组和元素
```javascript
// ❌ 禁止：样式组引用
@Style DefaultButton;

// ❌ 禁止：元素定义
@Element CustomBox;

// ❌ 禁止：全缀名
[Custom] @Style MyButton;
[Template] @Var ThemeColor;
```

### 4. CHTL块语法
```javascript
// ❌ 禁止：样式块
style {
    color: red;
}

// ❌ 禁止：文本块
text {
    "Some text"
}
```

### 5. CHTL操作语法
```javascript
// ❌ 禁止：继承
inherit @Style BaseTheme;

// ❌ 禁止：delete操作
delete color, background;
delete @Style OldTheme;
```

## ✅ 允许的语法

### 1. 生成器注释
```javascript
// ✅ 允许：生成器注释
-- 这是生成器注释，会被生成器识别和处理
-- Script配置信息
-- 执行时的特殊指令
```

### 2. 原始嵌入(类型无用)
```javascript
// ✅ 允许：原始嵌入 - 类型标识无用，任何类型都可以
[Origin] @Html {
    <div id="dynamic-content"></div>
    <span class="marker">Generated</span>
}

[Origin] @Style {
    .dynamic-style { 
        color: blue; 
        animation: fadeIn 1s;
    }
}

[Origin] @JavaScript {
    window.customConfig = { version: '1.0' };
    console.log('Embedded JS executed');
}

[Origin] @Chtl {
    div { 
        text { "Dynamic CHTL content" }
        style { color: green; }
    }
}

// ✅ 原始嵌入的特殊性质：类型标识无用，内容原样输出
[Origin] @CustomType {
    任何内容都会原样输出，包括：
    - CHTL语法: @Style Button; ThemeColor(primary);
    - HTML标签: <div>content</div>
    - 任何文本: 完全按原样传递
}

[Origin] @AnyName {
    /* 类型名称完全无用，可以是任何名称 */
    这里的内容不会被CHTL处理，直接原样输出
}
```

### 3. 纯JavaScript代码
```javascript
// ✅ 允许：标准JavaScript语法
function initApp() {
    console.log('Application starting...');
    const data = {
        status: 'initialized',
        timestamp: Date.now()
    };
    return data;
}

// ✅ 允许：DOM操作
document.addEventListener('DOMContentLoaded', function() {
    const elements = document.querySelectorAll('.dynamic');
    elements.forEach(el => {
        el.addEventListener('click', handleClick);
    });
});

// ✅ 允许：异步操作
async function fetchData() {
    try {
        const response = await fetch('/api/data');
        return await response.json();
    } catch (error) {
        console.error('Fetch failed:', error);
    }
}

// ✅ 允许：类和对象
class AppManager {
    constructor() {
        this.state = new Map();
    }
    
    updateState(key, value) {
        this.state.set(key, value);
    }
}
```

## 🔍 验证示例

### 正确的CHTL Script块
```javascript
// ✅ 完全符合约束的Script块

// 生成器注释
-- Script初始化配置
-- 设置全局变量和事件监听器

// 纯JavaScript功能
function setupApp() {
    const config = {
        debug: false,
        apiUrl: '/api/v1'
    };
    
    // 设置事件监听器
    window.addEventListener('load', () => {
        console.log('App loaded');
    });
    
    return config;
}

// 原始嵌入：动态HTML
[Origin] @Html {
    <div id="app-status" class="status-indicator">
        <span>Ready</span>
    </div>
}

// 原始嵌入：相关样式
[Origin] @Style {
    .status-indicator {
        position: fixed;
        top: 10px;
        right: 10px;
        padding: 5px 10px;
        background: #4CAF50;
        color: white;
        border-radius: 4px;
    }
}

// 原始嵌入：扩展JavaScript
[Origin] @JavaScript {
    window.appUtils = {
        formatTime: (date) => date.toLocaleString(),
        debounce: (func, wait) => {
            let timeout;
            return function executedFunction(...args) {
                const later = () => {
                    clearTimeout(timeout);
                    func(...args);
                };
                clearTimeout(timeout);
                timeout = setTimeout(later, wait);
            };
        }
    };
}

// 应用初始化
const appConfig = setupApp();
console.log('App initialized with config:', appConfig);
```

### 错误的CHTL Script块
```javascript
// ❌ 违反约束的Script块

function invalidFunction() {
    // ❌ 禁止：使用CHTL变量
    const color = ThemeColor(primaryColor);
    const size = @Var FontSize(large);
    
    // ❌ 禁止：命名空间引用
    const button = ButtonStyle from ui.components;
    
    // ❌ 禁止：样式组
    @Style DefaultButton;
    
    // ❌ 禁止：CHTL块语法
    style {
        color: red;
    }
    
    // ❌ 禁止：继承和delete操作
    inherit @Style BaseTheme;
    delete color, background;
}
```

## 📊 约束验证结果

约束器会提供详细的验证结果：

- **isValid**: 是否通过验证
- **violations**: 违规详情列表
- **foundGeneratorComments**: 发现的生成器注释
- **foundRawEmbeddings**: 发现的原始嵌入
- **pureJavaScriptLines**: 纯JavaScript代码行数

## 🚀 使用方法

```cpp
#include "constraints/chtl_script_constraint.hpp"

using namespace chtl;

CHtlScriptConstraint constraint;
std::string scriptContent = "/* CHTL Script内容 */";

auto result = constraint.validateCHtlScriptBlock(scriptContent);

if (result.isValid) {
    std::cout << "✓ Script语法符合约束" << std::endl;
    std::cout << "  生成器注释: " << result.foundGeneratorComments.size() << " 个" << std::endl;
    std::cout << "  原始嵌入: " << result.foundRawEmbeddings.size() << " 个" << std::endl;
    std::cout << "  纯JavaScript行: " << result.pureJavaScriptLines << " 行" << std::endl;
} else {
    std::cout << "✗ 发现CHTL语法违规:" << std::endl;
    for (const auto& violation : result.violations) {
        std::cout << "  - " << violation.message << std::endl;
    }
}
```

## 🎯 设计理念

这个约束器确保：

1. **清晰的边界**: Script块专注于JavaScript逻辑，不混入CHTL语法
2. **灵活的扩展**: 通过原始嵌入支持任意类型的代码注入
3. **生成器集成**: 保留生成器注释以支持构建时处理
4. **纯净的环境**: 避免CHTL变量等语法在JavaScript环境中造成混淆

Script块成为纯JavaScript运行环境，同时保留了必要的CHTL构建时特性。