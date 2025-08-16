package com.chtl.compiler.integration;

import com.chtl.compiler.namespace.NamespaceDefinition;
import com.chtl.context.ErrorType;
import com.chtl.compiler.namespace.DefinitionType;
import com.chtl.scanner.State;
import com.chtl.compiler.importer.ImportResult;
import com.chtl.context.WarningType;

import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.node.CustomDefinitionNode;
import com.chtl.ast.node.CustomUsageNode;
import com.chtl.ast.node.ImportStatementNode;
import com.chtl.ast.node.NamespaceNode;
import com.chtl.ast.node.TemplateDefinitionNode;
import com.chtl.ast.node.TemplateUsageNode;
import com.chtl.context.CompilationError;
import com.chtl.context.CompilationWarning;
import com.chtl.ast.*;
import com.chtl.context.CompilationContext;
import com.chtl.compiler.namespace.NamespaceManager;
import com.chtl.compiler.importer.ImportManager;
import com.chtl.scanner.ScannerStateMachine;
import java.nio.file.Path;

/**
 * 命名空间与导入集成系统
 * 协调命名空间管理和导入处理
 * 严格遵循CHTL语法文档
 */
public class NamespaceImportIntegration {
    private final CompilationContext context;
    private final NamespaceManager namespaceManager;
    private final ImportManager importManager;
    private final ScannerStateMachine stateMachine;
    
    // 命名空间路径栈（处理嵌套命名空间）
    private final Stack<String> namespacePathStack;
    
    // 导入优先级管理
    private final ImportPriorityManager priorityManager;
    
    // 循环依赖检测器
    private final CircularDependencyDetector dependencyDetector;
    
    public NamespaceImportIntegration(CompilationContext context, Path currentDirectory) {
        this.context = context;
        this.stateMachine = new ScannerStateMachine(context);
        this.namespaceManager = new NamespaceManager(context);
        this.importManager = new ImportManager(context, currentDirectory);
        this.namespacePathStack = new Stack<>();
        this.priorityManager = new ImportPriorityManager();
        this.dependencyDetector = new CircularDependencyDetector();
    }
    
    /**
     * 处理命名空间节点
     */
    public void processNamespace(NamespaceNode namespace) {
        try (var stateGuard = context.enterState(CompilationContext.State.NAMESPACE)) {
            // 构建完整的命名空间路径
            String namespacePath = buildNamespacePath(namespace.getName());
            namespacePathStack.push(namespacePath);
            
            // 进入命名空间
            namespaceManager.enterNamespace(namespace);
            
            // 处理命名空间内的内容
            processNamespaceContent(namespace);
            
            // 退出命名空间
            namespaceManager.exitNamespace();
            namespacePathStack.pop();
        }
    }
    
    /**
     * 处理导入语句
     */
    public void processImport(ImportStatementNode importNode) {
        try (var stateGuard = context.enterState(CompilationContext.State.IMPORT_STATEMENT)) {
            // 获取当前命名空间上下文
            String currentNamespace = getCurrentNamespacePath();
            
            // 检查循环依赖
            String importPath = importNode.getFromPath();
            if (!dependencyDetector.checkDependency(currentNamespace, importPath)) {
                context.addError(new com.chtl.context.CompilationError(
                    "检测到循环依赖: " + currentNamespace + " -> " + importPath,
                    com.chtl.context.CompilationError.ErrorType.CIRCULAR_DEPENDENCY
                ));
                return;
            }
            
            // 处理导入优先级
            ImportPriority priority = determineImportPriority(importNode);
            
            // 执行导入
            ImportManager.ImportResult result = importManager.processImport(importNode);
            
            // 注册到当前命名空间
            if (result.isSuccess()) {
                registerImportInNamespace(importNode, result, priority);
            }
        }
    }
    
