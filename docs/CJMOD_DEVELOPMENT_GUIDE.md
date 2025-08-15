# CJMOD开发指南

## 概述

CJMOD（CHTL JavaScript Module）是CHTL提供的扩展CHTL JS语法的方式，允许开发者通过Java接口自由实现CHTL JS的扩展功能。

## 核心特性

### 1. 极高的自由度
- 自由引入官方头文件
- 编写和管理全局状态
- 无限扩展JS语法的可能性

### 2. 优良的接口设计
- 简洁的`CJmodInterface`接口
- 灵活的`CJmodContext`上下文
- 强大的`CJmodGlobalStateManager`

### 3. 高级功能
- 函数调用缓存
- JIT编译优化
- 内存池管理
- 错误优雅降级
- 模块崩溃恢复

## 开发流程

### 1. 创建CJMOD项目结构

```
MyModule/
├── src/
│   ├── MyModule.java       # 主模块类
│   └── functions/          # 功能函数
│       ├── CustomFunction1.java
│       └── CustomFunction2.java
├── info/
│   └── MyModule.chtl       # 模块信息
└── README.md               # 模块文档
```

### 2. 实现主模块类

```java
package com.mymodule;

import com.chtl.cjmod.CJmodInterface;
import com.chtl.cjmod.CJmodContext;
import com.chtl.cjmod.CJmodFunction;
import java.util.*;

public class MyModule implements CJmodInterface {
    
    private final Map<String, CJmodFunction> functions = new HashMap<>();
    
    @Override
    public String getName() {
        return "MyModule";
    }
    
    @Override
    public String getVersion() {
        return "1.0.0";
    }
    
    @Override
    public void initialize(CJmodContext context) {
        // 注册函数
        registerFunction("myFunction", new MyFunction());
        registerFunction("anotherFunction", new AnotherFunction());
        
        // 初始化全局状态
        context.getGlobalState().set("moduleConfig", loadConfig());
    }
    
    @Override
    public Map<String, CJmodFunction> getFunctions() {
        return Collections.unmodifiableMap(functions);
    }
    
    @Override
    public void shutdown() {
        // 清理资源
        saveState();
        functions.clear();
    }
    
    private void registerFunction(String name, CJmodFunction function) {
        functions.put(name, function);
    }
}
```

### 3. 实现功能函数

```java
package com.mymodule.functions;

import com.chtl.cjmod.*;
import java.util.Map;

public class MyFunction implements CJmodFunction {
    
    @Override
    public String getName() {
        return "myFunction";
    }
    
    @Override
    public String getDescription() {
        return "自定义功能函数";
    }
    
    @Override
    public Object execute(Map<String, Object> params, CJmodContext context) {
        // 获取参数
        String target = (String) params.get("target");
        int duration = (int) params.getOrDefault("duration", 300);
        
        // 访问全局状态
        GlobalState state = context.getGlobalState();
        Config config = (Config) state.get("moduleConfig");
        
        // 执行功能
        try {
            return performCustomOperation(target, duration, config);
        } catch (Exception e) {
            // 错误处理
            context.getErrorHandler().handleError(e);
            return null;
        }
    }
    
    @Override
    public boolean isCacheable() {
        return true; // 允许缓存
    }
}
```

### 4. 编写模块信息

```chtl
// info/MyModule.chtl
[Info] {
    name = "MyModule";
    version = "1.0.0";
    description = "自定义CJMOD模块";
    author = "Your Name";
    license = "MIT";
    dependencies = "";
    category = "extension";
    minCHTLVersion = "1.0.0";
    maxCHTLVersion = "2.0.0";
}

// 注意：CJMOD不使用[Export]块
```

## 高级特性

### 1. 全局状态管理

```java
// 存储状态
context.getGlobalState().set("counter", new AtomicInteger(0));

// 获取状态
AtomicInteger counter = (AtomicInteger) context.getGlobalState().get("counter");

// 原子操作
context.getGlobalState().compute("counter", (k, v) -> {
    AtomicInteger c = (AtomicInteger) v;
    c.incrementAndGet();
    return c;
});
```

