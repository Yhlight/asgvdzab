package com.chtl.ast.node;

import com.chtl.ast.*;
import java.util.HashMap;
import java.util.Map;

/**
 * 变量组定义节点
 * 对应CHTL中的[Template] @Var或[Custom] @Var定义
 */
public class VarGroupDefinitionNode extends AbstractCHTLASTNode {
    private String name;
    private boolean isTemplate; // true为Template，false为Custom
    private Map<String, CHTLASTNode> variables; // 变量名到值的映射
    
    public VarGroupDefinitionNode(String name, boolean isTemplate) {
        super(NodeType.VAR_GROUP_DEFINITION);
        this.name = name;
        this.isTemplate = isTemplate;
        this.variables = new HashMap<>();
    }
    
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    public boolean isTemplate() {
        return isTemplate;
    }
    
    public void setTemplate(boolean template) {
        isTemplate = template;
    }
    
    public Map<String, CHTLASTNode> getVariables() {
        return variables;
    }
    
    public void addVariable(String varName, CHTLASTNode value) {
        variables.put(varName, value);
        if (value != null) {
            value.setParent(this);
        }
    }
    
    public CHTLASTNode getVariable(String varName) {
        return variables.get(varName);
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitVarGroupDefinition(this);
    }
}