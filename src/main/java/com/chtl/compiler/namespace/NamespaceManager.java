package com.chtl.compiler.namespace;

import com.chtl.ast.*;
import com.chtl.ast.node.*;
import com.chtl.context.CompilationContext;
import com.chtl.context.CompilationError;
import com.chtl.context.CompilationWarning;

import java.util.*;

/**
 * 命名空间管理器
 * 实现同名命名空间合并和冲突检测
 * 严格按照CHTL语法文档实现
 */
public class NamespaceManager {
    private final CompilationContext context;
    private final Map<String, MergedNamespace> namespaces;
    private final Stack<String> currentNamespaceStack;
    
    public NamespaceManager(CompilationContext context) {
        this.context = context;
        this.namespaces = new HashMap<>();
        this.currentNamespaceStack = new Stack<>();
    }
    
    /**
     * 进入命名空间
     */
    public void enterNamespace(NamespaceNode namespace) {
        String name = namespace.getName();
        currentNamespaceStack.push(name);
        
        // 获取或创建合并的命名空间
        MergedNamespace merged = namespaces.computeIfAbsent(name, k -> new MergedNamespace(k));
        
        // 添加到合并的命名空间
        merged.addNamespaceNode(namespace);
        
        // 检查冲突
        checkConflicts(merged);
    }
    
    /**
     * 离开命名空间
     */
    public void exitNamespace() {
        if (!currentNamespaceStack.isEmpty()) {
            currentNamespaceStack.pop();
        }
    }
    
    /**
     * 获取当前命名空间
     */
    public String getCurrentNamespace() {
        return currentNamespaceStack.isEmpty() ? null : currentNamespaceStack.peek();
    }
    
    /**
     * 获取完全限定名
     */
    public String getFullyQualifiedName(String name) {
        if (currentNamespaceStack.isEmpty()) {
            return name;
        }
        return String.join(".", currentNamespaceStack) + "." + name;
    }
    
    /**
     * 在命名空间中注册定义
     */
    public void registerDefinition(String namespaceName, String definitionName, 
                                  CHTLASTNode definition, DefinitionType type) {
        MergedNamespace namespace = namespaces.get(namespaceName);
        if (namespace == null) {
            context.addError(new CompilationError(
                "Namespace not found: " + namespaceName,
                CompilationError.ErrorType.UNDEFINED_REFERENCE
            ));
            return;
        }
        
        // 检查是否已存在
        Definition existing = namespace.getDefinition(definitionName, type);
        if (existing != null) {
            // 检测冲突
            if (!isCompatibleDefinition(existing.node, definition)) {
                context.addError(new CompilationError(
                    "Conflicting definition in namespace " + namespaceName + ": " + definitionName,
                    CompilationError.ErrorType.DUPLICATE_DEFINITION
                ));
                return;
            }
            
            // 发出警告
            context.addWarning(new CompilationWarning(
                "Duplicate definition in namespace " + namespaceName + ": " + definitionName,
                CompilationWarning.WarningType.POTENTIAL_ERROR
            ));
        }
        
        namespace.addDefinition(definitionName, definition, type);
    }
    
    /**
     * 查找定义
     */
    public CHTLASTNode findDefinition(String name, DefinitionType type) {
        // 首先在当前命名空间中查找
        if (!currentNamespaceStack.isEmpty()) {
            String currentNs = currentNamespaceStack.peek();
            MergedNamespace namespace = namespaces.get(currentNs);
            if (namespace != null) {
                Definition def = namespace.getDefinition(name, type);
                if (def != null) {
                    return def.node;
                }
            }
        }
        
        // 然后在全局命名空间中查找
        for (MergedNamespace namespace : namespaces.values()) {
            Definition def = namespace.getDefinition(name, type);
            if (def != null) {
                return def.node;
            }
        }
        
        return null;
    }
    
    /**
     * 合并所有同名命名空间
     */
    public void mergeAllNamespaces() {
        for (MergedNamespace namespace : namespaces.values()) {
            mergeNamespace(namespace);
        }
    }
    