    /**
     * 处理from语法（使用其他命名空间的元素）
     */
    public void processFromUsage(CHTLASTNode usage, String fromNamespace) {
        // 解析完整的命名空间路径（支持嵌套，如space.room.box）
        String[] pathParts = fromNamespace.split("\\.");
        
        // 查找目标命名空间
        NamespaceManager.NamespaceDefinition targetNamespace = 
            namespaceManager.findNamespace(pathParts);
            
        if (targetNamespace == null) {
            context.addError(new com.chtl.context.CompilationError(
                "未找到命名空间: " + fromNamespace,
                com.chtl.context.CompilationError.ErrorType.UNDEFINED_REFERENCE
            ));
            return;
        }
        
        // 验证使用的元素是否存在于目标命名空间
        if (!validateUsageInNamespace(usage, targetNamespace)) {
            context.addError(new com.chtl.context.CompilationError(
                "在命名空间 " + fromNamespace + " 中未找到请求的元素",
                com.chtl.context.CompilationError.ErrorType.UNDEFINED_REFERENCE
            ));
        }
    }
    
    /**
     * 构建命名空间路径
     */
    private String buildNamespacePath(String name) {
        if (namespacePathStack.isEmpty()) {
            return name;
        }
        return namespacePathStack.peek() + "." + name;
    }
    
    /**
     * 获取当前命名空间路径
     */
    private String getCurrentNamespacePath() {
        return namespacePathStack.isEmpty() ? "" : namespacePathStack.peek();
    }
    
    /**
     * 处理命名空间内容
     */
    private void processNamespaceContent(NamespaceNode namespace) {
        for (CHTLASTNode child : namespace.getChildren()) {
            if (child instanceof NamespaceNode) {
                // 处理嵌套命名空间
                processNamespace((NamespaceNode) child);
            } else if (child instanceof ImportStatementNode) {
                // 处理导入语句
                processImport((ImportStatementNode) child);
            } else {
                // 其他内容（模板定义、自定义定义等）
                registerDefinitionInNamespace(child);
            }
        }
    }
    
    /**
     * 注册定义到命名空间
     */
    private void registerDefinitionInNamespace(CHTLASTNode definition) {
        String currentNamespace = getCurrentNamespacePath();
        
        if (definition instanceof TemplateDefinitionNode) {
            TemplateDefinitionNode template = (TemplateDefinitionNode) definition;
            namespaceManager.registerDefinition(
                currentNamespace,
                NamespaceManager.DefinitionType.TEMPLATE,
                template.getName(),
                template
            );
        } else if (definition instanceof CustomDefinitionNode) {
            CustomDefinitionNode custom = (CustomDefinitionNode) definition;
            namespaceManager.registerDefinition(
                currentNamespace,
                NamespaceManager.DefinitionType.CUSTOM,
                custom.getName(),
                custom
            );
        }
    }
    
    /**
     * 确定导入优先级
     */
    private ImportPriority determineImportPriority(ImportStatementNode importNode) {
        // 根据导入类型和来源确定优先级
        if (importNode.getFromPath().startsWith("chtl::")) {
            return ImportPriority.OFFICIAL;
        } else if (importNode.getImportType() == ImportStatementNode.ImportType.CHTL) {
            return ImportPriority.LOCAL_CHTL;
        } else if (importNode.getImportType() == ImportStatementNode.ImportType.CJMOD) {
            return ImportPriority.CJMOD;
        } else {
            return ImportPriority.RESOURCE;
        }
    }
    
