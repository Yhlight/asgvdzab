package com.chtl.model;


import com.chtl.model.CHTLNode;
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
    
    
}