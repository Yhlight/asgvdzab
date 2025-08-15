package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class TemplateUsageNode extends AbstractCHTLASTNode {
    private String templateName;

    public TemplateUsageNode(String templateName) {
        super(NodeType.TEMPLATE_USAGE);
        this.templateName = templateName;
    }

    public String getTemplateName() { return templateName; }

}