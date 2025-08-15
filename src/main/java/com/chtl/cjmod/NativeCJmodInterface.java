package com.chtl.cjmod;

import java.nio.ByteBuffer;
import java.util.*;

/**
 * 本地CJMOD接口
 * 支持C++和其他本地语言实现的模块
 * 通过JNI或其他机制与本地代码交互
 */
public abstract class NativeCJmodInterface implements CJmodInterface {
    
    // 本地库路径
    protected String nativeLibraryPath;
    
    // 全局状态存储（Java端）
    protected final Map<String, Object> globalState = new HashMap<>();
    
    // 本地句柄
    protected long nativeHandle = 0;
    
    /**
     * 加载本地库
     */
    protected abstract void loadNativeLibrary(String path);
    
    /**
     * 本地方法声明
     */
    
    // 初始化本地模块
    protected native long nativeInitialize(String configJson);
    
    // 清理本地资源
    protected native void nativeCleanup(long handle);
    
    // 获取模块信息
    protected native String nativeGetModuleInfo(long handle);
    
    // 获取导出函数列表
    protected native String nativeGetExportedFunctions(long handle);
    
    // 执行函数
    protected native String nativeExecuteFunction(long handle, String functionName, 
                                                 String parametersJson, String contextJson);
    
    // 全局状态管理（本地端）
    protected native void nativeSetGlobalState(long handle, String key, byte[] value);
    protected native byte[] nativeGetGlobalState(long handle, String key);
    protected native boolean nativeHasGlobalState(long handle, String key);
    
    /**
     * 高级本地交互接口
     */
    
    // 直接内存访问
    protected native long allocateNativeMemory(int size);
    protected native void freeNativeMemory(long address);
    protected native void writeNativeMemory(long address, byte[] data, int offset, int length);
    protected native void readNativeMemory(long address, byte[] buffer, int offset, int length);
    
    // 本地回调注册
    protected native void registerCallback(String name, Object callback);
    
    // 异步执行
    protected native long beginAsyncExecution(long handle, String functionName, String parametersJson);
    protected native boolean isAsyncComplete(long asyncHandle);
    protected native String getAsyncResult(long asyncHandle);
    protected native void cancelAsync(long asyncHandle);
    
    /**
     * 工具方法
     */
    
    // JSON序列化/反序列化
    protected String toJson(Object obj) {
        // 简单的JSON序列化实现
        if (obj == null) return "null";
        if (obj instanceof String) return "\"" + escapeJson((String)obj) + "\"";
        if (obj instanceof Number || obj instanceof Boolean) return obj.toString();
        if (obj instanceof Map) {
            Map<?, ?> map = (Map<?, ?>) obj;
            StringBuilder json = new StringBuilder("{");
            boolean first = true;
            for (Map.Entry<?, ?> entry : map.entrySet()) {
                if (!first) json.append(",");
                json.append("\"").append(escapeJson(entry.getKey().toString())).append("\":");
                json.append(toJson(entry.getValue()));
                first = false;
            }
            json.append("}");
            return json.toString();
        }
        if (obj instanceof List) {
            List<?> list = (List<?>) obj;
            StringBuilder json = new StringBuilder("[");
            boolean first = true;
            for (Object item : list) {
                if (!first) json.append(",");
                json.append(toJson(item));
                first = false;
            }
            json.append("]");
            return json.toString();
        }
        return "\"" + escapeJson(obj.toString()) + "\"";
    }
    
    protected String escapeJson(String str) {
        return str.replace("\\", "\\\\")
                  .replace("\"", "\\\"")
                  .replace("\n", "\\n")
                  .replace("\r", "\\r")
                  .replace("\t", "\\t");
    }
    
    /**
     * 实现CJmodInterface
     */
    
    @Override
    public void initialize(Map<String, Object> config) {
        // 加载本地库
        if (nativeLibraryPath != null) {
            loadNativeLibrary(nativeLibraryPath);
        }
        
        // 初始化本地模块
        String configJson = toJson(config);
        nativeHandle = nativeInitialize(configJson);
        
        if (nativeHandle == 0) {
            throw new RuntimeException("Failed to initialize native module");
        }
    }
    
    @Override
    public void cleanup() {
        if (nativeHandle != 0) {
            nativeCleanup(nativeHandle);
            nativeHandle = 0;
        }
        globalState.clear();
    }
    
    @Override
    public FunctionResult executeFunction(String functionName, Map<String, Object> parameters, 
                                        ExecutionContext context) {
        if (nativeHandle == 0) {
            throw new IllegalStateException("Module not initialized");
        }
        
        // 验证参数
        List<String> errors = validateParameters(functionName, parameters);
        if (!errors.isEmpty()) {
            FunctionResult result = new FunctionResult(null, "");
            errors.forEach(result::addWarning);
            return result;
        }
        
        // 准备上下文
        Map<String, Object> contextMap = new HashMap<>();
        contextMap.put("file", context.getCurrentFile());
        contextMap.put("line", context.getCurrentLine());
        contextMap.put("column", context.getCurrentColumn());
        contextMap.put("scope", context.getCurrentScope());
        contextMap.put("config", context.getConfiguration());
        
        // 执行本地函数
        String parametersJson = toJson(parameters);
        String contextJson = toJson(contextMap);
        String resultJson = nativeExecuteFunction(nativeHandle, functionName, parametersJson, contextJson);
        
        // 解析结果
        return parseResult(resultJson);
    }
    
    protected abstract FunctionResult parseResult(String resultJson);
    
    /**
     * 高级功能：直接内存操作
     */
    public class NativeMemoryBuffer implements AutoCloseable {
        private final long address;
        private final int size;
        private boolean released = false;
        
        public NativeMemoryBuffer(int size) {
            this.size = size;
            this.address = allocateNativeMemory(size);
            if (address == 0) {
                throw new OutOfMemoryError("Failed to allocate native memory");
            }
        }
        
        public void write(byte[] data, int offset, int length) {
            if (released) throw new IllegalStateException("Buffer already released");
            if (offset + length > size) throw new IndexOutOfBoundsException();
            writeNativeMemory(address, data, offset, length);
        }
        
        public void read(byte[] buffer, int offset, int length) {
            if (released) throw new IllegalStateException("Buffer already released");
            if (offset + length > size) throw new IndexOutOfBoundsException();
            readNativeMemory(address, buffer, offset, length);
        }
        
        public ByteBuffer asByteBuffer() {
            if (released) throw new IllegalStateException("Buffer already released");
            return ByteBuffer.allocateDirect(size);
        }
        
        @Override
        public void close() {
            if (!released) {
                freeNativeMemory(address);
                released = true;
            }
        }
    }
    
    /**
     * 异步执行支持
     */
    public class AsyncExecution {
        private final long asyncHandle;
        private final String functionName;
        
        public AsyncExecution(String functionName, Map<String, Object> parameters) {
            this.functionName = functionName;
            String parametersJson = toJson(parameters);
            this.asyncHandle = beginAsyncExecution(nativeHandle, functionName, parametersJson);
        }
        
        public boolean isComplete() {
            return isAsyncComplete(asyncHandle);
        }
        
        public FunctionResult getResult() {
            if (!isComplete()) {
                throw new IllegalStateException("Async execution not complete");
            }
            String resultJson = getAsyncResult(asyncHandle);
            return parseResult(resultJson);
        }
        
        public void cancel() {
            cancelAsync(asyncHandle);
        }
    }
}