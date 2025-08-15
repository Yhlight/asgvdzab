package com.chtl.ast.chtljs;

/**
 * CHTL JS事件处理器节点
 */
public class EventHandlerNode extends AbstractCHTLJSASTNode {
    private String eventName;
    private HandlerType handlerType;
    private String handlerCode;
    
    public enum HandlerType {
        FUNCTION_EXPRESSION,  // function() { }
        FUNCTION_REFERENCE    // functionName
    }
    
    public EventHandlerNode(String eventName) {
        super(NodeType.EVENT_HANDLER);
        this.eventName = eventName;
    }
    
    @Override
    public void accept(CHTLJSASTVisitor visitor) {
        visitor.visitEventHandler(this);
    }
    
    // Getters and Setters
    public String getEventName() {
        return eventName;
    }
    
    public void setEventName(String eventName) {
        this.eventName = eventName;
    }
    
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