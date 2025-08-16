package com.chtl.compiler.cjmod;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import com.chtl.ast.chtljs.CHTLJSASTNode;
import com.chtl.ast.chtljs.*;
import com.chtl.context.CHTLJSContext;

/**
 * CJmod管理器
 * 管理所有加载的CJmod模块，并提供统一的接口
 */
public class CJmodManager {
    
    private final CJmodLoader loader;
    private final Map<String, CJmodInterface> modulesByName;
    private final List<CJmodInterface> allModules;
    private final CHTLJSContext context;
    private final StringBuilder runtimeCode;
    
    public CJmodManager(CHTLJSContext context) {
        this.loader = new CJmodLoader();
        this.modulesByName = new HashMap<>();
        this.allModules = new ArrayList<>();
        this.context = context;
        this.runtimeCode = new StringBuilder();
    }
    
    /**
     * 加载单个CJmod模块
     */
    public void loadModule(Path cjmodFile) throws Exception {
        CJmodInterface module = loader.loadModule(cjmodFile);
        registerModule(module);
    }
    
    /**
     * 从目录加载所有CJmod模块
     */
    public void loadModulesFromDirectory(Path directory) throws Exception {
        List<CJmodInterface> modules = loader.loadModulesFromDirectory(directory);
        for (CJmodInterface module : modules) {
            registerModule(module);
        }
    }
    
    /**
     * 注册模块
     */
    private void registerModule(CJmodInterface module) {
        String moduleName = module.getModuleName();
        
        if (modulesByName.containsKey(moduleName)) {
            throw new IllegalStateException("Module already registered: " + moduleName);
        }
        
        // 初始化模块
        module.initialize(context);
        
        // 注册模块
        modulesByName.put(moduleName, module);
        allModules.add(module);
        
        // 收集运行时代码
        String moduleRuntime = module.getRuntimeCode();
        if (moduleRuntime != null && !moduleRuntime.isEmpty()) {
            runtimeCode.append("\n// Runtime code from module: ").append(moduleName).append("\n");
            runtimeCode.append(moduleRuntime).append("\n");
        }
        
        System.out.println("Registered CJmod module: " + moduleName + " v" + module.getModuleVersion());
    }
    
    /**
     * 处理方法调用
     * 查找并执行对应的CJmod方法
     */
    public String processMethodCall(String moduleName, String methodName, 
                                   List<String> args) {
        CJmodInterface module = modulesByName.get(moduleName);
        if (module == null) {
            throw new IllegalArgumentException("Unknown module: " + moduleName);
        }
        
        return module.executeMethod(methodName, args, context);
    }
    
    /**
     * 处理AST节点
     * 让所有模块尝试处理节点
     */
    public String processASTNode(CHTLJSASTNode node) {
        // 按注册顺序尝试处理
        for (CJmodInterface module : allModules) {
            String result = module.processASTNode(node, context);
            if (result != null) {
                // 有模块处理了这个节点
                return result;
            }
        }
        
        // 没有模块处理
        return null;
    }
    
    /**
     * 检查是否有模块提供指定的方法
     */
    public boolean hasMethod(String fullMethodName) {
        int dotIndex = fullMethodName.lastIndexOf('.');
        if (dotIndex < 0) {
            return false;
        }
        
        String moduleName = fullMethodName.substring(0, dotIndex);
        String methodName = fullMethodName.substring(dotIndex + 1);
        
        CJmodInterface module = modulesByName.get(moduleName);
        if (module == null) {
            return false;
        }
        
        return module.getProvidedMethods().containsKey(methodName);
    }
    
    /**
     * 获取所有模块的运行时代码
     */
    public String getAllRuntimeCode() {
        return runtimeCode.toString();
    }
    
    /**
     * 获取所有模块信息
     */
    public List<ModuleInfo> getAllModuleInfo() {
        List<ModuleInfo> infos = new ArrayList<>();
        
        for (CJmodInterface module : allModules) {
            ModuleInfo info = new ModuleInfo(
                module.getModuleName(),
                module.getModuleVersion(),
                module.getDescription(),
                module.getAuthor(),
                new ArrayList<>(module.getProvidedMethods().keySet()),
                new ArrayList<>(module.getProvidedObjects().keySet())
            );
            infos.add(info);
        }
        
        return infos;
    }
    
