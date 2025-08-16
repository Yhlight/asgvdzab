package com.chtl.cjmod;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Function;

import com.chtl.compiler.cjmod.CJmodInterface;
import com.chtl.compiler.cjmod.CJmodManager.ModuleInfo;


import java.util.*;

/**
 * CJMOD核心接口
 * 提供极高自由度的扩展能力
 */
public interface CJmodInterface {
    
    /**
     * 模块元信息
     */
    interface ModuleInfo {
        String getName();
        String getVersion();
        String getDescription();
        String getAuthor();
        String getLicense();
        List<String> getDependencies();
        Map<String, String> getMetadata();
    }
    
    /**
     * 函数参数定义
     */
    class Parameter {
        private final String name;
        private final String type;
        private final boolean required;
        private final Object defaultValue;
        private final String description;
        
        public Parameter(String name, String type, boolean required, Object defaultValue, String description) {
            this.name = name;
            this.type = type;
            this.required = required;
            this.defaultValue = defaultValue;
            this.description = description;
        }
        
        // Getters
        public String getName() { return name; }
        public String getType() { return type; }
        public boolean isRequired() { return required; }
        public Object getDefaultValue() { return defaultValue; }
        public String getDescription() { return description; }
    }
    
    /**
     * 函数签名定义
     */
    class FunctionSignature {
        private final String name;
        private final List<Parameter> parameters;
        private final String returnType;
        private final String description;
        private final Map<String, Object> attributes;
        
        public FunctionSignature(String name, List<Parameter> parameters, 
                               String returnType, String description) {
            this.name = name;
            this.parameters = parameters;
            this.returnType = returnType;
            this.description = description;
            this.attributes = new HashMap<>();
        }
        
        public void setAttribute(String key, Object value) {
            attributes.put(key, value);
        }
        
        // Getters
        public String getName() { return name; }
        public List<Parameter> getParameters() { return parameters; }
        public String getReturnType() { return returnType; }
        public String getDescription() { return description; }
        public Map<String, Object> getAttributes() { return attributes; }
    }
    
    /**
     * 执行上下文
     * 提供运行时信息和工具
     */
    interface ExecutionContext {
        // 获取编译上下文信息
        Object getCompilationContext();
        
        // 获取当前文件信息
        String getCurrentFile();
        int getCurrentLine();
        int getCurrentColumn();
        
        // 获取作用域信息
        String getCurrentScope();
        Map<String, Object> getScopeVariables();
        
        // 获取/设置全局状态
        Object getGlobalState(String key);
        void setGlobalState(String key, Object value);
        boolean hasGlobalState(String key);
        
        // 日志和调试
        void log(String message);
        void debug(String message);
        void warning(String message);
        void error(String message);
        
        // 资源管理
        Object loadResource(String path);
        void saveResource(String path, Object data);
        
        // 与其他模块交互
        Object callFunction(String moduleName, String functionName, Object... args);
        boolean hasModule(String moduleName);
        
        // DOM和浏览器API访问（用于生成对应的JS代码）
        DOMBuilder createDOMBuilder();
        
        // 获取配置
        Map<String, Object> getConfiguration();
    }
    
    /**
     * DOM构建器接口
     */
    interface DOMBuilder {
        DOMBuilder querySelector(String selector);
        DOMBuilder getElementById(String id);
        DOMBuilder getElementsByClassName(String className);
        DOMBuilder getElementsByTagName(String tagName);
        
        DOMBuilder addEventListener(String event, String handler);
        DOMBuilder setAttribute(String name, String value);
        DOMBuilder setStyle(String property, String value);
        DOMBuilder addClass(String className);
        DOMBuilder removeClass(String className);
        
        DOMBuilder appendChild(DOMBuilder child);
        DOMBuilder insertBefore(DOMBuilder child, DOMBuilder reference);
        DOMBuilder remove();
        
        String build(); // 生成对应的JavaScript代码
    }
    
    /**
     * 函数执行结果
     */
    class FunctionResult {
        private final Object value;
        private final String generatedCode;
        private final Map<String, Object> metadata;
        private final List<String> warnings;
        
        public FunctionResult(Object value, String generatedCode) {
            this.value = value;
            this.generatedCode = generatedCode;
            this.metadata = new HashMap<>();
            this.warnings = new ArrayList<>();
        }
        
        public void addMetadata(String key, Object value) {
            metadata.put(key, value);
        }
        
        public void addWarning(String warning) {
            warnings.add(warning);
        }
        
        // Getters
        public Object getValue() { return value; }
        public String getGeneratedCode() { return generatedCode; }
        public Map<String, Object> getMetadata() { return metadata; }
        public List<String> getWarnings() { return warnings; }
    }
    
    /**
     * 核心方法
     */
    
    // 获取模块信息
    ModuleInfo getModuleInfo();
    
    // 获取所有导出的函数
    List<FunctionSignature> getExportedFunctions();
    
    // 执行函数
    FunctionResult executeFunction(String functionName, Map<String, Object> parameters, ExecutionContext context);
    
    // 初始化模块
    void initialize(Map<String, Object> config);
    
    // 清理资源
    void cleanup();
    
    // 验证参数
    default List<String> validateParameters(String functionName, Map<String, Object> parameters) {
        List<String> errors = new ArrayList<>();
        
        // 查找函数签名
        FunctionSignature signature = getExportedFunctions().stream()
            .filter(sig -> sig.getName().equals(functionName))
            .findFirst()
            .orElse(null);
            
        if (signature == null) {
            errors.add("Function not found: " + functionName);
            return errors;
        }
        
        // 验证必需参数
        for (Parameter param : signature.getParameters()) {
            if (param.isRequired() && !parameters.containsKey(param.getName())) {
                errors.add("Missing required parameter: " + param.getName());
            }
        }
        
        return errors;
    }
}
