package com.chtl.ast.chtljs;
import java.util.ArrayList;
import java.util.List;


import com.chtl.chtljs.ast.*;
import java.util.*;

public class CHTLJSArray extends AbstractCHTLJSASTNode {
    private List<CHTLJSASTNode> elements = new ArrayList<>();

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