    /**
     * 生成模块文档
     */
    public String generateDocumentation() {
        StringBuilder doc = new StringBuilder();
        doc.append("# CJmod Modules Documentation\n\n");
        
        for (CJmodInterface module : allModules) {
            doc.append("## ").append(module.getModuleName())
               .append(" (v").append(module.getModuleVersion()).append(")\n\n");
            
            doc.append("**Author:** ").append(module.getAuthor()).append("\n");
            doc.append("**Description:** ").append(module.getDescription()).append("\n\n");
            
            // 方法文档
            Map<String, CJmodInterface.MethodDescriptor> methods = module.getProvidedMethods();
            if (!methods.isEmpty()) {
                doc.append("### Methods\n\n");
                for (CJmodInterface.MethodDescriptor method : methods.values()) {
                    doc.append("#### `").append(method.getName()).append("`\n\n");
                    doc.append(method.getDescription()).append("\n\n");
                    
                    if (!method.getParameters().isEmpty()) {
                        doc.append("**Parameters:**\n");
                        for (CJmodInterface.ParameterDescriptor param : method.getParameters()) {
                            doc.append("- `").append(param.getName()).append("` (")
                               .append(param.getType()).append(")");
                            if (!param.isRequired()) {
                                doc.append(" [optional]");
                            }
                            doc.append(": ").append(param.getDescription()).append("\n");
                        }
                        doc.append("\n");
                    }
                    
                    doc.append("**Returns:** ").append(method.getReturnType()).append("\n\n");
                    
                    if (method.getExample() != null && !method.getExample().isEmpty()) {
                        doc.append("**Example:**\n```javascript\n")
                           .append(method.getExample())
                           .append("\n```\n\n");
                    }
                }
            }
            
            // 对象文档
            Map<String, CJmodInterface.ObjectDescriptor> objects = module.getProvidedObjects();
            if (!objects.isEmpty()) {
                doc.append("### Objects\n\n");
                for (CJmodInterface.ObjectDescriptor obj : objects.values()) {
                    doc.append("#### `").append(obj.getName()).append("`\n\n");
                    doc.append(obj.getDescription()).append("\n\n");
                    
                    if (!obj.getProperties().isEmpty()) {
                        doc.append("**Properties:**\n");
                        for (Map.Entry<String, String> prop : obj.getProperties().entrySet()) {
                            doc.append("- `").append(prop.getKey()).append("`: ")
                               .append(prop.getValue()).append("\n");
                        }
                        doc.append("\n");
                    }
                    
                    if (!obj.getMethods().isEmpty()) {
                        doc.append("**Methods:**\n");
                        for (String methodName : obj.getMethods().keySet()) {
                            doc.append("- `").append(methodName).append("`\n");
                        }
                        doc.append("\n");
                    }
                }
            }
            
            doc.append("---\n\n");
        }
        
        return doc.toString();
    }
    
    /**
     * 清理所有模块
     */
    public void cleanup() {
        loader.unloadAll();
        modulesByName.clear();
        allModules.clear();
        runtimeCode.setLength(0);
    }
    
    /**
     * 模块信息
     */
    public static class ModuleInfo {
        private final String name;
        private final String version;
        private final String description;
        private final String author;
        private final List<String> methods;
        private final List<String> objects;
        
        public ModuleInfo(String name, String version, String description,
                         String author, List<String> methods, List<String> objects) {
            this.name = name;
            this.version = version;
            this.description = description;
            this.author = author;
            this.methods = methods;
            this.objects = objects;
        }
        
        // Getters
        public String getName() { return name; }
        public String getVersion() { return version; }
        public String getDescription() { return description; }
        public String getAuthor() { return author; }
        public List<String> getMethods() { return methods; }
        public List<String> getObjects() { return objects; }
    }
}
