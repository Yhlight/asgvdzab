# CHTL语法示例

## 增强选择器语法

### 1. 基本增强选择器（在script块中）

```chtl
div {
    class = "container";
    
    button {
        class = "my-btn";
        text { 点击我 }
    }
    
    script {
        // 使用增强选择器
        {{.my-btn}}.listen({
            click: function() {
                console.log('按钮被点击');
            }
        });
        
        // 相当于
        // document.querySelector('.my-btn').addEventListener('click', ...)
    }
}
```

### 2. 局部样式块中的 `&` 和局部脚本块中的 `{{&}}`

**重要**：`&`和`{{&}}`是两个不同的概念！

#### 局部样式块使用 `&`

```chtl
div {
    class = "card";
    
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
    
    h2 {
        class = "title";
        text { 卡片标题 }
    }
    
    p {
        text { 卡片内容 }
    }
}
```

#### 局部脚本块使用 `{{&}}`

```chtl
div {
    class = "interactive-card";
    
    script {
        {{&}}.listen({
            click: function() {
                // this 指向当前元素
                this.classList.toggle('active');
            },
            mouseenter: function() {
                console.log('鼠标进入');
            }
        });
    }
    
    p {
        text { 点击切换状态 }
    }
}
```

### 3. 模板中的用法

```chtl
[Template] @Element Card {
    div {
        class = "card-wrapper";
        
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
            {{&}}.listen({
                mouseenter: function() {
                    console.log('Mouse entered card');
                }
            });
        }
        
        slot { }
    }
}
```

### 4. CHTL JS中的unquoted literals和事件支持

```chtl
script {
    // animate函数中可以使用unquoted literals
    {{.box}}.animate({
        duration: 500,          // 数字
        easing: ease-in-out,    // unquoted literal
        fill: forwards          // unquoted literal
    });

    // listen函数 - 支持所有标准JS事件
    {{.btn}}.listen({
        click: function() { /* 点击事件 */ },
        mouseenter: function() { /* 鼠标进入 */ },
        mouseleave: function() { /* 鼠标离开 */ },
        focus: function() { /* 获得焦点 */ },
        blur: function() { /* 失去焦点 */ },
        input: function() { /* 输入事件 */ },
        change: function() { /* 改变事件 */ },
        submit: function() { /* 提交事件 */ },
        keydown: function(e) { /* 键盘按下 */ },
        keyup: function(e) { /* 键盘释放 */ },
        // ... 支持所有DOM事件
    });

    // delegate函数 - 支持DOM对象作为target
    {{.container}}.delegate({
        target: {{.item}},              // 增强选择器
        click: handleClick,
        mouseenter: handleHover
    });

    // 或使用JS表达式获取DOM对象
    {{body}}.delegate({
        target: document.querySelectorAll('.dynamic-item'),  // DOM NodeList
        click: function(e) {
            console.log('Delegated click on', e.target);
        }
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