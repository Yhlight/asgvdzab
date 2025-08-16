package com.chtl.compiler.style;

import com.chtl.context.ErrorType;
import com.chtl.context.WarningType;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.node.CustomDefinitionNode;
import com.chtl.ast.node.CustomUsageNode;
import com.chtl.ast.node.NumberLiteralNode;
import com.chtl.ast.node.StringLiteralNode;
import com.chtl.ast.node.StylePropertyNode;
import com.chtl.ast.node.TemplateDefinitionNode;
import com.chtl.ast.node.TemplateUsageNode;
import com.chtl.ast.node.UnquotedLiteralNode;
import com.chtl.context.CompilationError;
import com.chtl.context.CompilationWarning;
import com.chtl.ast.*;
import com.chtl.context.CompilationContext;

/**
 * 样式组管理器
 * 负责管理样式组模板和自定义样式组
 * 严格遵循CHTL语法文档
 */
public class StyleGroupManager {
    private final CompilationContext context;
    
    // 样式组模板存储
    private final Map<String, TemplateDefinitionNode> styleTemplates;
    
    // 自定义样式组存储
    private final Map<String, CustomDefinitionNode> customStyleGroups;
    
    // 导入的样式组
    private final Map<String, ImportedStyleGroup> importedGroups;
    
    // 样式组依赖图（用于检测循环依赖）
    private final Map<String, Set<String>> dependencyGraph;
    
    public StyleGroupManager(CompilationContext context) {
        this.context = context;
        this.styleTemplates = new HashMap<Object, Object>();
        this.customStyleGroups = new HashMap<Object, Object>();
        this.importedGroups = new HashMap<Object, Object>();
        this.dependencyGraph = new HashMap<Object, Object>();
    }
    
    /**
     * 注册样式组模板
     * [Template] @Style 组名 { ... }
     */
    public void registerStyleTemplate(TemplateDefinitionNode template) {
        String name = template.getName();
        
        if (styleTemplates.containsKey(name)) {
            context.addWarning(new com.chtl.context.CompilationWarning(
                "样式组模板 '" + name + "' 被重新定义",
                com.chtl.context.CompilationWarning.WarningType.REDEFINITION
            ));
        }
        
        styleTemplates.put(name, template);
        
        // 更新依赖图
        updateDependencies(name, template);
    }
    
    /**
     * 注册自定义样式组
     * [Custom] @Style 组名 { ... }
     */
    public void registerCustomStyleGroup(CustomDefinitionNode custom) {
        String name = custom.getName();
        
        if (customStyleGroups.containsKey(name)) {
            context.addWarning(new com.chtl.context.CompilationWarning(
                "自定义样式组 '" + name + "' 被重新定义",
                com.chtl.context.CompilationWarning.WarningType.REDEFINITION
            ));
        }
        
        customStyleGroups.put(name, custom);
        
        // 更新依赖图
        updateDependencies(name, custom);
    }
    
    /**
     * 注册导入的样式组
     */
    public void registerImportedStyleGroup(String name, String originalName, 
                                           String sourcePath, boolean isTemplate) {
        ImportedStyleGroup imported = new ImportedStyleGroup(
            originalName, sourcePath, isTemplate
        );
        importedGroups.put(name, imported);
    }
    
    /**
     * 获取样式组模板
     */
    public TemplateDefinitionNode getStyleTemplate(String name) {
        // 首先检查本地定义
        TemplateDefinitionNode template = styleTemplates.get(name);
        if (template != null) {
            return template;
        }
        
        // 检查导入的模板
        ImportedStyleGroup imported = importedGroups.get(name);
        if (imported != null && imported.isTemplate) {
            // TODO: 从导入源加载模板
            return null;
        }
        
        return null;
    }
    
    /**
     * 获取自定义样式组
     */
    public CustomDefinitionNode getCustomStyleGroup(String name) {
        // 首先检查本地定义
        CustomDefinitionNode custom = customStyleGroups.get(name);
        if (custom != null) {
            return custom;
        }
        
        // 检查导入的自定义组
        ImportedStyleGroup imported = importedGroups.get(name);
        if (imported != null && !imported.isTemplate) {
            // TODO: 从导入源加载自定义组
            return null;
        }
        
        return null;
    }
    
