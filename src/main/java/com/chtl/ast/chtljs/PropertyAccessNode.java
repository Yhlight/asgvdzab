package com.chtl.ast.chtljs;

/**
 * 属性访问节点
 * 对应CHTL JS中的属性访问，如.style.color
 */
public class PropertyAccessNode extends AbstractCHTLJSASTNode {
    private CHTLJSASTNode object; // 对象
    private String property;      // 属性名
    
    public PropertyAccessNode(String property) {
        super(NodeType.PROPERTY_ACCESS);
        this.property = property;
    }
    
    public CHTLJSASTNode getObject() {
        return object;
    }
    
    public void setObject(CHTLJSASTNode object) {
        this.object = object;
    }
    
    public String getProperty() {
        return property;
    }
    
    public void setProperty(String property) {
        this.property = property;
    }
    
    @Override
    public void accept(CHTLJSASTVisitor visitor) {
        visitor.visitPropertyAccess(this);
    }
}