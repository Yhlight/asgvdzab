# CHTL语法示例

## 增强选择器语法

### 1. 基本增强选择器（在script块中）

```chtl
<div class="container">
    <button class="my-btn">点击我</button>
    
    script {
        // 使用增强选择器
        {{.my-btn}}.listen('click', function() {
            console.log('按钮被点击');
        });
        
        // 相当于
        // document.querySelector('.my-btn').addEventListener('click', ...)
    }
</div>
```

### 2. 局部样式块中的 `&` 和局部脚本块中的 `{{&}}`

**重要**：`&`和`{{&}}`是两个不同的概念！

#### 局部样式块使用 `&`

```chtl
<div class="card">
    style {
        & {
            background: white;
            padding: 20px;
            border-radius: 8px;
        }
        
        &:hover {
            box-shadow: 0 4px 8px rgba(0,0,0,0.1);
        }
        
        & .title {
            font-size: 24px;
            font-weight: bold;
        }
    }
    
    <h2 class="title">卡片标题</h2>
    <p>卡片内容</p>
</div>
```

#### 局部脚本块使用 `{{&}}`

```chtl
<div class="interactive-card">
    script {
        {{&}}.listen('click', function() {
            // this 指向当前元素
            this.classList.toggle('active');
        });
        
        {{&}}.listen('mouseenter', function() {
            console.log('鼠标进入');
        });
    }
    
    <p>点击切换状态</p>
</div>
```

### 3. 模板中的用法

```chtl
[Template] @Card {
    <div class="card-wrapper">
        style {
            & {
                border: 1px solid #ddd;
                padding: 16px;
            }
            
            &:hover {
                background-color: #f0f0f0;
            }
        }
        
        script {
            {{&}}.listen('mouseenter', function() {
                console.log('Mouse entered card');
            });
        }
        
        <slot/>
    </div>
}
```

### 4. CHTL JS中的unquoted literals

```chtl
script {
    // animate函数中可以使用unquoted literals
    {{.box}}.animate({
        duration: 500,          // 数字
        easing: ease-in-out,    // unquoted literal
        fill: forwards          // unquoted literal
    });
    
    // listen函数
    {{.btn}}.listen('click', function() {
        // 事件处理
    });
    
    // delegate函数
    {{.container}}.delegate('.item', 'click', function() {
        // 委托事件处理
    });
}
```

## 注意事项

1. **局部样式块使用 `&`**，表示当前元素的CSS选择器
2. **局部脚本块使用 `{{&}}`**，表示当前元素的增强选择器
3. 增强选择器主要用于script块中的DOM操作
4. CHTL JS不支持箭头函数语法
5. unquoted literals仅在特定上下文中可用（如animate的参数）
6. CHTL JS函数参数是无序的，可以任意组合使用