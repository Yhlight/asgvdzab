# CJMOD开发者指南

## 目录
1. [CJMOD概述](#cjmod概述)
2. [核心概念](#核心概念)
3. [开发Java模块](#开发java模块)
4. [开发C++模块](#开发c模块)
5. [全局状态管理](#全局状态管理)
6. [高级特性](#高级特性)
7. [最佳实践](#最佳实践)
8. [示例项目](#示例项目)

## CJMOD概述

CJMOD (CHTL JavaScript Module) 是CHTL提供的扩展机制，允许开发者通过Java或C++扩展CHTL JS的语法和功能。CJMOD体系具有极高的自由度，支持：

- 🚀 **自由引入官方头文件**：C++模块可以使用任何标准库和第三方库
- 🌐 **全局状态管理**：线程安全的全局状态存储和共享
- 🔧 **灵活的接口设计**：支持同步/异步执行、直接内存访问、回调机制
- 🎯 **模块间交互**：模块可以相互调用和共享数据
- 📦 **依赖管理**：自动处理模块依赖关系

## 核心概念

### 1. CJmodInterface

所有CJMOD模块都必须实现`CJmodInterface`接口：

```java
public interface CJmodInterface {
    // 获取模块信息
    ModuleInfo getModuleInfo();
    
    // 获取导出的函数
    List<FunctionSignature> getExportedFunctions();
    
    // 执行函数
    FunctionResult executeFunction(String functionName, 
                                 Map<String, Object> parameters, 
                                 ExecutionContext context);
    
    // 初始化和清理
    void initialize(Map<String, Object> config);
    void cleanup();
}
```

### 2. ExecutionContext

执行上下文提供了丰富的运行时信息和工具：

```java
interface ExecutionContext {
    // 编译信息
    Object getCompilationContext();
    String getCurrentFile();
    int getCurrentLine();
    
    // 全局状态
    Object getGlobalState(String key);
    void setGlobalState(String key, Object value);
    
    // 日志
    void log(String message);
    void error(String message);
    
    // 资源管理
    Object loadResource(String path);
    
    // 模块交互
    Object callFunction(String module, String function, Object... args);
    
    // DOM构建
    DOMBuilder createDOMBuilder();
}
```

## 开发Java模块

### 1. 创建模块类

```java
package com.example.mymodule;

import com.chtl.cjmod.*;
import java.util.*;

public class MyModule implements CJmodInterface {
    
    // 全局状态（模块级别）
    private Map<String, Object> moduleState = new HashMap<>();
    
    @Override
    public ModuleInfo getModuleInfo() {
        return new ModuleInfo() {
            public String getName() { return "MyModule"; }
            public String getVersion() { return "1.0.0"; }
            public String getDescription() { return "示例CJMOD模块"; }
            public String getAuthor() { return "Your Name"; }
            public String getLicense() { return "MIT"; }
            public List<String> getDependencies() { 
                return Arrays.asList(); 
            }
            public Map<String, String> getMetadata() {
                Map<String, String> meta = new HashMap<>();
                meta.put("homepage", "https://example.com");
                return meta;
            }
        };
    }
    
    @Override
    public List<FunctionSignature> getExportedFunctions() {
        List<FunctionSignature> functions = new ArrayList<>();
        
        // 定义函数: processData
        List<Parameter> params = Arrays.asList(
            new Parameter("data", "string", true, null, "要处理的数据"),
            new Parameter("options", "object", false, 
                         new HashMap<>(), "处理选项")
        );
        
        FunctionSignature processData = new FunctionSignature(
            "processData", params, "object", 
            "处理数据并返回结果"
        );
        functions.add(processData);
        
        return functions;
    }
    
    @Override
    public FunctionResult executeFunction(String functionName, 
                                        Map<String, Object> parameters,
                                        ExecutionContext context) {
        switch (functionName) {
            case "processData":
                return processData(parameters, context);
            default:
                throw new IllegalArgumentException(
                    "Unknown function: " + functionName
                );
        }
    }
    
    private FunctionResult processData(Map<String, Object> params, 
                                     ExecutionContext context) {
        String data = (String) params.get("data");
        Map<String, Object> options = (Map<String, Object>) 
                                    params.getOrDefault("options", new HashMap<>());
        
        // 使用全局状态
        Integer processCount = (Integer) context.getGlobalState("processCount");
        if (processCount == null) processCount = 0;
        context.setGlobalState("processCount", processCount + 1);
        
        // 处理数据
        String processed = data.toUpperCase();
        
        // 生成JavaScript代码
        StringBuilder js = new StringBuilder();
        js.append("console.log('Processing data...');\n");
        js.append("const result = '").append(processed).append("';\n");
        js.append("console.log('Process count:', ").append(processCount + 1).append(");\n");
        
        // 返回结果
        FunctionResult result = new FunctionResult(processed, js.toString());
        result.addMetadata("processCount", processCount + 1);
        
        return result;
    }
    
    @Override
    public void initialize(Map<String, Object> config) {
        // 初始化模块
        System.out.println("MyModule initialized with config: " + config);
        
        // 访问全局状态管理器
        CJmodGlobalStateManager stateManager = 
            (CJmodGlobalStateManager) config.get("globalStateManager");
        
        // 注册状态监听器
        stateManager.addStateChangeListener("mymodule.*", 
            new CJmodGlobalStateManager.StateChangeListener() {
                public void onStateChanged(String key, Object oldValue, 
                                         Object newValue, String modifier) {
                    System.out.println("State changed: " + key);
                }
                
                public void onStateRemoved(String key, Object oldValue, 
                                         String remover) {
                    System.out.println("State removed: " + key);
                }
            }
        );
    }
    
    @Override
    public void cleanup() {
        // 清理资源
        moduleState.clear();
    }
}
```

### 2. 创建模块描述符

创建`module.json`文件：

```json
{
    "name": "MyModule",
    "version": "1.0.0",
    "mainClass": "com.example.mymodule.MyModule",
    "description": "示例CJMOD模块",
    "author": "Your Name",
    "dependencies": ""
}
```

### 3. 打包模块

```bash
# 编译Java文件
javac -cp chtl-cjmod-api.jar MyModule.java

# 创建JAR包
jar cvf mymodule.jar com/example/mymodule/*.class module.json
```

## 开发C++模块

### 1. 创建模块头文件

```cpp
// mymodule.h
#ifndef MYMODULE_H
#define MYMODULE_H

#include <jni.h>
#include <string>
#include <map>
#include <vector>
#include <memory>

// 可以自由引入任何需要的头文件
#include <opencv2/opencv.hpp>
#include <boost/algorithm/string.hpp>
#include <thread>
#include <atomic>

// 全局状态
namespace MyModuleGlobal {
    extern std::atomic<int> processCount;
    extern std::map<std::string, cv::Mat> imageCache;
    extern std::mutex cacheMutex;
}

// JNI函数声明
extern "C" {
    JNIEXPORT jlong JNICALL Java_com_example_MyNativeModule_nativeInitialize
        (JNIEnv *, jobject, jstring);
    
    JNIEXPORT void JNICALL Java_com_example_MyNativeModule_nativeCleanup
        (JNIEnv *, jobject, jlong);
    
    JNIEXPORT jstring JNICALL Java_com_example_MyNativeModule_nativeExecuteFunction
        (JNIEnv *, jobject, jlong, jstring, jstring, jstring);
}

// 模块实现类
class MyModuleImpl {
private:
    std::map<std::string, std::string> config;
    std::unique_ptr<cv::VideoCapture> camera;
    
public:
    MyModuleImpl(const std::string& configJson);
    ~MyModuleImpl();
    
    std::string executeFunction(const std::string& functionName,
                               const std::string& paramsJson,
                               const std::string& contextJson);
    
private:
    std::string processImage(const std::string& imagePath, 
                           const std::map<std::string, std::string>& options);
    std::string captureFromCamera(int cameraId);
    std::string applyFilter(const cv::Mat& image, const std::string& filterType);
};

#endif
```

### 2. 实现模块

```cpp
// mymodule.cpp
#include "mymodule.h"
#include <json/json.h>
#include <sstream>

// 全局状态定义
namespace MyModuleGlobal {
    std::atomic<int> processCount(0);
    std::map<std::string, cv::Mat> imageCache;
    std::mutex cacheMutex;
}

// JNI实现
JNIEXPORT jlong JNICALL Java_com_example_MyNativeModule_nativeInitialize
    (JNIEnv *env, jobject obj, jstring configJson) {
    
    const char* config = env->GetStringUTFChars(configJson, nullptr);
    MyModuleImpl* impl = new MyModuleImpl(config);
    env->ReleaseStringUTFChars(configJson, config);
    
    return reinterpret_cast<jlong>(impl);
}

JNIEXPORT void JNICALL Java_com_example_MyNativeModule_nativeCleanup
    (JNIEnv *env, jobject obj, jlong handle) {
    
    MyModuleImpl* impl = reinterpret_cast<MyModuleImpl*>(handle);
    delete impl;
}

JNIEXPORT jstring JNICALL Java_com_example_MyNativeModule_nativeExecuteFunction
    (JNIEnv *env, jobject obj, jlong handle, jstring functionName, 
     jstring paramsJson, jstring contextJson) {
    
    MyModuleImpl* impl = reinterpret_cast<MyModuleImpl*>(handle);
    
    const char* funcName = env->GetStringUTFChars(functionName, nullptr);
    const char* params = env->GetStringUTFChars(paramsJson, nullptr);
    const char* context = env->GetStringUTFChars(contextJson, nullptr);
    
    std::string result = impl->executeFunction(funcName, params, context);
    
    env->ReleaseStringUTFChars(functionName, funcName);
    env->ReleaseStringUTFChars(paramsJson, params);
    env->ReleaseStringUTFChars(contextJson, context);
    
    return env->NewStringUTF(result.c_str());
}

// 模块实现
MyModuleImpl::MyModuleImpl(const std::string& configJson) {
    // 解析配置
    Json::Reader reader;
    Json::Value root;
    reader.parse(configJson, root);
    
    // 初始化OpenCV
    cv::setNumThreads(4);
    
    // 初始化相机（如果需要）
    if (root["enableCamera"].asBool()) {
        camera = std::make_unique<cv::VideoCapture>(0);
    }
}

MyModuleImpl::~MyModuleImpl() {
    // 清理资源
    if (camera && camera->isOpened()) {
        camera->release();
    }
    
    // 清理全局缓存
    std::lock_guard<std::mutex> lock(MyModuleGlobal::cacheMutex);
    MyModuleGlobal::imageCache.clear();
}

std::string MyModuleImpl::executeFunction(const std::string& functionName,
                                        const std::string& paramsJson,
                                        const std::string& contextJson) {
    Json::Reader reader;
    Json::Value params, context;
    reader.parse(paramsJson, params);
    reader.parse(contextJson, context);
    
    Json::Value result;
    
    if (functionName == "processImage") {
        std::string imagePath = params["imagePath"].asString();
        std::map<std::string, std::string> options;
        
        Json::Value::Members members = params["options"].getMemberNames();
        for (const auto& member : members) {
            options[member] = params["options"][member].asString();
        }
        
        std::string processed = processImage(imagePath, options);
        
        result["value"] = processed;
        result["generatedCode"] = "console.log('Image processed by C++ module');";
        
        // 更新全局计数
        int count = MyModuleGlobal::processCount.fetch_add(1) + 1;
        result["metadata"]["processCount"] = count;
        
    } else if (functionName == "captureFromCamera") {
        int cameraId = params["cameraId"].asInt();
        std::string captured = captureFromCamera(cameraId);
        
        result["value"] = captured;
        result["generatedCode"] = "console.log('Camera capture completed');";
    }
    
    Json::FastWriter writer;
    return writer.write(result);
}

std::string MyModuleImpl::processImage(const std::string& imagePath, 
                                     const std::map<std::string, std::string>& options) {
    // 检查缓存
    {
        std::lock_guard<std::mutex> lock(MyModuleGlobal::cacheMutex);
        auto it = MyModuleGlobal::imageCache.find(imagePath);
        if (it != MyModuleGlobal::imageCache.end()) {
            // 使用缓存的图像
            cv::Mat& image = it->second;
            // 处理图像...
        }
    }
    
    // 加载图像
    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        return "Error: Cannot load image";
    }
    
    // 应用滤镜
    auto filterIt = options.find("filter");
    if (filterIt != options.end()) {
        return applyFilter(image, filterIt->second);
    }
    
    // 缓存图像
    {
        std::lock_guard<std::mutex> lock(MyModuleGlobal::cacheMutex);
        MyModuleGlobal::imageCache[imagePath] = image.clone();
    }
    
    return "Image processed successfully";
}
```

### 3. 编译C++模块

```bash
# Linux/Mac
g++ -shared -fPIC -o mymodule.so mymodule.cpp \
    -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux \
    -lopencv_core -lopencv_imgproc -lopencv_imgcodecs \
    -lboost_system -ljsoncpp

# Windows
cl /LD mymodule.cpp /I"%JAVA_HOME%\include" /I"%JAVA_HOME%\include\win32" \
    opencv_world.lib jsoncpp.lib /Fe:mymodule.dll
```

## 全局状态管理

CJMOD提供了强大的全局状态管理机制：

### 1. 基本使用

```java
// 在模块中使用全局状态
CJmodGlobalStateManager stateManager = CJmodGlobalStateManager.getInstance();

// 设置状态
stateManager.setGlobalState("mymodule.config", configObject, "MyModule");

// 获取状态
Object config = stateManager.getGlobalState("mymodule.config");

// 查询状态
Map<String, Object> results = stateManager.queryStates(
    new StateQuery()
        .withKeyPattern("mymodule.*")
        .withMinAccessCount(5)
);
```

### 2. 状态监听

```java
// 注册监听器
stateManager.addStateChangeListener("user.*", 
    new StateChangeListener() {
        public void onStateChanged(String key, Object oldValue, 
                                 Object newValue, String modifier) {
            System.out.println("User state changed: " + key);
            // 响应状态变化
        }
        
        public void onStateRemoved(String key, Object oldValue, 
                                 String remover) {
            System.out.println("User state removed: " + key);
        }
    }
);
```

### 3. 持久化

```java
// 启用自动持久化
stateManager.enablePersistence(
    Paths.get("states.json"), 
    true // 自动持久化
);

// 手动持久化
stateManager.persist();
```

## 高级特性

### 1. 异步执行

```java
public class AsyncModule extends NativeCJmodInterface {
    
    @Override
    public FunctionResult executeFunction(String functionName, 
                                        Map<String, Object> parameters,
                                        ExecutionContext context) {
        // 创建异步执行
        AsyncExecution async = new AsyncExecution(functionName, parameters);
        
        // 立即返回，告诉CHTL JS生成异步代码
        FunctionResult result = new FunctionResult(null, 
            "const promise = new Promise((resolve, reject) => {\n" +
            "  // Async operation handled by CJMOD\n" +
            "  _chtl.waitForAsync('" + async.getId() + "', resolve, reject);\n" +
            "});"
        );
        
        return result;
    }
}
```

### 2. 直接内存操作

```java
// 创建本地内存缓冲区
try (NativeMemoryBuffer buffer = new NativeMemoryBuffer(1024 * 1024)) {
    // 写入数据
    byte[] data = "Hello from Java".getBytes();
    buffer.write(data, 0, data.length);
    
    // 传递给本地代码
    long address = buffer.getAddress();
    processNativeBuffer(address, data.length);
}
```

### 3. 模块间调用

```java
private FunctionResult callOtherModule(ExecutionContext context) {
    // 调用其他模块的函数
    Object result = context.callFunction(
        "ImageProcessor",     // 模块名
        "enhanceImage",      // 函数名
        imagePath,           // 参数
        options
    );
    
    // 生成JavaScript代码
    String js = "const enhanced = " + result + ";";
    
    return new FunctionResult(result, js);
}
```

## 最佳实践

### 1. 错误处理

```java
@Override
public FunctionResult executeFunction(String functionName, 
                                    Map<String, Object> parameters,
                                    ExecutionContext context) {
    try {
        // 验证参数
        List<String> errors = validateParameters(functionName, parameters);
        if (!errors.isEmpty()) {
            FunctionResult result = new FunctionResult(null, 
                "throw new Error('" + String.join(", ", errors) + "');"
            );
            errors.forEach(result::addWarning);
            return result;
        }
        
        // 执行函数
        return doExecute(functionName, parameters, context);
        
    } catch (Exception e) {
        context.error("Function execution failed: " + e.getMessage());
        
        return new FunctionResult(null, 
            "throw new Error('" + e.getMessage() + "');"
        );
    }
}
```

### 2. 资源管理

```java
public class ResourceAwareModule implements CJmodInterface {
    private final List<AutoCloseable> resources = new ArrayList<>();
    
    @Override
    public void initialize(Map<String, Object> config) {
        // 注册资源
        DatabaseConnection db = new DatabaseConnection(config);
        resources.add(db);
        
        FileWatcher watcher = new FileWatcher(config);
        resources.add(watcher);
    }
    
    @Override
    public void cleanup() {
        // 清理所有资源
        for (AutoCloseable resource : resources) {
            try {
                resource.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        resources.clear();
    }
}
```

### 3. 性能优化

```java
public class OptimizedModule implements CJmodInterface {
    // 使用缓存
    private final Map<String, FunctionResult> cache = new ConcurrentHashMap<>();
    
    @Override
    public FunctionResult executeFunction(String functionName, 
                                        Map<String, Object> parameters,
                                        ExecutionContext context) {
        // 生成缓存键
        String cacheKey = functionName + ":" + parameters.hashCode();
        
        // 检查缓存
        FunctionResult cached = cache.get(cacheKey);
        if (cached != null) {
            context.debug("Cache hit for " + cacheKey);
            return cached;
        }
        
        // 执行函数
        FunctionResult result = doExecute(functionName, parameters, context);
        
        // 缓存结果
        cache.put(cacheKey, result);
        
        return result;
    }
}
```

## 示例项目

### 完整的图像处理模块

参见 `/workspace/examples/cjmod/ImageProcessor/` 目录，包含：

- 完整的Java/C++混合实现
- 支持多种图像格式和滤镜
- 异步处理和进度回调
- 全局缓存管理
- 完整的错误处理

### 数据分析模块

参见 `/workspace/examples/cjmod/DataAnalyzer/` 目录，展示：

- 大数据处理能力
- 流式处理API
- 与Python集成（通过JNI）
- 实时数据可视化

## 总结

CJMOD体系提供了极大的自由度和灵活性，让开发者能够：

1. **自由扩展**：不受限制地使用任何库和技术
2. **高效集成**：与现有系统无缝集成
3. **强大功能**：从简单函数到复杂系统都能实现
4. **易于维护**：清晰的接口和模块化设计

通过CJMOD，CHTL不仅是一个模板语言，更是一个可扩展的开发平台。