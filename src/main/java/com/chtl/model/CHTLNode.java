package com.chtl.model;

/**
 * Base class for CHTL model nodes
 */
public abstract class CHTLNode {
    /**
     * Accept a visitor
     */
    public abstract void accept(CHTLNodeVisitor visitor);
}
