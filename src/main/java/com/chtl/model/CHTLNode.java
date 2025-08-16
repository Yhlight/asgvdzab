package com.chtl.model;


import com.chtl.model.CHTLNode;
/**
 * Base class for CHTL model nodes
 */
public abstract class CHTLNode {
    /**
     * Accept a visitor
     */
    public abstract void accept(CHTLNodeVisitor visitor);
}
