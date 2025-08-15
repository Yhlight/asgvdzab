package com.chtl.compiler.cjmod;import com.chtl.compiler.cjmod.CJmodInterface.MethodDescriptor;
import com.chtl.compiler.cjmod.CJmodInterface.ObjectDescriptor;
import com.chtl.compiler.cjmod.CJmodInterface.ParameterDescriptor;


import com.chtl.ast.chtljs.CHTLJSASTNode;
import com.chtl.context.CHTLJSContext;

import java.util.List;
import java.util.Map;

/**
 * CJmod接口定义
 * 所有CJmod模块必须实现此接口
 * 用于扩展CHTL JS功能
 */
public interface CJmodInterface {
    
    /**
     * 获取模块名称
     */
    String getModuleName();
    
    /**
     * 获取模块版本
     */
    String getModuleVersion();
    
    /**
     * 获取模块描述
     */
    String getDescription();
    
    /**
     * 获取作者信息
     */
    String getAuthor();
    
    /**
     * 获取模块提供的方法列表
     * @return 方法名到方法描述的映射
     */
    Map<String, MethodDescriptor> getProvidedMethods();
    
    /**
     * 获取模块提供的对象/类列表
     * @return 对象名到对象描述的映射
     */
    Map<String, ObjectDescriptor> getProvidedObjects();
    
    /**
     * 初始化模块
     * @param context CHTL JS编译上下文
     */
    void initialize(CHTLJSContext context);
    
    /**
     * 执行方法
     * @param methodName 方法名
     * @param args 参数列表
     * @param context 编译上下文
     * @return 返回的JavaScript代码
     */
    String executeMethod(String methodName, List<String> args, CHTLJSContext context);
    
    /**
     * 处理AST节点
     * 允许模块对特定的AST节点进行转换
     * @param node 输入的AST节点
     * @param context 编译上下文
     * @return 转换后的JavaScript代码，如果不处理则返回null
     */
    String processASTNode(CHTLJSASTNode node, CHTLJSContext context);
    
    /**
     * 获取模块的JavaScript运行时代码
     * 这些代码会被注入到生成的JavaScript中
     * @return JavaScript代码
     */
    String getRuntimeCode();
    
    /**
     * 清理资源
     */
    void cleanup();
    
    /**
     * 方法描述符
     */
    class MethodDescriptor {
        private final String name;
        private final String description;
        private final List<ParameterDescriptor> parameters;
        private final String returnType;
        private final String example;
        
        public MethodDescriptor(String name, String description, 
                              List<ParameterDescriptor> parameters, 
                              String returnType, String example) {
            this.name = name;
            this.description = description;
            this.parameters = parameters;
            this.returnType = returnType;
            this.example = example;
        }
        
        // Getters
        public String getName() { return name; }
        public String getDescription() { return description; }
        public List<ParameterDescriptor> getParameters() { return parameters; }
        public String getReturnType() { return returnType; }
        public String getExample() { return example; }
    }
    
    /**
     * 参数描述符
     */
    class ParameterDescriptor {
        private final String name;
        private final String type;
        private final String description;
        private final boolean required;
        
        public ParameterDescriptor(String name, String type, String description, boolean required) {
            this.name = name;
            this.type = type;
            this.description = description;
            this.required = required;
        }
        
        // Getters
        public String getName() { return name; }
        public String getType() { return type; }
        public String getDescription() { return description; }
        public boolean isRequired() { return required; }
    }
    
    /**
     * 对象描述符
     */
    class ObjectDescriptor {
        private final String name;
        private final String description;
        private final Map<String, MethodDescriptor> methods;
        private final Map<String, String> properties;
        
        public ObjectDescriptor(String name, String description,
                              Map<String, MethodDescriptor> methods,
                              Map<String, String> properties) {
            this.name = name;
            this.description = description;
            this.methods = methods;
            this.properties = properties;
        }
        
        // Getters
        public String getName() { return name; }
        public String getDescription() { return description; }
        public Map<String, MethodDescriptor> getMethods() { return methods; }
        public Map<String, String> getProperties() { return properties; }
    }
}