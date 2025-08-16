package com.chtl.compiler.style;

import com.chtl.context.WarningType;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.node.CustomDefinitionNode;
import com.chtl.ast.node.CustomOperationNode;
import com.chtl.ast.node.CustomUsageNode;
import com.chtl.ast.node.InheritStatementNode;
import com.chtl.ast.node.NumberLiteralNode;
import com.chtl.ast.node.StringLiteralNode;
import com.chtl.ast.node.StylePropertyNode;
import com.chtl.ast.node.TemplateUsageNode;
import com.chtl.ast.node.UnquotedLiteralNode;
import com.chtl.context.CompilationWarning;
import com.chtl.ast.*;
import com.chtl.context.CompilationContext;

/**
 * 样式继承处理器
 * 负责处理样式组之间的继承关系
 * 严格遵循CHTL语法文档
 */
public class StyleInheritanceProcessor {
    private final CompilationContext context;
    
    public StyleInheritanceProcessor(CompilationContext context) {
        this.context = context;
    }
    
    /**
     * 处理样式继承
     * 根据CHTL语法文档：
     * 1. 使用 @Style 组名 进行继承
     * 2. 使用 inherit @Style 组名 进行显式继承
     * 3. 后定义的属性覆盖先定义的属性
     * 4. 支持删除继承的属性或整个继承
     */
    public InheritanceResult processInheritance(
            CHTLASTNode node, 
            StyleGroupManager groupManager) {
        
        InheritanceResult result = new InheritanceResult();
        
        // 收集所有继承
        collectInheritances(node, result);
        
        // 解析继承链
        for (String inheritedName : result.inheritedGroups) {
            StyleGroupManager.ResolvedStyleGroup resolved = 
                groupManager.resolveStyleGroup(inheritedName);
            
            if (resolved != null) {
                // 合并属性（后定义覆盖先定义）
                for (Map.Entry<String, String> entry : resolved.properties.entrySet()) {
                    if (!result.deletedProperties.contains(entry.getKey())) {
                        result.properties.put(entry.getKey(), entry.getValue());
                    }
                }
            }
        }
        
        // 应用本地属性（覆盖继承的）
        applyLocalProperties(node, result);
        
        return result;
    }
    
    /**
     * 收集继承信息
     */
    private void collectInheritances(CHTLASTNode node, InheritanceResult result) {
        for (CHTLASTNode child : node.getChildren()) {
            if (child instanceof CustomUsageNode) {
                CustomUsageNode usage = (CustomUsageNode) child;
                String name = usage.getName();
                
                // 检查是否有特例化
                if (!usage.getChildren().isEmpty()) {
                    // 处理特例化中的删除操作
                    processSpecialization(usage, result);
                } else {
                    // 普通继承
                    result.inheritedGroups.add(name);
                }
                
            } else if (child instanceof TemplateUsageNode) {
                TemplateUsageNode usage = (TemplateUsageNode) child;
                result.inheritedGroups.add(usage.getName());
                
            } else if (child instanceof InheritStatementNode) {
                // 显式继承节点
                InheritStatementNode inherit = (InheritStatementNode) child;
                result.inheritedGroups.add(inherit.getTargetName());
            }
        }
    }
    
    /**
     * 处理特例化
     */
    private void processSpecialization(CustomUsageNode usage, InheritanceResult result) {
        String groupName = usage.getName();
        boolean shouldInherit = true;
        
        for (CHTLASTNode child : usage.getChildren()) {
            if (child instanceof CustomOperationNode) {
                CustomOperationNode operation = (CustomOperationNode) child;
                
                if (operation.getOperationType() == CustomOperationNode.OperationType.DELETE) {
                    // 处理删除操作
                    for (String target : operation.getDeleteTargets()) {
                        if (target.equals("@Style " + groupName)) {
                            // 删除整个继承
                            shouldInherit = false;
                        } else {
                            // 删除特定属性
                            result.deletedProperties.add(target);
                        }
                    }
                }
            }
        }
        
        if (shouldInherit) {
            result.inheritedGroups.add(groupName);
        }
    }
    
    /**
     * 应用本地属性
     */
    private void applyLocalProperties(CHTLASTNode node, InheritanceResult result) {
        for (CHTLASTNode child : node.getChildren()) {
            if (child instanceof StylePropertyNode) {
                StylePropertyNode property = (StylePropertyNode) child;
                String name = property.getProperty();
                String value = getStringValue(property.getValue());
                
                // 本地属性覆盖继承的属性
                result.properties.put(name, value);
                
                // 如果之前被删除了，现在重新定义了，移除删除标记
                result.deletedProperties.remove(name);
            }
        }
    }
    
    /**
     * 处理无值属性的特例化
     * [Custom] @Style TextSet { color, font-size; }
     */
    public Map<String, String> processNoValueProperties(
            CustomDefinitionNode styleGroup,
            Map<String, String> providedValues) {
        
        Map<String, String> result = new HashMap<Object, Object>();
        
        for (CHTLASTNode child : styleGroup.getChildren()) {
            if (child instanceof StylePropertyNode) {
                StylePropertyNode property = (StylePropertyNode) child;
                String name = property.getProperty();
                
                if (property.getValue() == null) {
                    // 无值属性，从提供的值中查找
                    String value = providedValues.get(name);
                    if (value != null) {
                        result.put(name, value);
                    } else {
                        context.addWarning(new com.chtl.context.CompilationWarning(
                            "无值属性 '" + name + "' 未提供值",
                            com.chtl.context.CompilationWarning.WarningType.MISSING_VALUE
                        ));
                    }
                } else {
                    // 有值属性，直接使用
                    result.put(name, getStringValue(property.getValue()));
                }
            }
        }
        
        return result;
    }
    
    /**
     * 继承处理结果
     */
    public static class InheritanceResult {
        // 继承的样式组名称列表
        public final List<String> inheritedGroups = new ArrayList<Object>();
        
        // 合并后的属性
        public final Map<String, String> properties = new LinkedHashMap<Object, Object>();
        
        // 被删除的属性
        public final Set<String> deletedProperties = new HashSet<Object>();
        
        // 被删除的继承
        public final Set<String> deletedInheritances = new HashSet<Object>();
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