### 2. 内存池管理

```java
// 获取内存池
MemoryPoolManager pool = context.getMemoryPool();

// 分配缓冲区
ByteBuffer buffer = pool.allocateBuffer(1024);
try {
    // 使用缓冲区
    processData(buffer);
} finally {
    // 释放缓冲区
    pool.releaseBuffer(buffer);
}

// 对象池
ObjectPool<MyObject> objectPool = pool.getObjectPool(MyObject.class);
MyObject obj = objectPool.borrow();
try {
    // 使用对象
    obj.process();
} finally {
    objectPool.release(obj);
}
```

### 3. 错误处理

```java
@Override
public Object execute(Map<String, Object> params, CJmodContext context) {
    try {
        return riskyOperation();
    } catch (Exception e) {
        // 记录错误
        context.getErrorHandler().handleError(e);
        
        // 优雅降级
        return context.getErrorHandler().gracefulFallback(() -> {
            return defaultValue();
        });
    }
}
```

### 4. 性能优化

```java
public class OptimizedFunction implements CJmodFunction {
    
    // 启用缓存
    @Override
    public boolean isCacheable() {
        return true;
    }
    
    // 自定义缓存键
    @Override
    public String getCacheKey(Map<String, Object> params) {
        return params.get("id") + ":" + params.get("type");
    }
    
    // JIT优化提示
    @Override
    public Map<String, Object> getOptimizationHints() {
        Map<String, Object> hints = new HashMap<>();
        hints.put("hotPath", true);
        hints.put("parameterPattern", "fixed");
        return hints;
    }
}
```

## 模块打包

### 1. 编译Java代码

```bash
javac -cp chtl-cjmod-api.jar src/**/*.java -d build/
```

### 2. 创建JAR包

```bash
jar cf mymodule.jar -C build/ . -C info/ .
```

### 3. 使用打包脚本

```bash
# Linux/Mac
./scripts/linux/package-cjmod.sh MyModule

# Windows
scripts\windows\package-cjmod.bat MyModule
```

## 测试与调试

### 1. 单元测试

```java
import org.junit.Test;
import static org.junit.Assert.*;

public class MyFunctionTest {
    
    @Test
    public void testBasicFunctionality() {
        MyFunction func = new MyFunction();
        Map<String, Object> params = new HashMap<>();
        params.put("target", ".test");
        params.put("duration", 500);
        
        MockContext context = new MockContext();
        Object result = func.execute(params, context);
        
        assertNotNull(result);
    }
}
```

### 2. 集成测试

```chtl
// test.chtl
[Import] @CJmod from MyModule;

script {
    // 测试自定义函数
    myFunction({
        target: {{.element}},
        duration: 300
    });
}
```

## 最佳实践

### 1. 参数验证

```java
@Override
public Object execute(Map<String, Object> params, CJmodContext context) {
    // 验证必需参数
    if (!params.containsKey("target")) {
        throw new IllegalArgumentException("Missing required parameter: target");
    }
    
    // 类型检查
    Object target = params.get("target");
    if (!(target instanceof String || target instanceof DOMElement)) {
        throw new IllegalArgumentException("Invalid target type");
    }
    
    // 继续执行...
}
```

### 2. 资源管理

```java
public class ResourceAwareFunction implements CJmodFunction {
    private final ExecutorService executor;
    
    public ResourceAwareFunction() {
        this.executor = Executors.newCachedThreadPool();
    }
    
    @Override
    public void cleanup() {
        executor.shutdown();
        try {
            if (!executor.awaitTermination(5, TimeUnit.SECONDS)) {
                executor.shutdownNow();
            }
        } catch (InterruptedException e) {
            executor.shutdownNow();
        }
    }
}
```

### 3. 线程安全

