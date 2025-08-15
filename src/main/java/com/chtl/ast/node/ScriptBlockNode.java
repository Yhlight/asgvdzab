package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class ScriptBlockNode extends AbstractCHTLASTNode {
    private String content;
    private boolean isLocal;

    public ScriptBlockNode(String content, boolean isLocal) {
        super(NodeType.SCRIPT_BLOCK);
        this.content = content;
        this.isLocal = isLocal;
    }

    public String getContent() { return content; }
    public boolean getIsLocal() { return isLocal; }

}