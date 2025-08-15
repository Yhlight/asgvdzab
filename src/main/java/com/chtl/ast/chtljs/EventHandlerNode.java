package com.chtl.ast.chtljs;

/**
 * CHTL JS事件处理器节点
 */
public class EventHandlerNode extends CHTLJSASTNode {
    private HandlerType handlerType;
    private String handlerCode;
    
    public enum HandlerType {
        FUNCTION_EXPRESSION,  // function() { }
        FUNCTION_REFERENCE    // functionName
    }
    
    public EventHandlerNode() {
        super(NodeType.EVENT_HANDLER);
    }
    
    @Override
    public void accept(CHTLJSASTVisitor visitor) {
        visitor.visitEventHandler(this);
    }
    
    // Getters and Setters
    public HandlerType getHandlerType() {
        return handlerType;
    }
    
    public void setHandlerType(HandlerType handlerType) {
        this.handlerType = handlerType;
    }
    
    public String getHandlerCode() {
        return handlerCode;
    }
    
    public void setHandlerCode(String handlerCode) {
        this.handlerCode = handlerCode;
    }
}