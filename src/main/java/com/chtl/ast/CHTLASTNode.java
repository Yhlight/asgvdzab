package com.chtl.ast;

import com.chtl.model.SourceLocation;
import java.util.List;

/**
 * Base interface for all CHTL AST nodes
 */
public interface CHTLASTNode {
    /**
     * Get the type of this node
     */
    NodeType getNodeType();

    /**
     * Get child nodes
     */
    List<CHTLASTNode> getChildren();

    /**
     * Add a child node
     */
    void addChild(CHTLASTNode child);

    /**
     * Get parent node
     */
    CHTLASTNode getParent();

    /**
     * Set parent node
     */
    void setParent(CHTLASTNode parent);

    /**
     * Get source location
     */
    SourceLocation getLocation();

    /**
     * Set source location
     */
    void setLocation(SourceLocation location);

    /**
     * Accept a visitor
     */
    void accept(CHTLASTVisitor visitor);
}