    /**
     * 合并单个命名空间
     */
    private void mergeNamespace(MergedNamespace namespace) {
        if (namespace.nodes.size() <= 1) {
            return; // 无需合并
        }
        
        // 创建合并后的命名空间节点
        NamespaceNode mergedNode = new NamespaceNode(namespace.name);
        
        // 合并全局约束
        Set<ConstraintNode> mergedConstraints = new HashSet<>();
        for (NamespaceNode node : namespace.nodes) {
            mergedConstraints.addAll(node.getGlobalConstraints());
        }
        for (ConstraintNode constraint : mergedConstraints) {
            mergedNode.addGlobalConstraint(constraint);
        }
        
        // 合并子节点
        Map<String, List<CHTLASTNode>> childrenByType = new HashMap<>();
        for (NamespaceNode node : namespace.nodes) {
            for (CHTLASTNode child : node.getChildren()) {
                String key = getChildKey(child);
                childrenByType.computeIfAbsent(key, k -> new ArrayList<>()).add(child);
            }
        }
        
        // 处理合并的子节点
        for (Map.Entry<String, List<CHTLASTNode>> entry : childrenByType.entrySet()) {
            List<CHTLASTNode> children = entry.getValue();
            if (children.size() == 1) {
                mergedNode.addChild(children.get(0));
            } else {
                // 检查冲突并合并
                CHTLASTNode merged = mergeChildren(children);
                if (merged != null) {
                    mergedNode.addChild(merged);
                }
            }
        }
        
        // 替换原始节点
        namespace.mergedNode = mergedNode;
    }
    
    /**
     * 检查命名空间内的冲突
     */
    private void checkConflicts(MergedNamespace namespace) {
        // 检查定义冲突
        Map<String, List<Definition>> definitionsByName = new HashMap<>();
        
        for (Map.Entry<String, Definition> entry : namespace.definitions.entrySet()) {
            String name = entry.getValue().name;
            definitionsByName.computeIfAbsent(name, k -> new ArrayList<>()).add(entry.getValue());
        }
        
        for (Map.Entry<String, List<Definition>> entry : definitionsByName.entrySet()) {
            List<Definition> defs = entry.getValue();
            if (defs.size() > 1) {
                // 检查是否有不兼容的定义
                for (int i = 0; i < defs.size() - 1; i++) {
                    for (int j = i + 1; j < defs.size(); j++) {
                        if (!isCompatibleDefinition(defs.get(i).node, defs.get(j).node)) {
                            context.addError(new CompilationError(
                                "Conflicting definitions in namespace " + namespace.name + 
                                ": " + entry.getKey(),
                                CompilationError.ErrorType.DUPLICATE_DEFINITION
                            ));
                        }
                    }
                }
            }
        }
    }
    
    /**
     * 检查两个定义是否兼容
     */
    private boolean isCompatibleDefinition(CHTLASTNode def1, CHTLASTNode def2) {
        // 相同类型的定义才可能兼容
        if (def1.getNodeType() != def2.getNodeType()) {
            return false;
        }
        
        // 模板定义的兼容性检查
        if (def1 instanceof TemplateDefinitionNode && def2 instanceof TemplateDefinitionNode) {
            TemplateDefinitionNode t1 = (TemplateDefinitionNode) def1;
            TemplateDefinitionNode t2 = (TemplateDefinitionNode) def2;
            
            // 必须是相同类型和名称
            if (t1.getTemplateType() != t2.getTemplateType() || 
                !t1.getName().equals(t2.getName())) {
                return false;
            }
            
            // 检查内容是否兼容（简化：只检查子节点数量）
            return t1.getChildren().size() == t2.getChildren().size();
        }
        
        // 自定义定义的兼容性检查
        if (def1 instanceof CustomDefinitionNode && def2 instanceof CustomDefinitionNode) {
            CustomDefinitionNode c1 = (CustomDefinitionNode) def1;
            CustomDefinitionNode c2 = (CustomDefinitionNode) def2;
            
            // 必须是相同类型和名称
            if (c1.getCustomType() != c2.getCustomType() || 
                !c1.getName().equals(c2.getName())) {
                return false;
            }
            
            // 检查空属性列表是否相同
            return new HashSet<>(c1.getEmptyProperties()).equals(
                   new HashSet<>(c2.getEmptyProperties()));
        }
        
        // 其他类型暂时认为不兼容
        return false;
    }
    
