package com.chtl.ast.node;
import com.chtl.ast.CHTLASTNode;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class ImportStatementNode extends AbstractCHTLASTNode {
    private String importType;
    private String source;

    public ImportStatementNode(String importType, String source) {
        super(NodeType.IMPORT_STATEMENT);
        this.importType = importType;
        this.source = source;
    }

    public String getImportType() { return importType; }
    public String getSource() { return source; }

}