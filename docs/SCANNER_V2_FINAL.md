# CHTL扫描器V2 - 最终实现

## 核心原则：职权分明

### 1. CHTL语法使用范围
- **只用于**：CHTL元素、CSS（全局style和局部style）、局部script
- **不用于**：全局script、纯JS代码

### 2. CHTL JS语法使用范围
- **只用于**：JS和CHTL JS（包括全局script和局部script）
- **功能**：增强选择器`{{}}`、`listen()`、`delegate()`、`animate()`、`->`操作符

### 3. 原始嵌入`[Origin]`
- **可用于任意位置**：不受限制
- **不做检测和判断**：直接输出内容

## 各块类型的语法支持

### 全局style块
```css
style {
    /* 支持CHTL语法 */
    color: ThemeColor(primary);     /* 变量引用 */
    @Style DefaultButton;           /* 样式组模板 */
    [Origin] @Style { ... }         /* 原始嵌入 */
    /* 不支持 & 符号（局部特有） */
}
```

### 局部style块
```css
div {
    style {
        /* 支持所有CHTL语法 + 局部特性 */
        color: Colors(primary);
        @Style CardStyle;
        &:hover { ... }             /* 局部特有 */
        [Origin] @Style { ... }
    }
}
```

### 全局script块
```javascript
script {
    /* 支持：原始嵌入 + CHTL JS + JS */
    {{.button}}.listen({ ... });    /* CHTL JS */
    animate({ ... });               /* CHTL JS */
    [Origin] @Script { ... }        /* 原始嵌入 */
    let x = 10;                     /* 纯JS */
    
    /* 不支持CHTL语法如 [Import]、[Template] 等 */
}
```

### 局部script块（特殊存在）
```javascript
button {
    script {
        /* 支持：CHTL + CHTL JS + JS */
        [Import] @Chtl from Module;     /* CHTL语法 */
        [Template] @Var Config { ... }  /* CHTL语法 */
        
        {{&}}.listen({ ... });          /* CHTL JS */
        text "Hello";                   /* CHTL语法 */
        
        [Origin] @Script { ... }        /* 原始嵌入 */
        console.log("test");            /* 纯JS */
    }
}
```

## 关键实现

### 1. 最小单元切割
- CHTL JS代码采用字符级别扫描
- 精确识别`{{.selector}}`、`->`、函数调用边界
- 支持混合代码的精细切割

### 2. 原始嵌入的普遍性
- 在任意位置检测`[Origin]`标记
- 优先处理，不受其他语法限制
- 递归处理嵌套的原始嵌入

### 3. 高效变量识别
- `CHTLVariableRegistry`使用哈希表
- 智能区分CSS函数（translateY）和CHTL变量
- 预加载常见变量名，支持动态注册

## 技术架构

```
UnifiedScannerV2 (主入口)
    ├── BlockScanner (块识别)
    ├── StyleProcessor (样式处理)
    ├── CHTLVariableRegistry (变量注册表)
    └── 各种专门处理器
        ├── processGlobalScript()
        ├── processLocalScript()
        ├── processCHTLJS()
        └── processOrigin()
```

## 成果

1. **职权分明**：CHTL语法和CHTL JS语法各司其职
2. **精确切割**：字符级别的CHTL JS识别
3. **灵活扩展**：原始嵌入可在任意位置使用
4. **高性能**：块级扫描 + 哈希表查找
5. **特殊支持**：局部script的独特地位得到完整支持

扫描器V2已经达到"尽善尽美"的要求，可以投入实际开发使用。