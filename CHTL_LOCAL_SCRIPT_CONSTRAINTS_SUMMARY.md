# CHTL局部Script块约束器

## 🎯 约束器作用

CHTL局部script块(script { ... })允许使用**特定的CHTL语法**，同时**保护JavaScript模板语法**不被误禁，确保script块既能利用CHTL的变量系统，又能正常使用JavaScript的所有功能。

## ✅ 允许的CHTL语法

### 1. 模板变量和自定义变量
```javascript
script {
    // 模板变量简写形式
    const primaryColor = ThemeColor(primary);
    
    // 模板变量完整形式
    const bgColor = @Var ColorPalette(secondary);
    
    // 自定义变量组特例化
    const customColor = ThemeColor(tableColor = rgb(255, 192, 203));
}
```

### 2. 命名空间from导入
```javascript
script {
    // 从命名空间导入配置
    AppConfig from config/application.chtl;
    UserData from data/users.chtl;
    
    // 使用导入的配置
    console.log('App config:', AppConfig);
}
```

### 3. 生成器注释(特殊存在)
```javascript
script {
    -- 局部script配置信息
    -- 组件特定的JavaScript逻辑
    -- 生成器注释任意地方都可以写
    
    function setupScript() {
        // JavaScript逻辑
    }
}
```

### 4. 原始嵌入(特殊存在)
```javascript
script {
    // 原始JavaScript嵌入
    [Origin] @JavaScript {
        window.localScriptConfig = {
            version: '1.0',
            features: ['variables', 'templates']
        };
    }
    
    // 原始HTML嵌入
    [Origin] @Html {
        <div id="local-script-marker"></div>
    }
    
    // 原始CSS嵌入
    [Origin] @Style {
        .local-override { display: none; }
    }
}
```

### 5. JavaScript模板语法保护
```javascript
script {
    // ✅ JavaScript模板语法（特供语法，不应被误禁）
    const userName = {{user.name}};
    const htmlContent = {{{article.content}}};
    const safeOutput = {{&message.text}};
    
    // 复杂模板表达式
    const computed = {{user.firstName + ' ' + user.lastName}};
    const conditional = {{user.isActive ? 'active' : 'inactive'}};
    
    // 在函数中使用
    function displayUser() {
        document.getElementById('name').textContent = {{user.displayName}};
        document.getElementById('avatar').src = {{user.avatar.url}};
    }
    
    // 数组和对象中使用
    const data = {
        name: {{profile.name}},
        settings: {{{profile.settingsJson}}},
        preferences: {{&profile.preferencesString}}
    };
}
```

### 6. 纯JavaScript代码
```javascript
script {
    // 标准JavaScript语法完全支持
    function initializeScript() {
        console.log('Script initialized');
        
        const config = {
            debug: true,
            features: ['template', 'variables']
        };
        
        return config;
    }
    
    // 事件监听器
    document.addEventListener('DOMContentLoaded', function() {
        initializeScript();
    });
    
    // 异步函数
    async function loadData() {
        try {
            const response = await fetch('/api/data');
            const data = await response.json();
            
            // 可以使用CHTL变量
            data.theme = ThemeColor(accent);
            
            return data;
        } catch (error) {
            console.error('Load failed:', error);
        }
    }
    
    // ES6+ 语法
    const [first, ...rest] = items;
    const {name, age} = user;
    const result = items.map(item => item.value);
}
```

## ❌ 禁止的CHTL语法

### 1. 样式相关语法
```javascript
script {
    // ❌ 禁止：样式组
    @Style DefaultButton;
    inherit @Style BaseTheme;
    delete @Style OldTheme;
    
    // ❌ 禁止：样式全缀名
    [Custom] @Style MyButton;
    [Template] @Style ButtonBase;
    
    // ❌ 禁止：样式块
    style {
        color: red;
        background: blue;
    }
}
```

### 2. 元素相关语法
```javascript
script {
    // ❌ 禁止：元素定义
    @Element CustomDiv;
    [Custom] @Element MyDiv;
    [Template] @Element BaseElement;
    
    // ❌ 禁止：文本块
    text {
        "这是文本内容"
    }
}
```

## 🔍 验证示例

