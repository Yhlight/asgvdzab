package com.chtl.cjmod;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;

import com.chtl.compiler.cjmod.CJmodManager;


import java.util.*;
import java.io.*;
import java.nio.file.*;

/**
 * 默认执行上下文实现
 */
public class DefaultExecutionContext implements CJmodInterface.ExecutionContext {
    
    private final String moduleName;
    private final String functionName;
    private final Map<String, Object> configuration;
    private final CJmodGlobalStateManager stateManager;
    private final CJmodManager moduleManager;
    
    // 当前执行信息
    private String currentFile = "unknown";
    private int currentLine = 0;
    private int currentColumn = 0;
    private String currentScope = "global";
    private Map<String, Object> scopeVariables = new HashMap<>();
    
    public DefaultExecutionContext(String moduleName, String functionName) {
        this.moduleName = moduleName;
        this.functionName = functionName;
        this.configuration = new HashMap<>();
        this.stateManager = CJmodGlobalStateManager.getInstance();
        this.moduleManager = CJmodManager.getInstance();
    }
    
    @Override
    public Object getCompilationContext() {
        // 返回当前编译上下文
        Map<String, Object> context = new HashMap<>();
        context.put("module", moduleName);
        context.put("function", functionName);
        context.put("file", currentFile);
        context.put("line", currentLine);
        return context;
    }
    
    @Override
    public String getCurrentFile() {
        return currentFile;
    }
    
    @Override
    public int getCurrentLine() {
        return currentLine;
    }
    
    @Override
    public int getCurrentColumn() {
        return currentColumn;
    }
    
    @Override
    public String getCurrentScope() {
        return currentScope;
    }
    
    @Override
    public Map<String, Object> getScopeVariables() {
        return new HashMap<>(scopeVariables);
    }
    
    @Override
    public Object getGlobalState(String key) {
        return stateManager.getGlobalState(key);
    }
    
    @Override
    public void setGlobalState(String key, Object value) {
        stateManager.setGlobalState(key, value, moduleName);
    }
    
    @Override
    public boolean hasGlobalState(String key) {
        return stateManager.hasGlobalState(key);
    }
    
    @Override
    public void log(String message) {
        System.out.println("[" + moduleName + "] " + message);
    }
    
    @Override
    public void debug(String message) {
        System.out.println("[DEBUG][" + moduleName + "] " + message);
    }
    
    @Override
    public void warning(String message) {
        System.err.println("[WARNING][" + moduleName + "] " + message);
    }
    
    @Override
    public void error(String message) {
        System.err.println("[ERROR][" + moduleName + "] " + message);
    }
    
    @Override
    public Object loadResource(String path) {
        try {
            Path resourcePath = Paths.get(path);
            if (Files.exists(resourcePath)) {
                return Files.readAllBytes(resourcePath);
            }
            
            // 尝试从类路径加载
            InputStream is = getClass().getClassLoader().getResourceAsStream(path);
            if (is != null) {
                return is.readAllBytes();
            }
            
            return null;
        } catch (IOException e) {
            error("Failed to load resource: " + path + " - " + e.getMessage());
            return null;
        }
    }
    
    @Override
    public void saveResource(String path, Object data) {
        try {
            Path resourcePath = Paths.get(path);
            Files.createDirectories(resourcePath.getParent());
            
            if (data instanceof byte[]) {
                Files.write(resourcePath, (byte[]) data);
            } else if (data instanceof String) {
                Files.writeString(resourcePath, (String) data);
            } else {
                Files.writeString(resourcePath, data.toString());
            }
        } catch (IOException e) {
            error("Failed to save resource: " + path + " - " + e.getMessage());
        }
    }
    
    @Override
    public Object callFunction(String moduleName, String functionName, Object... args) {
        try {
            // 获取目标模块
            CJmodManager.LoadedModule targetModule = moduleManager.getModule(moduleName);
            if (targetModule == null) {
                // 尝试加载模块
                moduleManager.loadModule(moduleName).get();
                targetModule = moduleManager.getModule(moduleName);
            }
            
            if (targetModule != null && targetModule.getState() == CJmodManager.ModuleState.ACTIVE) {
                // 准备参数
                Map<String, Object> parameters = new HashMap<>();
                if (args.length > 0) {
                    if (args[0] instanceof Map) {
                        parameters = (Map<String, Object>) args[0];
                    } else {
                        // 将参数转换为Map
                        for (int i = 0; i < args.length; i++) {
                            parameters.put("arg" + i, args[i]);
                        }
                    }
                }
                
                // 执行函数
                CJmodInterface.FunctionResult result = 
                    moduleManager.executeFunction(moduleName, functionName, parameters).get();
                
                return result.getValue();
            }
            
            throw new RuntimeException("Module not available: " + moduleName);
            
        } catch (Exception e) {
            error("Failed to call function: " + moduleName + "." + functionName + " - " + e.getMessage());
            return null;
        }
    }
    
    @Override
    public boolean hasModule(String moduleName) {
        CJmodManager.LoadedModule module = moduleManager.getModule(moduleName);
        return module != null && module.getState() == CJmodManager.ModuleState.ACTIVE;
    }
    
    @Override
    public CJmodInterface.DOMBuilder createDOMBuilder() {
        return new DefaultDOMBuilder();
    }
    
    @Override
    public Map<String, Object> getConfiguration() {
        return new HashMap<>(configuration);
    }
    
    // Setters for context information
    public void setCurrentFile(String file) {
        this.currentFile = file;
    }
    
    public void setCurrentLine(int line) {
        this.currentLine = line;
    }
    
    public void setCurrentColumn(int column) {
        this.currentColumn = column;
    }
    
    public void setCurrentScope(String scope) {
        this.currentScope = scope;
    }
    
    public void setScopeVariables(Map<String, Object> variables) {
        this.scopeVariables = new HashMap<>(variables);
    }
    
    public void setConfiguration(Map<String, Object> config) {
        this.configuration.clear();
        this.configuration.putAll(config);
    }
}