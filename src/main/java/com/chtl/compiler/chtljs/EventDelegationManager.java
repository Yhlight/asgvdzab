package com.chtl.compiler.chtljs;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.chtl.compiler.chtljs.CHTLJSSystem.DelegationInfo;


import java.util.*;

/**
 * 事件委托管理器
 * 管理全局事件委托注册表
 */
public class EventDelegationManager {
    // 存储父元素和其委托的事件信息
    private final Map<String, DelegationInfo> delegationRegistry;
    
    public EventDelegationManager() {
        this.delegationRegistry = new HashMap<>();
    }
    
    /**
     * 注册事件委托
     */
    public void registerDelegation(String parentSelector, String targetSelector, 
                                  String eventType, String handlerName) {
        String key = parentSelector + "_" + eventType;
        
        DelegationInfo info = delegationRegistry.computeIfAbsent(key, 
            k -> new DelegationInfo(parentSelector, eventType));
        
        info.addTarget(targetSelector, handlerName);
    }
    
    /**
     * 生成事件委托初始化代码
     */
    public String generateInitializationCode() {
        if (delegationRegistry.isEmpty()) {
            return "";
        }
        
        StringBuilder code = new StringBuilder();
        code.append("// Event delegation initialization\n");
        code.append("document.addEventListener('DOMContentLoaded', function() {\n");
        
        for (DelegationInfo info : delegationRegistry.values()) {
            code.append(info.generateCode()).append("\n");
        }
        
        code.append("});\n");
        
        return code.toString();
    }
    
    /**
     * 事件委托信息内部类
     */
    private static class DelegationInfo {
        private final String parentSelector;
        private final String eventType;
        private final List<TargetHandler> targets;
        
        public DelegationInfo(String parentSelector, String eventType) {
            this.parentSelector = parentSelector;
            this.eventType = eventType;
            this.targets = new ArrayList<>();
        }
        
        public void addTarget(String targetSelector, String handlerName) {
            targets.add(new TargetHandler(targetSelector, handlerName));
        }
        
        public String generateCode() {
            StringBuilder code = new StringBuilder();
            code.append(String.format("  document.querySelector('%s').addEventListener('%s', function(e) {\n",
                parentSelector, eventType));
            
            for (TargetHandler target : targets) {
                code.append(String.format("    if (e.target.matches('%s')) {\n", target.selector));
                code.append(String.format("      %s.call(e.target, e);\n", target.handler));
                code.append("    }\n");
            }
            
            code.append("  });\n");
            
            return code.toString();
        }
    }
    
    /**
     * 目标和处理器对
     */
    private static class TargetHandler {
        final String selector;
        final String handler;
        
        TargetHandler(String selector, String handler) {
            this.selector = selector;
            this.handler = handler;
        }
    }
}
