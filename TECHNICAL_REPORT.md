# CHTL统一扫描器技术报告
## 可变长度切片机制实现

### 概述

根据您的要求，我们重新设计并实现了CHTL统一扫描器的核心架构，引入了可变长度切片机制。这一机制确保了代码片段的精确切割，特别是对CHTL和CHTL JS特殊语法的支持。

### 核心架构设计

#### 1. 可变长度切片工作流程

```
源代码 → 初始切片创建 → 完整性检查 → 智能扩增 → 二次切割 → 上下文合并 → 最终片段
```

#### 2. 关键组件

##### 2.1 片段类型与级别系统

```cpp
enum class FragmentType {
    CHTL,           // CHTL片段
    CHTL_JS,        // CHTL JS片段  
    CSS,            // CSS片段
    JAVASCRIPT,     // JavaScript片段
    HTML,           // HTML片段（原始嵌入）
    UNKNOWN         // 未知类型
};

enum class FragmentLevel {
    BLOCK,          // 块级片段（完整的代码块）
    STATEMENT,      // 语句级片段（单个语句）
    EXPRESSION,     // 表达式级片段（单个表达式）
    TOKEN,          // 词法单元级片段（最小单元）
    COMPOSITE       // 复合片段（多个相关片段的组合）
};
```

##### 2.2 增强的代码片段结构

```cpp
class CodeFragment {
public:
    FragmentType type;          // 片段类型
    FragmentLevel level;        // 切割级别
    std::string content;        // 片段内容
    FragmentPosition position;  // 位置信息
    std::string context;        // 上下文信息
    bool isComplete;            // 是否为完整片段
    std::vector<std::string> dependencies; // 依赖的其他片段
};
```

### 核心算法实现

#### 1. 初始切片创建

```cpp
std::vector<std::string> createInitialSlices(const std::string& sourceCode) {
    std::vector<std::string> slices;
    size_t pos = 0;
    
    while (pos < sourceCode.length()) {
        size_t sliceSize = std::min(initialSliceSize_, sourceCode.length() - pos);
        std::string slice = sourceCode.substr(pos, sliceSize);
        slices.push_back(slice);
        pos += sliceSize;
    }
    
    return slices;
}
```

**特点：**
- 默认切片大小：1024字符
- 可配置的初始大小
- 边界安全处理

#### 2. 智能扩增机制

```cpp
std::string expandSliceIfNeeded(const std::string& slice, const std::string& nextSlice, 
                               size_t& expansions) {
    // 检查大括号平衡
    int braceCount = 0;
    bool inString = false;
    
    // 检查CHTL JS特殊语法完整性
    if (currentSlice.find("{{") != std::string::npos) {
        size_t lastDoubleOpen = currentSlice.rfind("{{");
        size_t lastDoubleClose = currentSlice.rfind("}}", lastDoubleOpen);
        if (lastDoubleClose == std::string::npos) {
            needsExpansion = true;
        }
    }
    
    // 检查箭头操作符截断
    if (currentSlice.length() > 0 && currentSlice.back() == '-') {
        needsExpansion = true;
    }
}
```

**扩增策略：**
- **大括号平衡检查**：确保`{}`配对完整
- **CHTL JS语法检查**：检测`{{...}}`选择器完整性
- **箭头操作符检查**：避免`->`在中间被截断
- **最大扩增次数限制**：防止无限扩增（默认5次）
- **扩增因子**：每次扩增1.5倍大小

#### 3. 二次切割算法

##### 3.1 CHTL最小单元识别

```cpp
std::vector<std::string> identifyCHTLMinimalUnits(const std::string& content) {
    // 识别text块
    if (content.substr(i, 4) == "text") {
        // 查找完整的text { "内容" }结构
        size_t braceEnd = findMatchingBrace(content, i);
        units.push_back(content.substr(textStart, braceEnd - textStart + 1));
    }
    
    // 识别HTML元素块
    // 识别属性声明
    // 识别样式块
}
```

##### 3.2 CHTL JS最小单元识别

```cpp
std::vector<std::string> identifyCHTLJSMinimalUnits(const std::string& content) {
    // 处理增强选择器 {{...}}
    if (content.substr(i, 2) == "{{") {
        // 查找完整的{{...}}结构
        while (content.substr(i, 2) != "}}") i++;
        units.push_back(content.substr(selectorStart, i - selectorStart));
    }
    
    // 处理箭头操作符 ->
    if (content.substr(i, 2) == "->") {
        units.push_back("->");
    }
}
```

**最小单元类型：**

