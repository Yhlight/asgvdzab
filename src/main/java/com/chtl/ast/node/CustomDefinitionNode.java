package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

/**
 * 自定义定义节点
 * 对应CHTL中的[Custom] @Type Name { }
 * Custom支持特例化操作，比Template更灵活
 */
public class CustomDefinitionNode extends AbstractCHTLASTNode implements Serializable {
    private static final long serialVersionUID = 1L;
    
    /**
     * 自定义类型
     */
    public enum CustomType {
        ELEMENT("@Element"),
        STYLE("@Style"),
        VAR("@Var");
        
        private final String symbol;
        
        CustomType(String symbol) {
            this.symbol = symbol;
        }
        
        public String getSymbol() {
            return symbol;
        }
    }
    
    private CustomType customType;
    private String name;
    private List<CHTLASTNode> body = new ArrayList<>();
    private List<String> inherits = new ArrayList<>(); // 继承的模板或自定义
    private List<SpecializationNode> specializations = new ArrayList<>(); // 特例化操作
    
    public CustomDefinitionNode(CustomType customType, String name) {
        super(NodeType.CUSTOM_DEFINITION);
        this.customType = customType;
        this.name = name;
    }
    
    // Getters and setters
    public CustomType getCustomType() {
        return customType;
    }
    
    public void setCustomType(CustomType customType) {
        this.customType = customType;
    }
    
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    public List<CHTLASTNode> getBody() {
        return body;
    }
    
    public void addBodyNode(CHTLASTNode node) {
        this.body.add(node);
    }
    
    public List<String> getInherits() {
        return inherits;
    }
    
    public void addInherit(String inherit) {
        this.inherits.add(inherit);
    }
    
    public List<SpecializationNode> getSpecializations() {
        return specializations;
    }
    
    public void addSpecialization(SpecializationNode specialization) {
        this.specializations.add(specialization);
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitCustomDefinition(this);
    }
    
    @Override
    public String toString() {
        return String.format("[Custom] %s %s", customType.getSymbol(), name);
    }
}