```java
public class ThreadSafeFunction implements CJmodFunction {
    private final ConcurrentHashMap<String, Object> cache = new ConcurrentHashMap<>();
    private final ReadWriteLock lock = new ReentrantReadWriteLock();
    
    @Override
    public Object execute(Map<String, Object> params, CJmodContext context) {
        String key = generateKey(params);
        
        // 尝试从缓存读取
        lock.readLock().lock();
        try {
            Object cached = cache.get(key);
            if (cached != null) {
                return cached;
            }
        } finally {
            lock.readLock().unlock();
        }
        
        // 计算并缓存结果
        lock.writeLock().lock();
        try {
            // 双重检查
            Object cached = cache.get(key);
            if (cached != null) {
                return cached;
            }
            
            Object result = computeResult(params);
            cache.put(key, result);
            return result;
        } finally {
            lock.writeLock().unlock();
        }
    }
}
```

## 发布与分发

### 1. 模块签名

```bash
# 生成密钥对
./scripts/generate-keys.sh

# 签名模块
./scripts/sign-module.sh mymodule.jar
```

### 2. 发布到仓库

```bash
# 发布到官方仓库
chtl publish mymodule.jar

# 发布到私有仓库
chtl publish mymodule.jar --repo https://my-repo.com
```

### 3. 版本管理

遵循语义化版本规范：
- 主版本号：不兼容的API修改
- 次版本号：向下兼容的功能性新增
- 修订号：向下兼容的问题修正

## 示例：Chtholly模块

官方Chtholly模块展示了CJMOD的最佳实践：

```java
// PrintMylove函数实现
public class PrintMylove implements CJmodFunction {
    
    @Override
    public Object execute(Map<String, Object> params, CJmodContext context) {
        String url = (String) params.get("url");
        String mode = (String) params.getOrDefault("mode", "ASCII");
        String width = (String) params.get("width");
        String height = (String) params.get("height");
        double scale = (Double) params.getOrDefault("scale", 1.0);
        
        // 处理图片并生成字符
        BufferedImage image = loadImage(url);
        String result = convertToCharacters(image, mode, width, height, scale);
        
        return result;
    }
}
```

## 故障排除

### 常见问题

1. **模块加载失败**
   - 检查JAR包结构
   - 验证info文件格式
   - 确认依赖版本

2. **函数执行错误**
   - 启用调试日志
   - 检查参数类型
   - 验证上下文状态

3. **性能问题**
   - 分析热点函数
   - 启用缓存
   - 优化内存使用

## 总结

CJMOD提供了极大的自由度和强大的功能，让开发者能够：
- 自由扩展CHTL JS语法
- 管理复杂的全局状态
- 实现高性能的自定义功能
- 优雅处理错误和异常

遵循本指南的最佳实践，您可以开发出稳定、高效、易维护的CJMOD模块。

## 实际功能开发示例

### 示例1：实现一个拖拽功能（drag）

这个示例展示如何实现一个完整的拖拽功能，让元素可以被拖动。

#### 1. 创建项目结构

```
DragModule/
├── src/
│   ├── DragModule.java
│   └── functions/
│       └── DragFunction.java
└── info/
    └── DragModule.chtl
```

#### 2. 实现主模块

```java
// src/DragModule.java
package com.chtl.modules.drag;

import com.chtl.cjmod.*;
import java.util.*;

public class DragModule implements CJmodInterface {
    
    private final Map<String, CJmodFunction> functions = new HashMap<>();
    private final DragStateManager stateManager = new DragStateManager();
    
    @Override
    public String getName() {
        return "DragModule";
    }
    
    @Override
    public String getVersion() {
        return "1.0.0";
    }
    
    @Override
    public void initialize(CJmodContext context) {
        // 注册拖拽函数
        functions.put("drag", new DragFunction(stateManager));
        
        // 初始化全局状态
        context.getGlobalState().set("dragState", stateManager);
        
        // 注入必要的CSS
        injectStyles(context);
    }
    
    private void injectStyles(CJmodContext context) {
        String styles = """
            .draggable {
                cursor: move;
                position: relative;
            }
            .dragging {
                opacity: 0.5;
                z-index: 1000;
            }
            """;
        context.injectCSS(styles);
    }
    
    @Override
    public Map<String, CJmodFunction> getFunctions() {
        return Collections.unmodifiableMap(functions);
    }
}
```