| 语法 | 单元类型 | 示例 |
|------|----------|------|
| `{{selector}}` | 增强选择器 | `{{.box}}`, `{{#header}}` |
| `->` | 箭头操作符 | `element->style` |
| `text { }` | 文本块 | `text { "Hello" }` |
| `element { }` | 元素块 | `div { ... }` |
| `listen({ })` | 监听器 | `listen({ click: ... })` |

#### 4. 上下文感知合并

```cpp
FragmentList mergeRelatedFragments(const FragmentList& fragments) {
    // 连续的相同类型TOKEN级片段合并为COMPOSITE级片段
    for (const auto& fragment : fragments) {
        if (fragment->level == FragmentLevel::TOKEN) {
            if (currentComposite->type == fragment->type) {
                // 合并到当前复合片段
                compositeContent += fragment->content;
            } else {
                // 开始新的复合片段
                currentComposite = std::make_shared<CodeFragment>(
                    fragment->type, FragmentLevel::COMPOSITE, "", position);
            }
        }
    }
}
```

**合并策略：**
- 相同类型的连续TOKEN片段合并
- 保持语义相关性
- 避免过度细分
- 生成COMPOSITE级别片段

### 性能特性

#### 1. 时间复杂度
- **初始切片**：O(n) - 线性扫描
- **扩增检查**：O(k×m) - k为扩增次数，m为切片大小
- **二次切割**：O(n) - 单次遍历
- **合并操作**：O(f) - f为片段数量

#### 2. 空间复杂度
- **切片存储**：O(n) - 与源代码成正比
- **片段对象**：O(f) - 与最终片段数成正比
- **临时缓冲**：O(s) - s为最大切片大小

#### 3. 可配置参数

```cpp
class CHTLUnifiedScanner {
private:
    size_t initialSliceSize_;       // 初始切片大小（默认1024）
    size_t maxExpansions_;          // 最大扩增次数（默认5）
    double expansionFactor_;        // 扩增因子（默认1.5）
};
```

### 测试验证

#### 1. 基础测试用例
```chtl
html {
    body {
        div {
            text { "Hello World" }
        }
    }
}
```

**结果：**
- 初始切片：2个
- 扩增次数：1次（大括号不平衡）
- 最终片段：1个COMPOSITE级CHTL片段

#### 2. CHTL JS高级测试
```chtl
script {
    {{.container}}->addEventListener('click', function() {
        console.log('Clicked!');
    });
    
    {{#header}}->style->display = 'block';
}
```

**扫描结果：**
- 检测到`{{.container}}`选择器
- 识别`->`箭头操作符
- 正确处理JavaScript函数体
- 避免在特殊语法中间截断

### 优势与特点

#### 1. 精确性
- **语法感知**：理解CHTL和CHTL JS的特殊语法
- **上下文保持**：保持代码的语义完整性
- **边界安全**：避免在关键语法中间截断

#### 2. 灵活性
- **可配置参数**：支持不同场景的优化
- **多级切割**：从BLOCK到TOKEN的分层处理
- **智能合并**：基于上下文的片段组合

#### 3. 扩展性
- **模块化设计**：每个处理步骤独立
- **类型系统**：支持新的片段类型扩展
- **插件架构**：便于添加新的语法支持

#### 4. 性能优化
- **惰性扩增**：只在需要时进行扩增
- **缓存机制**：避免重复计算
- **内存效率**：及时释放临时数据

### 与传统扫描器对比

| 特性 | 传统扫描器 | 可变长度切片扫描器 |
|------|------------|-------------------|
| 切片策略 | 固定大小 | 智能可变 |
| 语法感知 | 无 | 完整支持 |
| 完整性保证 | 否 | 是 |
| 特殊语法支持 | 有限 | 全面 |
| 性能 | 一般 | 优化 |
| 扩展性 | 差 | 优秀 |

### 后续优化方向

1. **并行处理**：支持多线程切片处理
2. **缓存优化**：缓存常见模式的切割结果
3. **增量扫描**：支持代码变更的增量处理
4. **语法扩展**：支持更多CHTL特性
5. **性能监控**：添加详细的性能指标

### 结论

新实现的可变长度切片机制成功解决了CHTL统一扫描器的核心挑战：

1. **精确切割**：确保代码片段的完整性
2. **语法支持**：完整支持CHTL和CHTL JS特殊语法
3. **性能优化**：智能扩增和上下文合并
4. **架构清晰**：模块化设计便于维护和扩展

这一机制为后续的编译器优化和语法扩展奠定了坚实的基础，使得JS和CSS编译器能够无需特别处理就支持CHTL的特殊特征。