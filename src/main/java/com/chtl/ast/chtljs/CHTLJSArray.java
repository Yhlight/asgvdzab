package com.chtl.ast.chtljs;
import com.chtl.ast.NodeType;
import java.util.ArrayList;
import java.util.List;
import com.chtl.chtljs.ast.*;

public class CHTLJSArray extends AbstractCHTLJSASTNode {
    private List<CHTLJSASTNode> elements = new ArrayList<Object>();

    public CHTLJSArray() {
        super(CHTLJSASTNode.NodeType.ARRAY);
    }

    public void addElement(CHTLJSASTNode element) {
        elements.add(element);
    }

    public List<CHTLJSASTNode> getElements() {
        return elements;
    }
}