#### 3. 实现拖拽函数

```java
// src/functions/DragFunction.java
package com.chtl.modules.drag.functions;

import com.chtl.cjmod.*;
import java.util.*;

public class DragFunction implements CJmodFunction {
    
    private final DragStateManager stateManager;
    
    public DragFunction(DragStateManager stateManager) {
        this.stateManager = stateManager;
    }
    
    @Override
    public String getName() {
        return "drag";
    }
    
    @Override
    public String getDescription() {
        return "使元素可拖拽";
    }
    
    @Override
    public Object execute(Map<String, Object> params, CJmodContext context) {
        // 获取参数
        Object target = params.get("target");
        Map<String, Object> options = (Map<String, Object>) params.getOrDefault("options", new HashMap<>());
        
        // 配置选项
        boolean constrainToParent = (boolean) options.getOrDefault("constrainToParent", false);
        String handle = (String) options.getOrDefault("handle", null);
        Map<String, Object> callbacks = (Map<String, Object>) options.getOrDefault("on", new HashMap<>());
        
        // 生成JavaScript代码
        String dragId = "drag_" + System.currentTimeMillis();
        String jsCode = generateDragCode(dragId, target, constrainToParent, handle, callbacks);
        
        // 注入JavaScript
        context.injectJS(jsCode);
        
        // 返回控制对象
        return createControlObject(dragId, context);
    }
    
    private String generateDragCode(String dragId, Object target, 
                                  boolean constrainToParent, String handle, 
                                  Map<String, Object> callbacks) {
        return String.format("""
            (function() {
                const element = %s;
                const handle = %s || element;
                let isDragging = false;
                let startX, startY, initialX, initialY;
                
                // 添加样式
                element.classList.add('draggable');
                
                // 开始拖拽
                handle.addEventListener('mousedown', function(e) {
                    isDragging = true;
                    startX = e.clientX;
                    startY = e.clientY;
                    
                    const rect = element.getBoundingClientRect();
                    initialX = element.offsetLeft;
                    initialY = element.offsetTop;
                    
                    element.classList.add('dragging');
                    
                    // 触发开始回调
                    %s
                });
                
                // 拖拽中
                document.addEventListener('mousemove', function(e) {
                    if (!isDragging) return;
                    
                    e.preventDefault();
                    
                    let newX = initialX + (e.clientX - startX);
                    let newY = initialY + (e.clientY - startY);
                    
                    // 约束到父元素
                    if (%s) {
                        const parent = element.parentElement;
                        const parentRect = parent.getBoundingClientRect();
                        const elementRect = element.getBoundingClientRect();
                        
                        newX = Math.max(0, Math.min(newX, parent.offsetWidth - element.offsetWidth));
                        newY = Math.max(0, Math.min(newY, parent.offsetHeight - element.offsetHeight));
                    }
                    
                    element.style.left = newX + 'px';
                    element.style.top = newY + 'px';
                    
                    // 触发拖拽回调
                    %s
                });
                
                // 结束拖拽
                document.addEventListener('mouseup', function() {
                    if (!isDragging) return;
                    
                    isDragging = false;
                    element.classList.remove('dragging');
                    
                    // 触发结束回调
                    %s
                });
                
                // 存储控制对象
                window['%s'] = {
                    enable: () => handle.style.pointerEvents = 'auto',
                    disable: () => handle.style.pointerEvents = 'none',
                    reset: () => {
                        element.style.left = '0px';
                        element.style.top = '0px';
                    }
                };
            })();
            """,
            convertTarget(target),
            handle != null ? convertTarget(handle) : "null",
            generateCallback(callbacks.get("start")),
            constrainToParent,
            generateCallback(callbacks.get("drag")),
            generateCallback(callbacks.get("end")),
            dragId
        );
    }
    
    private String convertTarget(Object target) {
        if (target instanceof String) {
            return String.format("document.querySelector('%s')", target);
        }
        // 假设是DOM元素引用
        return target.toString();
    }
    
    private String generateCallback(Object callback) {
        if (callback == null) return "";
        return String.format("if (typeof %s === 'function') { %s(element, {x: newX, y: newY}); }", 
            callback, callback);
    }
    
    private Map<String, Object> createControlObject(String dragId, CJmodContext context) {
        Map<String, Object> control = new HashMap<>();
        control.put("enable", new ControlMethod(dragId, "enable", context));
        control.put("disable", new ControlMethod(dragId, "disable", context));
        control.put("reset", new ControlMethod(dragId, "reset", context));
        return control;
    }
}
```

