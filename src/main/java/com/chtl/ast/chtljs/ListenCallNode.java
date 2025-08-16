package com.chtl.ast.chtljs;
import com.chtl.ast.NodeType;

import java.util.HashMap;
import java.util.Map;

/**
 * listen方法调用节点
 * 对应CHTL JS中的->listen({...})
 */
public class ListenCallNode extends AbstractCHTLJSASTNode {
    private CHTLJSASTNode target; // 调用listen的目标
    private Map<String, EventHandlerNode> eventHandlers;
    
    public ListenCallNode() {
        super(NodeType.LISTEN_CALL);
        this.eventHandlers = new HashMap<Object, Object>();
    }
    
    public CHTLJSASTNode getTarget() {
        return target;
    }
    
    public void setTarget(CHTLJSASTNode target) {
        this.target = target;
    }
    
    public Map<String, EventHandlerNode> getEventHandlers() {
        return eventHandlers;
    }
    
    public void addEventHandler(String eventName, EventHandlerNode handler) {
        eventHandlers.put(eventName, handler);
    }
    
    @Override
    public void accept(CHTLJSASTVisitor visitor) {
        visitor.visitListenCall(this);
    }
}