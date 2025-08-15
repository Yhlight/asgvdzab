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