#### 4. 使用示例

```chtl
// 使用拖拽功能
[Import] @CJmod from DragModule;

div {
    class: "container";
    
    div {
        id: "draggable-box";
        class: "box";
        text { "拖动我！" }
    }
    
    script {
        // 简单使用
        drag({
            target: {{#draggable-box}}
        });
        
        // 高级使用
        let controller = drag({
            target: {{#draggable-box}},
            options: {
                constrainToParent: true,
                handle: {{.drag-handle}},
                on: {
                    start: (element, pos) => {
                        console.log('开始拖拽', pos);
                    },
                    drag: (element, pos) => {
                        console.log('拖拽中', pos);
                    },
                    end: (element, pos) => {
                        console.log('结束拖拽', pos);
                    }
                }
            }
        });
        
        // 使用控制方法
        {{.disable-btn}}.listen({
            click: () => controller.disable()
        });
    }
}
```

### 示例2：实现一个表单验证功能（validate）

这个示例展示如何实现表单验证功能。

#### 1. 项目结构

```
ValidateModule/
├── src/
│   ├── ValidateModule.java
│   ├── functions/
│   │   └── ValidateFunction.java
│   └── validators/
│       ├── EmailValidator.java
│       ├── PhoneValidator.java
│       └── CustomValidator.java
└── info/
    └── ValidateModule.chtl
```

#### 2. 实现验证函数

