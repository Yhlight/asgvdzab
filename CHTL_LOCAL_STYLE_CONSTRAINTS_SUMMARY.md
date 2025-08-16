# CHTL局部样式块约束器

## 🎯 约束器作用

CHTL局部样式块(style { ... })与全局样式块具有**相同的语法约束**，都允许使用完整的CHTL样式相关语法。

## ✅ 允许的CHTL语法

### 1. 模板变量和自定义变量
```chtl
style {
    // 模板变量简写形式
    color: ThemeColor(primary);
    
    // 模板变量完整形式
    background: @Var ColorPalette(secondary);
    
    // 自定义变量特例化
    border-color: ThemeColor(tableColor = rgb(255, 192, 203));
}
```

### 2. 样式组系统
```chtl
style {
    // 样式组引用
    @Style DefaultButton;
    
    // 全缀名样式组
    [Custom] @Style MyCustomButton;
    
    // 样式组继承
    inherit @Style BaseTheme;
    
    // 无值样式组（在块内定义）
    @Style LocalButton {
        padding: 8px;
        border-radius: 4px;
    }
    
    // 样式组特例化
    @Style CustomButton {
        delete margin, padding;
    }
}
```

### 3. Delete操作
```chtl
style {
    // delete属性
    delete line-height, border;
    
    // delete继承
    delete @Style OldTheme;
}
```

### 4. 继承系统
```chtl
style {
    // 显式继承
    inherit @Style BaseTheme;
    
    // 样式组引用（隐式继承）
    @Style ButtonBase;
}
```

### 5. 生成器注释
```chtl
style {
    -- 局部样式配置信息
    -- 组件特定的样式设置
    
    color: red;
}
```

### 6. 原始嵌入(官方3种类型)
```chtl
style {
    // 原始CSS嵌入
    [Origin] @Style {
        .local-override {
            transition: all 0.3s ease;
        }
    }
    
    // 原始HTML嵌入
    [Origin] @Html {
        <span class="local-indicator">Local</span>
    }
    
    // 原始JavaScript嵌入
    [Origin] @JavaScript {
        console.log('Local style applied');
    }
}
```

### 7. 命名空间导入
```chtl
style {
    // 从命名空间导入样式
    ButtonStyle from ui/components.chtl;
    IconStyle from icons/basic.chtl;
}
```

### 8. 全缀名引用
```chtl
style {
    // 模板全缀名
    [Template] @Var FontSize;
    [Template] @Style BaseTheme;
    
    // 自定义全缀名
    [Custom] @Var LocalColor;
    [Custom] @Style CustomButton;
}
```

### 9. 基本CSS语法
```chtl
style {
    // CSS属性
    font-size: 14px;
    background-color: white;
    
    // CSS选择器
    .local-button {
        padding: 10px;
        margin: 5px;
    }
    
    #local-container {
        width: 100%;
        height: auto;
    }
    
    // 伪类选择器
    &:hover {
        opacity: 0.8;
    }
}
```

## 🔍 验证示例

### 正确的CHTL局部样式块
```chtl
div {
    style {
        // ✅ 完全符合约束的局部样式块
        
        -- 局部样式配置
        -- 支持所有CHTL样式语法
        
        // 变量系统
        color: ThemeColor(primary);
        background: @Var ColorPalette(secondary);
        border-color: CustomColor(highlight = #ff6b6b);
        
        // 样式组系统
        @Style ButtonBase;
        [Custom] @Style LocalButton;
        inherit @Style Theme;
        
        // 操作系统
        delete margin, padding;
        delete @Style OldStyle;
        
        // 命名空间导入
        IconStyle from icons/basic.chtl;
        
        // 全缀名引用
        [Template] @Var FontSize;
        [Custom] @Style CustomButton;
        
        // 原始嵌入
        [Origin] @Style {
            .local-override {
                transition: all 0.3s ease;
            }
        }
        
        // 基本CSS
        .local-button {
            padding: 8px 16px;
            border-radius: 4px;
            cursor: pointer;
        }
        
        &:hover {
            opacity: 0.8;
        }
        
        font-family: Arial, sans-serif;
        line-height: 1.5;
    }
}
```

## 🎯 关键特性

### 1. 与全局样式块一致
- **相同的语法约束**: 局部样式块支持所有全局样式块的CHTL语法
- **完整的功能**: 变量、样式组、继承、delete、原始嵌入等全部可用
- **统一的规则**: 使用相同的验证逻辑和约束规则

### 2. 原始嵌入优先级
- **特殊处理**: 原始嵌入被优先识别，避免与其他语法混淆
- **内容透明**: 原始嵌入内容不参与CHTL语法验证
- **官方类型**: 只支持`@Html`、`@Style`、`@JavaScript`三种类型

### 3. 灵活的组合使用
- **多系统并用**: 可在同一局部样式块中使用变量、样式组、CSS等
- **命名空间支持**: 支持from子句导入外部样式定义
- **全缀名避免冲突**: 支持完整限定名避免命名冲突

## 📊 统计信息

局部样式约束器提供详细的使用统计：
- 模板变量使用数量
- 自定义变量使用数量  
- 样式组引用数量
- 继承关系数量
- 生成器注释数量
- 原始嵌入数量
- from子句数量
- 全缀名使用数量
- 基本CSS语句数量

## 🚀 设计理念

**局部样式块 = 全局样式块的完整语法**

局部样式块不是全局样式块的简化版本，而是具有相同能力的样式定义环境。这种设计确保了：

1. **语法一致性**: 开发者无需记忆不同的语法规则
2. **功能完整性**: 所有CHTL样式特性都可在局部使用
3. **代码复用性**: 全局和局部样式可以无缝切换
4. **维护简便性**: 统一的约束规则降低维护成本

局部样式块让CHTL的样式系统真正做到了"在任何地方都能发挥完整威力"！