    /**
     * 注册导入到命名空间
     */
    private void registerImportInNamespace(ImportStatementNode importNode, 
                                          ImportManager.ImportResult result,
                                          ImportPriority priority) {
        String currentNamespace = getCurrentNamespacePath();
        
        // 如果有as语句，使用别名
        String name = importNode.getAsName() != null ? 
            importNode.getAsName() : result.getDefaultName();
            
        // 检查是否有名称冲突
        if (priorityManager.hasConflict(currentNamespace, name)) {
            ImportPriority existingPriority = priorityManager.getPriority(currentNamespace, name);
            
            if (priority.ordinal() <= existingPriority.ordinal()) {
                // 新导入的优先级更高或相同，替换
                priorityManager.register(currentNamespace, name, priority);
                
                context.addWarning(new com.chtl.context.CompilationWarning(
                    "导入 '" + name + "' 替换了已存在的定义（基于优先级）",
                    com.chtl.context.CompilationWarning.WarningType.REDEFINITION
                ));
            } else {
                // 现有定义优先级更高，保留现有定义
                context.addWarning(new com.chtl.context.CompilationWarning(
                    "导入 '" + name + "' 被忽略，因为已存在更高优先级的定义",
                    com.chtl.context.CompilationWarning.WarningType.IMPORT_IGNORED
                ));
                return;
            }
        } else {
            priorityManager.register(currentNamespace, name, priority);
        }
        
        // 注册到命名空间
        namespaceManager.registerImport(currentNamespace, name, result.getNode());
    }
    
    /**
     * 验证使用是否存在于命名空间
     */
    private boolean validateUsageInNamespace(CHTLASTNode usage, 
                                           NamespaceManager.NamespaceDefinition namespace) {
        if (usage instanceof TemplateUsageNode) {
            TemplateUsageNode templateUsage = (TemplateUsageNode) usage;
            return namespace.hasDefinition(
                NamespaceManager.DefinitionType.TEMPLATE,
                templateUsage.getName()
            );
        } else if (usage instanceof CustomUsageNode) {
            CustomUsageNode customUsage = (CustomUsageNode) usage;
            return namespace.hasDefinition(
                NamespaceManager.DefinitionType.CUSTOM,
                customUsage.getName()
            );
        }
        
        return false;
    }
    
    /**
     * 导入优先级枚举
     */
    private enum ImportPriority {
        OFFICIAL,       // 官方模块（chtl::）最高优先级
        LOCAL_CHTL,     // 本地CHTL文件
        CJMOD,          // CJmod扩展
        RESOURCE        // 资源文件（HTML/CSS/JS）最低优先级
    }
    
    /**
     * 导入优先级管理器
     */
    private static class ImportPriorityManager {
        private final Map<String, Map<String, ImportPriority>> priorities;
        
        ImportPriorityManager() {
            this.priorities = new HashMap<Object, Object>();
        }
        
        boolean hasConflict(String namespace, String name) {
            return priorities.containsKey(namespace) && 
                   priorities.get(namespace).containsKey(name);
        }
        
        ImportPriority getPriority(String namespace, String name) {
            return priorities.getOrDefault(namespace, Collections.emptyMap()).get(name);
        }
        
        void register(String namespace, String name, ImportPriority priority) {
            priorities.computeIfAbsent(namespace, k -> new HashMap<Object, Object>())
                     .put(name, priority);
        }
    }
    
    /**
     * 循环依赖检测器
     */
    private static class CircularDependencyDetector {
        private final Map<String, Set<String>> dependencies;
        private final Set<String> visiting;
        
        CircularDependencyDetector() {
            this.dependencies = new HashMap<Object, Object>();
            this.visiting = new HashSet<Object>();
        }
        
        boolean checkDependency(String from, String to) {
            // 记录依赖关系
            dependencies.computeIfAbsent(from, k -> new HashSet<Object>()).add(to);
            
            // 检查是否形成循环
            return !hasCycle(from, to);
        }
        
        private boolean hasCycle(String current, String target) {
            if (current.equals(target)) {
                return true;
            }
            
            if (visiting.contains(current)) {
                return true;
            }
            
            visiting.add(current);
            
            Set<String> deps = dependencies.get(current);
            if (deps != null) {
                for (String dep : deps) {
                    if (hasCycle(dep, target)) {
                        visiting.remove(current);
                        return true;
                    }
                }
            }
            
            visiting.remove(current);
            return false;
        }
    }
    
    /**
     * 获取命名空间管理器
     */
    public NamespaceManager getNamespaceManager() {
        return namespaceManager;
    }
    
    /**
     * 获取导入管理器
     */
    public ImportManager getImportManager() {
        return importManager;
    }
}