```java
// src/functions/ValidateFunction.java
package com.chtl.modules.validate.functions;

import com.chtl.cjmod.*;
import java.util.*;

public class ValidateFunction implements CJmodFunction {
    
    private final Map<String, Validator> validators = new HashMap<>();
    
    public ValidateFunction() {
        // 注册内置验证器
        validators.put("email", new EmailValidator());
        validators.put("phone", new PhoneValidator());
        validators.put("required", new RequiredValidator());
        validators.put("minLength", new MinLengthValidator());
        validators.put("maxLength", new MaxLengthValidator());
        validators.put("pattern", new PatternValidator());
    }
    
    @Override
    public Object execute(Map<String, Object> params, CJmodContext context) {
        Object target = params.get("target");
        Map<String, Object> rules = (Map<String, Object>) params.get("rules");
        Map<String, Object> messages = (Map<String, Object>) params.getOrDefault("messages", new HashMap<>());
        Map<String, Object> options = (Map<String, Object>) params.getOrDefault("options", new HashMap<>());
        
        // 生成验证代码
        String validateId = "validate_" + System.currentTimeMillis();
        String jsCode = generateValidationCode(validateId, target, rules, messages, options);
        
        // 注入代码
        context.injectJS(jsCode);
        
        // 注入样式
        injectValidationStyles(context);
        
        // 返回验证控制对象
        return createValidationControl(validateId);
    }
    
    private String generateValidationCode(String validateId, Object target,
                                        Map<String, Object> rules,
                                        Map<String, Object> messages,
                                        Map<String, Object> options) {
        StringBuilder js = new StringBuilder();
        js.append("(function() {\n");
        js.append("  const form = ").append(convertTarget(target)).append(";\n");
        js.append("  const validators = {};\n");
        
        // 生成验证规则
        for (Map.Entry<String, Object> rule : rules.entrySet()) {
            String field = rule.getKey();
            Map<String, Object> fieldRules = (Map<String, Object>) rule.getValue();
            
            js.append(String.format("  validators['%s'] = {\n", field));
            for (Map.Entry<String, Object> fieldRule : fieldRules.entrySet()) {
                String ruleName = fieldRule.getKey();
                Object ruleValue = fieldRule.getValue();
                
                js.append(String.format("    %s: %s,\n", ruleName, 
                    generateValidatorFunction(ruleName, ruleValue, messages)));
            }
            js.append("  };\n");
        }
        
        // 实时验证
        if ((boolean) options.getOrDefault("realtime", true)) {
            js.append("""
                form.addEventListener('input', function(e) {
                    if (e.target.name && validators[e.target.name]) {
                        validateField(e.target);
                    }
                });
                """);
        }
        
        // 验证函数
        js.append("""
            function validateField(field) {
                const rules = validators[field.name];
                const value = field.value;
                let isValid = true;
                let errorMessage = '';
                
                for (const [ruleName, validator] of Object.entries(rules)) {
                    const result = validator(value, field);
                    if (!result.valid) {
                        isValid = false;
                        errorMessage = result.message;
                        break;
                    }
                }
                
                // 显示/隐藏错误
                showFieldError(field, isValid, errorMessage);
                return isValid;
            }
            
            function showFieldError(field, isValid, message) {
                const errorEl = field.parentElement.querySelector('.error-message');
                
                if (isValid) {
                    field.classList.remove('invalid');
                    field.classList.add('valid');
                    if (errorEl) errorEl.style.display = 'none';
                } else {
                    field.classList.remove('valid');
                    field.classList.add('invalid');
                    if (errorEl) {
                        errorEl.textContent = message;
                        errorEl.style.display = 'block';
                    }
                }
            }
            """);
        
        // 全表单验证
        js.append(String.format("""
            window['%s'] = {
                validate: function() {
                    let isValid = true;
                    const errors = {};
                    
                    for (const fieldName in validators) {
                        const field = form.querySelector(`[name="${fieldName}"]`);
                        if (field && !validateField(field)) {
                            isValid = false;
                            errors[fieldName] = field.parentElement.querySelector('.error-message').textContent;
                        }
                    }
                    
                    return { valid: isValid, errors: errors };
                },
                reset: function() {
                    form.querySelectorAll('.invalid, .valid').forEach(el => {
                        el.classList.remove('invalid', 'valid');
                    });
                    form.querySelectorAll('.error-message').forEach(el => {
                        el.style.display = 'none';
                    });
                }
            };
            """, validateId));
        
        js.append("})();\n");
        return js.toString();
    }
    
    private void injectValidationStyles(CJmodContext context) {
        context.injectCSS("""
            .invalid {
                border-color: #dc3545 !important;
            }
            .valid {
                border-color: #28a745 !important;
            }
            .error-message {
                color: #dc3545;
                font-size: 0.875em;
                margin-top: 0.25rem;
                display: none;
            }
            """);
    }
}
```

#### 3. 使用示例