    /**
     * 获取子节点的键
     */
    private String getChildKey(CHTLASTNode node) {
        if (node instanceof TemplateDefinitionNode) {
            TemplateDefinitionNode template = (TemplateDefinitionNode) node;
            return "template:" + template.getTemplateType() + ":" + template.getName();
        } else if (node instanceof CustomDefinitionNode) {
            CustomDefinitionNode custom = (CustomDefinitionNode) node;
            return "custom:" + custom.getCustomType() + ":" + custom.getName();
        } else if (node instanceof ElementNode) {
            ElementNode element = (ElementNode) node;
            return "element:" + element.getTagName();
        } else {
            return node.getNodeType().toString();
        }
    }
    
    /**
     * 合并子节点
     */
    private CHTLASTNode mergeChildren(List<CHTLASTNode> children) {
        if (children.isEmpty()) {
            return null;
        }
        
        // 检查是否所有子节点都兼容
        CHTLASTNode first = children.get(0);
        for (int i = 1; i < children.size(); i++) {
            if (!isCompatibleDefinition(first, children.get(i))) {
                context.addError(new CompilationError(
                    "Cannot merge incompatible definitions",
                    CompilationError.ErrorType.SEMANTIC_ERROR
                ));
                return first; // 返回第一个作为默认
            }
        }
        
        // 如果都兼容，返回第一个（简化处理）
        context.addWarning(new CompilationWarning(
            "Multiple compatible definitions found, using first",
            CompilationWarning.WarningType.POTENTIAL_ERROR
        ));
        
        return first;
    }
    
    /**
     * 获取所有命名空间
     */
    public Collection<MergedNamespace> getAllNamespaces() {
        return namespaces.values();
    }
    
    /**
     * 合并的命名空间
     */
    public static class MergedNamespace {
        private final String name;
        private final List<NamespaceNode> nodes;
        private final Map<String, Definition> definitions;
        private NamespaceNode mergedNode;
        
        public MergedNamespace(String name) {
            this.name = name;
            this.nodes = new ArrayList<>();
            this.definitions = new HashMap<>();
        }
        
        public void addNamespaceNode(NamespaceNode node) {
            nodes.add(node);
        }
        
        public void addDefinition(String name, CHTLASTNode node, DefinitionType type) {
            String key = type.name() + ":" + name;
            definitions.put(key, new Definition(name, node, type));
        }
        
        public Definition getDefinition(String name, DefinitionType type) {
            String key = type.name() + ":" + name;
            return definitions.get(key);
        }
        
        public String getName() {
            return name;
        }
        
        public List<NamespaceNode> getNodes() {
            return Collections.unmodifiableList(nodes);
        }
        
        public NamespaceNode getMergedNode() {
            return mergedNode != null ? mergedNode : (nodes.isEmpty() ? null : nodes.get(0));
        }
    }
    
    /**
     * 定义
     */
    public static class Definition {
        private final String name;
        private final CHTLASTNode node;
        private final DefinitionType type;
        
        public Definition(String name, CHTLASTNode node, DefinitionType type) {
            this.name = name;
            this.node = node;
            this.type = type;
        }
        
        public String getName() {
            return name;
        }
        
        public CHTLASTNode getNode() {
            return node;
        }
        
        public DefinitionType getType() {
            return type;
        }
    }
    
    /**
     * 定义类型
     */
    public enum DefinitionType {
        TEMPLATE_STYLE,
        TEMPLATE_ELEMENT,
        TEMPLATE_VAR,
        CUSTOM_STYLE,
        CUSTOM_ELEMENT,
        CUSTOM_VAR,
        ELEMENT,
        VARIABLE,
        FUNCTION
    }
}