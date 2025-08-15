package com.chtl.ast.node;
import com.chtl.ast.NodeType;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.AbstractCHTLASTNode;

import com.chtl.model.Text;

public class TextNode extends AbstractCHTLASTNode {
    private Text text;

    public TextNode(Text text) {
        super(NodeType.TEXT);
        this.text = text;
    }

    public Text getText() {
        return text;
    }
}
