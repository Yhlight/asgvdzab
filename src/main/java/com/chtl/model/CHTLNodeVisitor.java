package com.chtl.model;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class CHTLNodeVisitor extends AbstractCHTLASTNode {
public interface CHTLNodeVisitor {
    void visitTemplate(TemplateNode node);
    void visitScript(ScriptNode node);
    void visitStyle(StyleNode node);
    void visitCustom(CustomNode node);
}

}