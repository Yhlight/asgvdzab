package com.chtl.model;

/**
 * 脚本节点
 */
public class ScriptNode extends CHTLNode {
    private String content;
    private boolean isLocal;
    
    public ScriptNode(String content, boolean isLocal) {
        super("script");
        this.content = content;
        this.isLocal = isLocal;
    }
    
    public String getContent() {
        return content;
    }
    
    public void setContent(String content) {
        this.content = content;
    }
    
    public boolean isLocal() {
        return isLocal;
    }
    
    @Override
    public void accept(CHTLNodeVisitor visitor) {
        visitor.visit(this);
    }
}