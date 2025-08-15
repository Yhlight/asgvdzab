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

### 2. 自引用选择器 `{{&}}`

`{{&}}`用于local style/script块中，引用当前元素：

```chtl
<div class="card">
    style {
        {{&}} {
            background: white;
            padding: 20px;
            border-radius: 8px;
        }
        
        {{&}}:hover {
            box-shadow: 0 4px 8px rgba(0,0,0,0.1);
        }
        
        {{&}} .title {
            font-size: 24px;
            font-weight: bold;
        }
    }
    
    script {
        {{&}}.listen('click', function() {
            // this 指向当前元素
            this.classList.toggle('active');
        });
    }
    
    <h2 class="title">卡片标题</h2>
    <p>卡片内容</p>
</div>
```

### 3. 模板中的用法

```chtl
[Template] @Card {
    <div class="card-wrapper">
        style {
            {{&}} {
                border: 1px solid #ddd;
                padding: 16px;
            }
        }
        
        script {
            {{&}}.listen('mouseenter', function() {
                this.style.backgroundColor = '#f0f0f0';
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

1. `{{&}}` 只能在local style/script块中使用
2. 增强选择器主要用于script块中的DOM操作
3. CHTL JS不支持箭头函数语法
4. unquoted literals仅在特定上下文中可用（如animate的参数）