```chtl
[Import] @CJmod from ValidateModule;

form {
    id: "user-form";
    
    div {
        class: "form-group";
        
        label { text { "邮箱" } }
        input {
            type: "email";
            name: "email";
            placeholder: "请输入邮箱";
        }
        span { class: "error-message"; }
    }
    
    div {
        class: "form-group";
        
        label { text { "手机号" } }
        input {
            type: "tel";
            name: "phone";
            placeholder: "请输入手机号";
        }
        span { class: "error-message"; }
    }
    
    button {
        type: "submit";
        text { "提交" }
    }
    
    script {
        // 初始化验证
        let validator = validate({
            target: {{#user-form}},
            rules: {
                email: {
                    required: true,
                    email: true
                },
                phone: {
                    required: true,
                    pattern: /^1[3-9]\d{9}$/
                }
            },
            messages: {
                email: {
                    required: "请输入邮箱",
                    email: "请输入正确的邮箱格式"
                },
                phone: {
                    required: "请输入手机号",
                    pattern: "请输入正确的手机号"
                }
            },
            options: {
                realtime: true
            }
        });
        
        // 表单提交时验证
        {{#user-form}}.listen({
            submit: (e) => {
                e.preventDefault();
                
                const result = validator.validate();
                if (result.valid) {
                    console.log('表单验证通过');
                } else {
                    console.log('验证失败', result.errors);
                }
            }
        });
    }
}
```

### 示例3：实现一个虚拟滚动功能（virtualScroll）

用于处理大量数据的高性能滚动。

```java
// src/functions/VirtualScrollFunction.java
public class VirtualScrollFunction implements CJmodFunction {
    
    @Override
    public Object execute(Map<String, Object> params, CJmodContext context) {
        Object container = params.get("container");
        List<?> items = (List<?>) params.get("items");
        int itemHeight = (int) params.getOrDefault("itemHeight", 50);
        Function<Object, String> renderer = (Function<Object, String>) params.get("renderer");
        
        String vsId = "vs_" + System.currentTimeMillis();
        
        // 生成虚拟滚动代码
        String jsCode = String.format("""
            (function() {
                const container = %s;
                const items = %s;
                const itemHeight = %d;
                const renderer = %s;
                
                // 计算可见区域
                const containerHeight = container.offsetHeight;
                const visibleCount = Math.ceil(containerHeight / itemHeight);
                const bufferCount = 5;
                
                // 创建内容容器
                const content = document.createElement('div');
                content.style.height = (items.length * itemHeight) + 'px';
                content.style.position = 'relative';
                
                // 创建可见项容器
                const visibleContainer = document.createElement('div');
                visibleContainer.style.position = 'absolute';
                visibleContainer.style.top = '0';
                visibleContainer.style.left = '0';
                visibleContainer.style.right = '0';
                
                content.appendChild(visibleContainer);
                container.appendChild(content);
                
                // 渲染函数
                function renderVisibleItems() {
                    const scrollTop = container.scrollTop;
                    const startIndex = Math.floor(scrollTop / itemHeight);
                    const endIndex = Math.min(
                        startIndex + visibleCount + bufferCount * 2,
                        items.length
                    );
                    
                    // 清空并重新渲染
                    visibleContainer.innerHTML = '';
                    visibleContainer.style.transform = `translateY(${startIndex * itemHeight}px)`;
                    
                    for (let i = startIndex; i < endIndex; i++) {
                        const itemEl = document.createElement('div');
                        itemEl.style.height = itemHeight + 'px';
                        itemEl.innerHTML = renderer(items[i], i);
                        visibleContainer.appendChild(itemEl);
                    }
                }
                
                // 监听滚动
                container.addEventListener('scroll', renderVisibleItems);
                
                // 初始渲染
                renderVisibleItems();
                
                // 返回控制对象
                window['%s'] = {
                    refresh: renderVisibleItems,
                    scrollTo: (index) => {
                        container.scrollTop = index * itemHeight;
                    },
                    update: (newItems) => {
                        items = newItems;
                        content.style.height = (items.length * itemHeight) + 'px';
                        renderVisibleItems();
                    }
                };
            })();
            """,
            convertTarget(container),
            context.toJSON(items),
            itemHeight,
            renderer.toString(),
            vsId
        );
        
        context.injectJS(jsCode);
        
        return createControl(vsId);
    }
}
```

使用示例：

