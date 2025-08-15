package com.chtl.compiler.cjmod;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.chtl.compiler.cjmod.CJmodInterface.MethodDescriptor;
import com.chtl.compiler.cjmod.CJmodInterface.ObjectDescriptor;
import com.chtl.compiler.cjmod.CJmodInterface.ParameterDescriptor;


import com.chtl.ast.chtljs.CHTLJSASTNode;
import com.chtl.context.CHTLJSContext;

import java.util.*;

/**
 * CJmod模块抽象基类
 * 提供CJmodInterface的默认实现
 * 开发者可以继承此类来创建自己的CJmod模块
 */
public abstract class AbstractCJmodModule implements CJmodInterface {
    
    protected final String moduleName;
    protected final String moduleVersion;
    protected final String description;
    protected final String author;
    
    protected final Map<String, MethodDescriptor> methods;
    protected final Map<String, ObjectDescriptor> objects;
    protected CHTLJSContext context;
    
    protected AbstractCJmodModule(String moduleName, String moduleVersion, 
                                 String description, String author) {
        this.moduleName = moduleName;
        this.moduleVersion = moduleVersion;
        this.description = description;
        this.author = author;
        this.methods = new HashMap<>();
        this.objects = new HashMap<>();
    }
    
    @Override
    public String getModuleName() {
        return moduleName;
    }
    
    @Override
    public String getModuleVersion() {
        return moduleVersion;
    }
    
    @Override
    public String getDescription() {
        return description;
    }
    
    @Override
    public String getAuthor() {
        return author;
    }
    
    @Override
    public Map<String, MethodDescriptor> getProvidedMethods() {
        return Collections.unmodifiableMap(methods);
    }
    
    @Override
    public Map<String, ObjectDescriptor> getProvidedObjects() {
        return Collections.unmodifiableMap(objects);
    }
    
    @Override
    public void initialize(CHTLJSContext context) {
        this.context = context;
        // 注册方法和对象
        registerMethods();
        registerObjects();
    }
    
    /**
     * 子类应该重写此方法来注册提供的方法
     */
    protected abstract void registerMethods();
    
    /**
     * 子类应该重写此方法来注册提供的对象
     */
    protected abstract void registerObjects();
    
    /**
     * 注册一个方法
     */
    protected void registerMethod(MethodDescriptor method) {
        methods.put(method.getName(), method);
    }
    
    /**
     * 注册一个对象
     */
    protected void registerObject(ObjectDescriptor object) {
        objects.put(object.getName(), object);
    }
    
    /**
     * 创建方法描述符的辅助方法
     */
    protected MethodDescriptor createMethod(String name, String description,
                                          String returnType, String example) {
        return new MethodDescriptor(name, description, 
            Collections.emptyList(), returnType, example);
    }
    
    /**
     * 创建带参数的方法描述符
     */
    protected MethodDescriptor createMethod(String name, String description,
                                          List<ParameterDescriptor> parameters,
                                          String returnType, String example) {
        return new MethodDescriptor(name, description, parameters, returnType, example);
    }
    
    /**
     * 创建参数描述符
     */
    protected ParameterDescriptor createParameter(String name, String type, 
                                                String description, boolean required) {
        return new ParameterDescriptor(name, type, description, required);
    }
    
    /**
     * 创建参数描述符（必需参数）
     */
    protected ParameterDescriptor createParameter(String name, String type, String description) {
        return createParameter(name, type, description, true);
    }
    
    @Override
    public String executeMethod(String methodName, List<String> args, CHTLJSContext context) {
        if (!methods.containsKey(methodName)) {
            throw new IllegalArgumentException("Unknown method: " + methodName);
        }
        
        // 验证参数
        MethodDescriptor method = methods.get(methodName);
        validateArguments(method, args);
        
        // 执行具体方法
        return doExecuteMethod(methodName, args, context);
    }
    
    /**
     * 子类应该实现此方法来执行具体的方法逻辑
     */
    protected abstract String doExecuteMethod(String methodName, List<String> args, 
                                            CHTLJSContext context);
    
    /**
     * 验证方法参数
     */
    protected void validateArguments(MethodDescriptor method, List<String> args) {
        List<ParameterDescriptor> params = method.getParameters();
        
        // 检查必需参数
        long requiredCount = params.stream().filter(ParameterDescriptor::isRequired).count();
        if (args.size() < requiredCount) {
            throw new IllegalArgumentException(
                String.format("Method %s requires at least %d arguments, but got %d",
                    method.getName(), requiredCount, args.size())
            );
        }
        
        // 检查参数数量上限
        if (args.size() > params.size()) {
            throw new IllegalArgumentException(
                String.format("Method %s accepts at most %d arguments, but got %d",
                    method.getName(), params.size(), args.size())
            );
        }
    }
    
    @Override
    public String processASTNode(CHTLJSASTNode node, CHTLJSContext context) {
        // 默认不处理任何节点
        return null;
    }
    
    @Override
    public String getRuntimeCode() {
        // 默认没有运行时代码
        return "";
    }
    
    @Override
    public void cleanup() {
        // 默认没有资源需要清理
    }
    
    /**
     * 生成JavaScript函数调用代码
     */
    protected String generateFunctionCall(String functionName, List<String> args) {
        StringBuilder js = new StringBuilder();
        js.append(functionName).append("(");
        
        for (int i = 0; i < args.size(); i++) {
            if (i > 0) js.append(", ");
            js.append(args.get(i));
        }
        
        js.append(")");
        return js.toString();
    }
    
    /**
     * 生成JavaScript对象创建代码
     */
    protected String generateObjectCreation(String className, List<String> args) {
        StringBuilder js = new StringBuilder();
        js.append("new ").append(className).append("(");
        
        for (int i = 0; i < args.size(); i++) {
            if (i > 0) js.append(", ");
            js.append(args.get(i));
        }
        
        js.append(")");
        return js.toString();
    }
    
    /**
     * 转义JavaScript字符串
     */
    protected String escapeJavaScript(String str) {
        return str.replace("\\", "\\\\")
                 .replace("\"", "\\\"")
                 .replace("'", "\\'")
                 .replace("\n", "\\n")
                 .replace("\r", "\\r")
                 .replace("\t", "\\t");
    }
}