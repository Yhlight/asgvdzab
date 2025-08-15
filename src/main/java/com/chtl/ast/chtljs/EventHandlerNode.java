package com.chtl.ast.chtljs;

/**
 * 事件处理器节点
 * 对应listen或delegate中的事件处理函数
 */
public class EventHandlerNode extends AbstractCHTLJSASTNode {
    
    public enum HandlerType {
        FUNCTION_EXPRESSION,   // function() { }
        ARROW_FUNCTION,       // () => { }
        FUNCTION_REFERENCE    // 函数引用，如 handleClick
    }
    
    private String eventName;
    private HandlerType handlerType;
    private String handlerCode; // 处理函数的代码或引用名
    
    public EventHandlerNode(String eventName) {
        super(NodeType.EVENT_HANDLER);
        this.eventName = eventName;
    }
    
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
    
    @Override
    public void accept(CHTLJSASTVisitor visitor) {
        visitor.visitEventHandler(this);
    }
}