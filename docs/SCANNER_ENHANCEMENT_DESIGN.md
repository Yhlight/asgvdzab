# 扫描器增强设计文档

## 基于CHTL语法文档的关键发现

### 1. 全局style块的限制

根据CHTL语法文档第90行明确说明：
> **注意！CHTL并没有对全局样式块进行增强，请不要尝试在全局样式块使用局部样式块的功能**

这意味着：
- 全局style块中**不支持**`&`语法
- 全局style块应该是**纯CSS**
- 不应该在全局style中混入CHTL特殊语法

### 2. 变量使用方式

在style块中使用变量的正确语法是：
```chtl
style {
    color: ThemeColor(tableColor);  // 函数调用形式
    background: @Var ThemeColor.tableColor;  // 可选的@Var前缀形式
}
```

### 3. 局部style块的特性

局部style块（元素内的style）支持：
- 内联样式（直接写属性）
- 类选择器（自动添加类名）
- ID选择器（自动添加ID）
- `&`符号（上下文推导）
- 伪类/伪元素（配合`&`使用）

## 扫描器增强方案

### 1. 保持全局style块为纯CSS

```java
private void scanInCSS() {
    // 只在局部style中处理CHTL语法
    if (contextManager.getCurrentContextType() == ContextType.LOCAL_STYLE) {
        // 处理变量函数调用：VariableName(property)
        if (isVariableFunctionCall()) {
            scanVariableFunctionCall();
            return;
        }
        
        // 处理可选的@Var语法
        if (current() == '@' && matchKeywordAt(position, "@Var")) {
            scanOptionalVarPrefix();
            return;
        }
    }
    
    // 全局style保持纯CSS扫描
    handleCSSToken();
}
```

### 2. 增强CHTL JS扫描精度

对于script块，需要更精确的切割：

```java
private void scanInCHTLJS() {
    // 识别增强选择器
    if (current() == '{' && peek() == '{') {
        scanEnhancedSelector();
        return;
    }
    
    // 识别CHTL JS特殊函数
    if (isAtCHTLJSFunction()) {
        scanCHTLJSFunction();
        return;
    }
    
    // 识别->操作符
    if (current() == '-' && peek() == '>') {
        scanArrowOperator();
        return;
    }
    
    // 其他JS代码
    scanJavaScriptToken();
}
```

### 3. 最小单元切割策略

#### 3.1 CSS中的最小单元
- CSS选择器
- CSS属性名
- CSS属性值
- 变量函数调用
- CSS注释

#### 3.2 CHTL JS中的最小单元
- 增强选择器 `{{}}`
- 函数调用（listen、delegate、animate）
- 操作符（->、.）
- 参数对象
- 回调函数

#### 3.3 CHTL中的最小单元
- 元素定义
- 属性赋值
- 关键字块（text、style、script）
- 模板引用

## 实现步骤

### 第一步：修正CSS扫描器
1. 区分全局和局部style上下文
2. 局部style中识别变量函数调用
3. 保持全局style的纯CSS特性

### 第二步：增强CHTL JS扫描器
1. 实现更精确的语法边界检测
2. 识别所有CHTL JS特殊语法
3. 正确处理混合的JS和CHTL JS代码

### 第三步：优化片段合并
1. 根据语法边界合并片段
2. 保持最小语义单元
3. 优化扫描性能

## 预期效果

### 示例1：局部style中的变量使用
```chtl
div {
    style {
        color: ThemeColor(primary);  // 被识别为CHTL片段
        background: #fff;            // CSS片段
    }
}
```

片段结果：
1. `color: ` - CSS片段
2. `ThemeColor(primary)` - CHTL片段
3. `; background: #fff;` - CSS片段

### 示例2：CHTL JS精确切割
```chtl
script {
    {{.button}}.listen({  // CHTL JS片段
        click: function() {  // JS片段
            console.log('clicked');  // JS片段
        }
    });
}
```

片段结果：
1. `{{.button}}` - CHTL JS片段
2. `.listen({` - CHTL JS片段
3. `click: function() { console.log('clicked'); }` - JS片段
4. `});` - CHTL JS片段