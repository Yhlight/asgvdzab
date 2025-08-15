package com.chtl.ast.chtljs;
import com.chtl.ast.NodeType;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * delegate方法调用节点
 * 对应CHTL JS中的->delegate({...})
 */
public class DelegateCallNode extends AbstractCHTLJSASTNode {
    private CHTLJSASTNode parentElement; // 父元素
    private List<CHTLJSASTNode> targetElements; // 要代理的子元素
    private Map<String, EventHandlerNode> eventHandlers;
    
    public DelegateCallNode() {
        super(NodeType.DELEGATE_CALL);
        this.targetElements = new ArrayList<>();
        this.eventHandlers = new HashMap<>();
    }
    
    public CHTLJSASTNode getParentElement() {
        return parentElement;
    }
    
    public void setParentElement(CHTLJSASTNode parentElement) {
        this.parentElement = parentElement;
    }
    
    public List<CHTLJSASTNode> getTargetElements() {
        return targetElements;
    }
    
    public void addTargetElement(CHTLJSASTNode target) {
        if (target != null) {
            targetElements.add(target);
        }
    }
    
    public Map<String, EventHandlerNode> getEventHandlers() {
        return eventHandlers;
    }
    
    public void addEventHandler(String eventName, EventHandlerNode handler) {
        eventHandlers.put(eventName, handler);
    }
    
    @Override
    public void accept(CHTLJSASTVisitor visitor) {
        visitor.visitDelegateCall(this);
    }
}