### 正确的CHTL局部script块
```javascript
div {
    script {
        // ✅ 完全符合约束的局部script块
        
        -- 局部script配置
        -- 支持模板变量、自定义变量组、变量组特例化、命名空间from
        -- 生成器注释和原始嵌入是特殊存在
        -- JavaScript模板语法{{&}}等不应被误禁
        
        // CHTL变量系统
        const primaryColor = ThemeColor(primary);
        const secondaryColor = @Var ColorPalette(secondary);
        const customColor = ThemeColor(highlight = #ff6b6b);
        
        // 命名空间导入
        AppConfig from config/application.chtl;
        UserData from data/users.chtl;
        
        // JavaScript模板语法
        const userName = {{user.name}};
        const profile = {{{user.profileHtml}}};
        const message = {{&notification.text}};
        
        // 原始嵌入
        [Origin] @JavaScript {
            window.localScriptConfig = {
                version: '1.0',
                features: ['variables', 'templates']
            };
        }
        
        // 纯JavaScript逻辑
        function initializeLocalScript() {
            console.log('Initializing local script with config:', AppConfig);
            
            // 使用CHTL变量
            document.body.style.setProperty('--primary-color', primaryColor);
            document.body.style.setProperty('--secondary-color', secondaryColor);
            
            // 使用JavaScript模板
            const welcomeMsg = {{translations.welcome}};
            console.log('Welcome message:', welcomeMsg);
            
            // 事件处理
            document.addEventListener('click', function(e) {
                if (e.target.matches('.interactive')) {
                    const elementData = {{&e.target.dataset.info}};
                    processInteraction(elementData);
                }
            });
        }
        
        // 异步数据加载
        async function loadUserPreferences() {
            try {
                const response = await fetch('/api/preferences');
                const preferences = await response.json();
                
                // 应用用户偏好
                preferences.theme = primaryColor;
                preferences.locale = {{user.locale}};
                
                return preferences;
            } catch (error) {
                console.error('Failed to load preferences:', error);
                return null;
            }
        }
        
        // 初始化
        document.addEventListener('DOMContentLoaded', function() {
            initializeLocalScript();
            loadUserPreferences().then(prefs => {
                if (prefs) {
                    console.log('User preferences loaded:', prefs);
                }
            });
        });
    }
}
```

## 🎯 关键特性

### 1. JavaScript模板语法保护
- **{{variable}}**: 简单变量插值
- **{{{html}}}**: HTML内容插值
- **{{&unescaped}}**: 无转义文本插值
- **复杂表达式**: 支持JavaScript表达式和条件语句
- **优先级处理**: 模板语法优先识别，避免被CHTL约束误禁

### 2. 特殊存在的全局支持
- **生成器注释**: `--注释`任意地方都可以写
- **原始嵌入**: `[Origin] @Type { content }`任意地方都可以写
- **内容透明**: 特殊存在的内容不参与其他语法约束检查

### 3. 精准的CHTL语法支持
- **变量系统**: 支持模板变量、自定义变量、变量特例化
- **命名空间**: 支持from子句导入外部定义
- **样式隔离**: 禁止样式和元素相关CHTL语法，保持script职责单一

### 4. 智能语法区分
- **CHTL vs JavaScript**: 准确区分CHTL语法和JavaScript语法
- **模板语法识别**: 正确识别并保护`{{&}}`等JavaScript模板语法
- **优先级处理**: 特殊语法优先处理，避免语法冲突

## 📊 统计信息

局部script约束器提供详细的使用统计：
- 模板变量使用数量
- 自定义变量使用数量
- 生成器注释数量
- 原始嵌入数量
- from子句数量
- JavaScript模板语法数量
- 纯JavaScript代码行数

## 🚀 设计理念

**局部script = JavaScript环境 + 精选CHTL语法**

局部script块的设计理念是创建一个**JavaScript优先**的环境，同时提供必要的CHTL集成能力：

1. **JavaScript为主体**: 保持JavaScript的完整功能和语法
2. **CHTL变量集成**: 允许使用CHTL的变量系统进行数据绑定
3. **命名空间支持**: 支持导入外部CHTL定义
4. **模板语法保护**: 确保`{{&}}`等特供语法不被误禁
5. **职责边界清晰**: 禁止样式和元素相关操作，保持职责单一

这种设计确保了局部script块能够：
- 充分利用JavaScript的动态特性
- 无缝集成CHTL的变量系统  
- 保护JavaScript本身的模板语法功能
- 维护清晰的关注点分离

局部script让CHTL的脚本系统真正做到了"**JavaScript功能完整，CHTL集成精准**"！