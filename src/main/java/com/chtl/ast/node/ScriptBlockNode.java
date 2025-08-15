package com.chtl.ast.node;import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.*;

/**
 * 脚本块节点
 * 对应CHTL中的script { }块
 */
public class ScriptBlockNode extends AbstractCHTLASTNode {
    private String content;
    private boolean isLocal; // true表示局部脚本块，false表示全局脚本块
    private boolean isCHTLJS; // true表示包含CHTL JS语法
    
    public ScriptBlockNode(boolean isLocal) {
        super(NodeType.SCRIPT_BLOCK);
        this.isLocal = isLocal;
        this.content = "";
        this.isCHTLJS = false;
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
    
    public boolean isLocal() {
        return isLocal;
    }
    
    public void setLocal(boolean local) {
        isLocal = local;
    }
    
    public boolean isCHTLJS() {
        return isCHTLJS;
    }
    
    public void setCHTLJS(boolean CHTLJS) {
        isCHTLJS = CHTLJS;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitScriptBlock(this);
    }
}