    /**
     * 解析样式组继承
     * 处理样式组之间的继承关系
     */
    public ResolvedStyleGroup resolveStyleGroup(String name) {
        ResolvedStyleGroup resolved = new ResolvedStyleGroup(name);
        Set<String> visited = new HashSet<Object>();
        
        // 检测循环依赖
        if (hasCircularDependency(name)) {
            context.addError(new com.chtl.context.CompilationError(
                "样式组 '" + name + "' 存在循环依赖",
                com.chtl.context.CompilationError.ErrorType.CIRCULAR_DEPENDENCY
            ));
            return resolved;
        }
        
        // 递归解析继承链
        resolveInheritance(name, resolved, visited);
        
        return resolved;
    }
    
    /**
     * 更新依赖图
     */
    private void updateDependencies(String name, CHTLASTNode node) {
        Set<String> deps = new HashSet<Object>();
        
        // 遍历节点查找依赖
        for (CHTLASTNode child : node.getChildren()) {
            if (child instanceof CustomUsageNode) {
                deps.add(((CustomUsageNode) child).getName());
            } else if (child instanceof TemplateUsageNode) {
                deps.add(((TemplateUsageNode) child).getName());
            }
        }
        
        dependencyGraph.put(name, deps);
    }
    
    /**
     * 检测循环依赖
     */
    private boolean hasCircularDependency(String start) {
        Set<String> visited = new HashSet<Object>();
        Set<String> recursionStack = new HashSet<Object>();
        
        return detectCycle(start, visited, recursionStack);
    }
    
    /**
     * DFS检测循环
     */
    private boolean detectCycle(String node, Set<String> visited, Set<String> recursionStack) {
        visited.add(node);
        recursionStack.add(node);
        
        Set<String> deps = dependencyGraph.getOrDefault(node, Collections.emptySet());
        for (String dep : deps) {
            if (!visited.contains(dep)) {
                if (detectCycle(dep, visited, recursionStack)) {
                    return true;
                }
            } else if (recursionStack.contains(dep)) {
                return true;
            }
        }
        
        recursionStack.remove(node);
        return false;
    }
    
    /**
     * 递归解析继承
     */
    private void resolveInheritance(String name, ResolvedStyleGroup resolved, Set<String> visited) {
        if (visited.contains(name)) {
            return;
        }
        visited.add(name);
        
        // 获取样式组定义
        TemplateDefinitionNode template = getStyleTemplate(name);
        if (template != null) {
            processStyleGroupNode(template, resolved, visited);
            return;
        }
        
        CustomDefinitionNode custom = getCustomStyleGroup(name);
        if (custom != null) {
            processStyleGroupNode(custom, resolved, visited);
        }
    }
    
    /**
     * 处理样式组节点
     */
    private void processStyleGroupNode(CHTLASTNode node, ResolvedStyleGroup resolved, Set<String> visited) {
        for (CHTLASTNode child : node.getChildren()) {
            if (child instanceof StylePropertyNode) {
                StylePropertyNode property = (StylePropertyNode) child;
                // 后定义的属性覆盖先定义的
                String propName = property.getProperty();
                String propValue = getStringValue(property.getValue());
                resolved.properties.put(propName, propValue);
                
            } else if (child instanceof CustomUsageNode) {
                // 递归解析继承的样式组
                String inheritedName = ((CustomUsageNode) child).getName();
                resolveInheritance(inheritedName, resolved, visited);
                
            } else if (child instanceof TemplateUsageNode) {
                // 递归解析继承的模板
                String inheritedName = ((TemplateUsageNode) child).getName();
                resolveInheritance(inheritedName, resolved, visited);
            }
        }
    }
    
    /**
     * 导入的样式组信息
     */
    private static class ImportedStyleGroup {
        final String originalName;
        final String sourcePath;
        final boolean isTemplate;
        
        ImportedStyleGroup(String originalName, String sourcePath, boolean isTemplate) {
            this.originalName = originalName;
            this.sourcePath = sourcePath;
            this.isTemplate = isTemplate;
        }
    }
    
    /**
     * 解析后的样式组
     */
    public static class ResolvedStyleGroup {
        public final String name;
        public final Map<String, String> properties;
        public final List<String> inheritanceChain;
        
        public ResolvedStyleGroup(String name) {
            this.name = name;
            this.properties = new LinkedHashMap<Object, Object>();
            this.inheritanceChain = new ArrayList<Object>();
        }
    }
    
    /**
     * 从AST节点获取字符串值
     */
    private String getStringValue(CHTLASTNode node) {
        if (node == null) return "";
        
        if (node instanceof StringLiteralNode) {
            return ((StringLiteralNode) node).getValue();
        } else if (node instanceof UnquotedLiteralNode) {
            return ((UnquotedLiteralNode) node).getValue();
        } else if (node instanceof NumberLiteralNode) {
            return ((NumberLiteralNode) node).getValue();
        }
        
        return node.toString();
    }
}