```chtl
script {
    // 生成大量数据
    const items = Array.from({length: 10000}, (_, i) => ({
        id: i,
        name: `Item ${i}`,
        value: Math.random()
    }));
    
    // 初始化虚拟滚动
    const scroller = virtualScroll({
        container: {{.scroll-container}},
        items: items,
        itemHeight: 60,
        renderer: (item, index) => `
            <div class="item">
                <span class="index">${index}</span>
                <span class="name">${item.name}</span>
                <span class="value">${item.value.toFixed(2)}</span>
            </div>
        `
    });
    
    // 滚动到指定位置
    {{.jump-btn}}.listen({
        click: () => scroller.scrollTo(5000)
    });
}
```

## 开发技巧和最佳实践

### 1. 性能优化技巧

```java
// 使用缓存提高性能
public class CachedFunction implements CJmodFunction {
    private final LoadingCache<String, Object> cache;
    
    public CachedFunction() {
        this.cache = Caffeine.newBuilder()
            .maximumSize(1000)
            .expireAfterWrite(5, TimeUnit.MINUTES)
            .build(this::computeExpensiveOperation);
    }
    
    @Override
    public Object execute(Map<String, Object> params, CJmodContext context) {
        String cacheKey = generateCacheKey(params);
        return cache.get(cacheKey);
    }
}
```

### 2. 错误处理模式

```java
@Override
public Object execute(Map<String, Object> params, CJmodContext context) {
    try {
        // 参数验证
        validateParams(params);
        
        // 执行操作
        return performOperation(params, context);
        
    } catch (ValidationException e) {
        // 参数验证失败
        context.logError("参数验证失败: " + e.getMessage());
        return createErrorResponse("INVALID_PARAMS", e.getMessage());
        
    } catch (Exception e) {
        // 未知错误，优雅降级
        context.logError("执行失败", e);
        return context.getErrorHandler().gracefulFallback(() -> {
            // 返回默认值或空操作
            return createEmptyResponse();
        });
    }
}
```

### 3. 异步操作处理

```java
public class AsyncFunction implements CJmodFunction {
    
    @Override
    public Object execute(Map<String, Object> params, CJmodContext context) {
        String asyncId = "async_" + UUID.randomUUID();
        
        // 立即返回Promise-like对象
        CompletableFuture<Object> future = CompletableFuture.supplyAsync(() -> {
            // 异步操作
            return performAsyncOperation(params);
        });
        
        // 注册回调
        future.thenAccept(result -> {
            // 通过事件通知结果
            context.emit(asyncId + "_complete", result);
        }).exceptionally(error -> {
            context.emit(asyncId + "_error", error.getMessage());
            return null;
        });
        
        // 返回控制对象
        Map<String, Object> control = new HashMap<>();
        control.put("id", asyncId);
        control.put("then", createThenFunction(asyncId, context));
        control.put("catch", createCatchFunction(asyncId, context));
        
        return control;
    }
}
```

### 4. 与其他模块交互

```java
public class InteropFunction implements CJmodFunction {
    
    @Override
    public Object execute(Map<String, Object> params, CJmodContext context) {
        // 检查依赖模块
        if (!context.hasModule("DragModule")) {
            throw new DependencyException("需要DragModule");
        }
        
        // 调用其他模块的函数
        Object dragResult = context.callModuleFunction(
            "DragModule", 
            "drag", 
            params
        );
        
        // 访问其他模块的状态
        DragStateManager dragState = (DragStateManager) 
            context.getModuleState("DragModule", "dragState");
        
        // 组合功能
        return combineFunctionality(dragResult, dragState);
    }
}
```

## 发布前检查清单

- [ ] 所有函数都有完整的参数验证
- [ ] 错误处理覆盖所有异常情况
- [ ] 性能关键路径已优化
- [ ] 内存泄漏检查通过
- [ ] 文档包含所有API
- [ ] 示例代码可正常运行
- [ ] 版本兼容性已验证
- [ ] 安全性审查通过

---

通过这些实际示例，开发者可以快速理解如何开发功能丰富、性能优异的CJMOD模块。记住：CJMOD的强大不仅在于扩展语法，更在于创造全新的开发体验！