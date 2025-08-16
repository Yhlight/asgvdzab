# CHTL 快速入门指南

欢迎使用 CHTL！本指南将帮助您在 15 分钟内掌握 CHTL 的基础知识。

## 1. 安装 CHTL

```bash
# 验证Java版本（需要Java 17+）
java -version

# 下载并安装CHTL
# 将chtl添加到PATH后验证
chtl --version
```

## 2. Hello World

创建您的第一个CHTL文件 `hello.chtl`:

```chtl
body {
    h1 {
        text { Hello, World! }
    }
}
```

编译并运行：

```bash
chtl hello.chtl
# 生成 hello.html
```

## 3. 基础元素和属性

```chtl
div {
    id: "container";
    class: "main-content";
    
    h1 {
        text { CHTL 基础教程 }
    }
    
    p {
        text { CHTL 让 Web 开发更简单。 }
    }
    
    a {
        href: "https://chtl-lang.org";
        text { 了解更多 }
    }
}
```

## 4. 添加样式

### 内联样式

```chtl
div {
    style {
        background: #f0f0f0;
        padding: 20px;
        border-radius: 8px;
    }
    
    h2 {
        style {
            color: #333;
            margin-bottom: 10px;
        }
        text { 样式化的标题 }
    }
}
```

### 自动化类名

```chtl
div {
    style {
        // 自动生成类名
        .card {
            background: white;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            padding: 15px;
        }
    }
    
    text { 这是一个卡片 }
}
```

## 5. 使用模板

### 定义模板

```chtl
// 定义一个按钮模板
[Template] @Element Button {
    button {
        class: "btn";
        style {
            padding: 10px 20px;
            background: #007bff;
            color: white;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            
            &:hover {
                background: #0056b3;
            }
        }
        text { 点击我 }
    }
}

// 使用模板
body {
    @Element Button;
}
```

### 样式模板

```chtl
// 定义样式模板
[Template] @Style CardStyle {
    background: white;
    border: 1px solid #ddd;
    border-radius: 8px;
    padding: 20px;
    margin: 10px;
}

// 使用样式模板
div {
    style {
        @Style CardStyle;
        // 可以添加额外样式
        box-shadow: 0 2px 4px rgba(0,0,0,0.1);
    }
    text { 样式化的卡片 }
}
```

## 6. CHTL JS 基础

### 事件处理

```chtl
button {
    id: "myButton";
    text { 点击计数 }
    
    script {
        let count = 0;
        let btn = {{#myButton}};
        
        btn->listen("click", () => {
            count++;
            btn.textContent = `点击次数: ${count}`;
        });
    }
}
```

### DOM 操作

```chtl
div {
    id: "container";
    
    input {
        id: "nameInput";
        type: "text";
        placeholder: "输入您的名字";
    }
    
    button {
        id: "greetBtn";
        text { 打招呼 }
    }
    
    p {
        id: "greeting";
    }
    
    script {
        let input = {{#nameInput}};
        let btn = {{#greetBtn}};
        let greeting = {{#greeting}};
        
        btn->listen("click", () => {
            let name = input.value || "朋友";
            greeting.textContent = `你好, ${name}!`;
        });
    }
}
```

## 7. 创建一个完整的组件

```chtl
// 定义一个待办事项组件
[Template] @Element TodoItem {
    li {
        style {
            display: flex;
            align-items: center;
            padding: 10px;
            border-bottom: 1px solid #eee;
            
            &:hover {
                background: #f5f5f5;
            }
        }
        
        input {
            type: "checkbox";
            margin-right: 10px;
        }
        
        span {
            flex: 1;
            text { 待办事项 }
        }
        
        button {
            style {
                background: #dc3545;
                color: white;
                border: none;
                padding: 5px 10px;
                border-radius: 3px;
                cursor: pointer;
            }
            text { 删除 }
        }
    }
}

// 创建待办事项列表
div {
    style {
        max-width: 400px;
        margin: 0 auto;
        padding: 20px;
    }
    
    h1 {
        text { 我的待办事项 }
    }
    
    input {
        id: "todoInput";
        type: "text";
        placeholder: "添加新任务...";
        style {
            width: 100%;
            padding: 10px;
            margin-bottom: 10px;
            border: 1px solid #ddd;
            border-radius: 4px;
        }
    }
    
    ul {
        id: "todoList";
        style {
            list-style: none;
            padding: 0;
        }
        
        @Element TodoItem;
    }
    
    script {
        let input = {{#todoInput}};
        let list = {{#todoList}};
        
        // 添加新任务
        input->listen("keypress", (e) => {
            if (e.key === "Enter" && input.value.trim()) {
                addTodo(input.value.trim());
                input.value = "";
            }
        });
        
        // 使用事件委托处理删除
        list->delegate("click", "button", (e) => {
            e.target.closest("li").remove();
        });
        
        function addTodo(text) {
            let li = document.createElement("li");
            li.innerHTML = `
                <input type="checkbox">
                <span>${text}</span>
                <button>删除</button>
            `;
            list.appendChild(li);
        }
    }
}
```

## 8. 下一步

恭喜！您已经学习了 CHTL 的基础知识。接下来您可以：

1. **阅读完整文档**: 查看 [用户手册](UserGuide.md) 了解更多高级特性
2. **探索示例项目**: 学习实际项目的最佳实践
3. **加入社区**: 获取帮助和分享经验

### 有用的命令

```bash
# 开发模式（带调试信息）
chtl --development myapp.chtl

# 生产构建（优化输出）
chtl --production -o dist/index.html myapp.chtl

# 监视文件变化
chtl --watch myapp.chtl

# 查看帮助
chtl --help
```

### 提示

- 使用模板来复用代码
- 局部样式可以避免全局污染
- CHTL JS 的 `->` 语法让事件处理更直观
- 善用命名空间组织大型项目

现在开始用 CHTL 构建您的下一个 Web 项目吧！