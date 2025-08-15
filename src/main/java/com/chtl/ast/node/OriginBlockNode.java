package com.chtl.ast.node;import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.*;

/**
 * 原始嵌入块节点
 * 对应CHTL中的[Origin] @Type { }
 */
public class OriginBlockNode extends AbstractCHTLASTNode {
    
    public enum OriginType {
        HTML,       // @Html
        STYLE,      // @Style
        JAVASCRIPT  // @JavaScript
    }
    
    private OriginType originType;
    private String name; // 可选的名称，用于增强原始嵌入
    private String content;
    
    public OriginBlockNode(OriginType originType) {
        super(NodeType.ORIGIN_BLOCK);
        this.originType = originType;
        this.content = "";
    }
    
    public OriginBlockNode(OriginType originType, String name) {
        super(NodeType.ORIGIN_BLOCK);
        this.originType = originType;
        this.name = name;
        this.content = "";
    }
    
    public OriginType getOriginType() {
        return originType;
    }
    
    public void setOriginType(OriginType originType) {
        this.originType = originType;
    }
    
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    public String getContent() {
        return content;
    }
    
    public void setContent(String content) {
        this.content = content;
    }
    
    public void appendContent(String text) {
        this.content += text;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitOriginBlock(this);
    }
}