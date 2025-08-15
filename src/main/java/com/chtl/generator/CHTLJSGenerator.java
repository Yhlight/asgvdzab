package com.chtl.generator;

import com.chtl.ast.chtljs.*;
import java.util.*;

/**
 * CHTL JS代码生成器
 * 从CHTL JS AST生成JavaScript代码
 */
public class CHTLJSGenerator implements CHTLJSASTVisitor {
    private StringBuilder output;
    private int selectorIdCounter = 0;
    private Map<String, String> selectorCache; // 缓存选择器查询结果
    
    // 事件委托管理
    private Map<String, List<DelegationInfo>> delegationMap;
    
    public CHTLJSGenerator() {
        this.selectorCache = new HashMap<>();
        this.delegationMap = new HashMap<>();
    }
    
    /**
     * 生成JavaScript代码
     */
    public String generate(List<CHTLJSASTNode> statements) {
        this.output = new StringBuilder();
        this.selectorIdCounter = 0;
        this.selectorCache.clear();
        this.delegationMap.clear();
        
        // 生成CHTL辅助对象
        generateCHTLHelper();
        
        // 处理所有语句
        for (CHTLJSASTNode statement : statements) {
            statement.accept(this);
            output.append(";\n");
        }
        
        // 生成事件委托代码
        generateDelegationCode();
        
        return output.toString();
    }
    
    @Override
    public void visitEnhancedSelector(EnhancedSelectorNode node) {
        String selector = node.getSelector();
        String jsCode = transformSelector(node);
        output.append(jsCode);
    }
    
    @Override
    public void visitListenCall(ListenCallNode node) {
        // 获取目标元素
        String targetVar = generateTempVar();
        output.append("const ").append(targetVar).append(" = ");
        node.getTarget().accept(this);
        output.append(";\n");
        
        // 为每个事件绑定处理器
        for (Map.Entry<String, EventHandlerNode> entry : node.getEventHandlers().entrySet()) {
            String eventName = entry.getKey();
            EventHandlerNode handler = entry.getValue();
            
            output.append(targetVar).append(".addEventListener('").append(eventName).append("', ");
            generateEventHandler(handler);
            output.append(")");
        }
    }
    
    @Override
    public void visitDelegateCall(DelegateCallNode node) {
        // 获取父元素
        String parentVar = generateTempVar();
        output.append("const ").append(parentVar).append(" = ");
        node.getParentElement().accept(this);
        output.append(";\n");
        
        // 获取目标选择器
        List<String> targetSelectors = new ArrayList<>();
        for (CHTLJSASTNode target : node.getTargetElements()) {
            if (target instanceof EnhancedSelectorNode) {
                EnhancedSelectorNode selectorNode = (EnhancedSelectorNode) target;
                targetSelectors.add(transformSelectorToCSS(selectorNode));
            }
        }
        
        // 注册事件委托
        String parentId = getOrCreateParentId(parentVar);
        
        for (Map.Entry<String, EventHandlerNode> entry : node.getEventHandlers().entrySet()) {
            String eventName = entry.getKey();
            EventHandlerNode handler = entry.getValue();
            
            DelegationInfo info = new DelegationInfo();
            info.parentId = parentId;
            info.targetSelectors = targetSelectors;
            info.eventName = eventName;
            info.handler = handler;
            
            delegationMap.computeIfAbsent(parentId, k -> new ArrayList<>()).add(info);
        }
    }
    
    @Override
    public void visitAnimateCall(AnimateCallNode node) {
        output.append("_chtl.animate(");
        generateAnimationConfig(node.getConfig());
        output.append(")");
    }
    
    @Override
    public void visitAnimationController(AnimationControllerNode node) {
        // 动画控制器在animate函数中生成
        // 这里通常不会被直接调用
    }
    
    @Override
    public void visitEventHandler(EventHandlerNode node) {
        generateEventHandler(node);
    }
    
    @Override
    public void visitAnimationConfig(AnimationConfigNode node) {
        generateAnimationConfig(node);
    }
    
    @Override
    public void visitAnimationKeyframe(AnimationKeyframeNode node) {
        output.append("{ at: ").append(node.getAt());
        
        for (Map.Entry<String, String> entry : node.getCssProperties().entrySet()) {
            output.append(", '").append(entry.getKey()).append("': ");
            output.append("'").append(entry.getValue()).append("'");
        }
        
        output.append(" }");
    }
    
    @Override
    public void visitArrowOperation(ArrowOperationNode node) {
        node.getLeft().accept(this);
        output.append(".");
        node.getRight().accept(this);
    }
    
    @Override
    public void visitPropertyAccess(PropertyAccessNode node) {
        if (node.getObject() != null) {
            node.getObject().accept(this);
            output.append(".");
        }
        output.append(node.getProperty());
    }
    
    @Override
    public void visitExpression(ExpressionNode node) {
        output.append(node.getExpression());
    }
    
    // 辅助方法
    
    private void generateCHTLHelper() {
        output.append("// CHTL JS Helper Object\n");
        output.append("const _chtl = {\n");
        
        // 选择器查询函数
        output.append("    query: function(selector) {\n");
        output.append("        if (selector.startsWith('.')) {\n");
        output.append("            return document.querySelector(selector);\n");
        output.append("        } else if (selector.startsWith('#')) {\n");
        output.append("            return document.getElementById(selector.substring(1));\n");
        output.append("        } else if (selector.includes(' ')) {\n");
        output.append("            return document.querySelector(selector);\n");
        output.append("        } else if (selector.includes('[') && selector.includes(']')) {\n");
        output.append("            const match = selector.match(/^(\\w+)\\[(\\d+)\\]$/);\n");
        output.append("            if (match) {\n");
        output.append("                const elements = document.getElementsByTagName(match[1]);\n");
        output.append("                return elements[parseInt(match[2])];\n");
        output.append("            }\n");
        output.append("        } else {\n");
        output.append("            // 先尝试作为ID\n");
        output.append("            let element = document.getElementById(selector);\n");
        output.append("            if (!element) {\n");
        output.append("                // 再尝试作为类名\n");
        output.append("                element = document.querySelector('.' + selector);\n");
        output.append("            }\n");
        output.append("            if (!element) {\n");
        output.append("                // 最后作为标签名\n");
        output.append("                element = document.querySelector(selector);\n");
        output.append("            }\n");
        output.append("            return element;\n");
        output.append("        }\n");
        output.append("    },\n\n");
        
        // 动画函数
        output.append("    animate: function(config) {\n");
        output.append("        const duration = config.duration || 1000;\n");
        output.append("        const easing = config.easing || 'linear';\n");
        output.append("        const begin = config.begin || {};\n");
        output.append("        const end = config.end || {};\n");
        output.append("        const keyframes = config.when || [];\n");
        output.append("        const loop = config.loop || 1;\n");
        output.append("        const delay = config.delay || 0;\n");
        output.append("        const direction = config.direction || 'normal';\n");
        output.append("        const callback = config.callback;\n");
        output.append("        \n");
        output.append("        // 创建动画控制对象\n");
        output.append("        let animationId = null;\n");
        output.append("        let isPaused = false;\n");
        output.append("        let isPlaying = false;\n");
        output.append("        let currentLoop = 0;\n");
        output.append("        let startTime = null;\n");
        output.append("        let pausedTime = 0;\n");
        output.append("        \n");
        output.append("        const controller = {\n");
        output.append("            play: function() {\n");
        output.append("                if (!isPlaying) {\n");
        output.append("                    isPlaying = true;\n");
        output.append("                    isPaused = false;\n");
        output.append("                    startAnimation();\n");
        output.append("                }\n");
        output.append("                return this;\n");
        output.append("            },\n");
        output.append("            \n");
        output.append("            pause: function() {\n");
        output.append("                if (isPlaying && !isPaused) {\n");
        output.append("                    isPaused = true;\n");
        output.append("                    if (animationId) {\n");
        output.append("                        cancelAnimationFrame(animationId);\n");
        output.append("                        animationId = null;\n");
        output.append("                    }\n");
        output.append("                }\n");
        output.append("                return this;\n");
        output.append("            },\n");
        output.append("            \n");
        output.append("            stop: function() {\n");
        output.append("                isPlaying = false;\n");
        output.append("                isPaused = false;\n");
        output.append("                if (animationId) {\n");
        output.append("                    cancelAnimationFrame(animationId);\n");
        output.append("                    animationId = null;\n");
        output.append("                }\n");
        output.append("                // 重置到初始状态\n");
        output.append("                applyStyles(begin);\n");
        output.append("                return this;\n");
        output.append("            },\n");
        output.append("            \n");
        output.append("            reverse: function() {\n");
        output.append("                // 交换开始和结束状态\n");
        output.append("                [begin, end] = [end, begin];\n");
        output.append("                return this;\n");
        output.append("            },\n");
        output.append("            \n");
        output.append("            onComplete: function(fn) {\n");
        output.append("                const originalCallback = callback;\n");
        output.append("                config.callback = function() {\n");
        output.append("                    if (originalCallback) originalCallback();\n");
        output.append("                    if (fn) fn();\n");
        output.append("                };\n");
        output.append("                return this;\n");
        output.append("            },\n");
        output.append("            \n");
        output.append("            isPlaying: function() {\n");
        output.append("                return isPlaying && !isPaused;\n");
        output.append("            },\n");
        output.append("            \n");
        output.append("            isPaused: function() {\n");
        output.append("                return isPaused;\n");
        output.append("            }\n");
        output.append("        };\n");
        output.append("        \n");
        output.append("        // 应用样式的辅助函数\n");
        output.append("        function applyStyles(styles) {\n");
        output.append("            // TODO: 实现样式应用逻辑\n");
        output.append("        }\n");
        output.append("        \n");
        output.append("        // 启动动画的函数\n");
        output.append("        function startAnimation() {\n");
        output.append("            setTimeout(() => {\n");
        output.append("                if (!isPlaying) return;\n");
        output.append("                startTime = performance.now() - pausedTime;\n");
        output.append("                animate();\n");
        output.append("            }, delay);\n");
        output.append("        }\n");
        output.append("        \n");
        output.append("        // 动画主循环\n");
        output.append("        function animate() {\n");
        output.append("            if (!isPlaying || isPaused) return;\n");
        output.append("            \n");
        output.append("            const currentTime = performance.now();\n");
        output.append("            const elapsed = currentTime - startTime;\n");
        output.append("            const progress = Math.min(elapsed / duration, 1);\n");
        output.append("            \n");
        output.append("            // TODO: 实现缓动函数和关键帧插值\n");
        output.append("            \n");
        output.append("            if (progress < 1) {\n");
        output.append("                animationId = requestAnimationFrame(animate);\n");
        output.append("            } else {\n");
        output.append("                // 动画完成\n");
        output.append("                currentLoop++;\n");
        output.append("                if (loop === -1 || currentLoop < loop) {\n");
        output.append("                    startTime = performance.now();\n");
        output.append("                    animate();\n");
        output.append("                } else {\n");
        output.append("                    isPlaying = false;\n");
        output.append("                    if (config.callback) config.callback();\n");
        output.append("                }\n");
        output.append("            }\n");
        output.append("        }\n");
        output.append("        \n");
        output.append("        // 自动开始动画\n");
        output.append("        controller.play();\n");
        output.append("        \n");
        output.append("        return controller;\n");
        output.append("    }\n");
        
        output.append("};\n\n");
    }
    
    private String transformSelector(EnhancedSelectorNode node) {
        String selector = node.getSelector();
        
        // 检查缓存
        if (selectorCache.containsKey(selector)) {
            return selectorCache.get(selector);
        }
        
        String varName = "_sel" + (++selectorIdCounter);
        String queryCode = "_chtl.query('" + selector + "')";
        
        selectorCache.put(selector, varName);
        
        // 生成查询代码
        output.append("const ").append(varName).append(" = ").append(queryCode).append(";\n");
        
        return varName;
    }
    
    private String transformSelectorToCSS(EnhancedSelectorNode node) {
        String selector = node.getSelector();
        
        switch (node.getSelectorType()) {
            case CLASS:
                return selector; // 已经是.className格式
            case ID:
                return selector; // 已经是#idName格式
            case TAG:
                return selector;
            case INDEX:
                return node.getTagName(); // 索引访问转为标签选择器
            case DESCENDANT:
                return selector;
            default:
                return selector;
        }
    }
    
    private void generateEventHandler(EventHandlerNode handler) {
        switch (handler.getHandlerType()) {
            case FUNCTION_EXPRESSION:
                output.append(handler.getHandlerCode());
                break;
            case FUNCTION_REFERENCE:
                output.append(handler.getHandlerCode());
                break;
        }
    }
    
    private void generateAnimationConfig(AnimationConfigNode config) {
        output.append("{\n");
        
        if (config.getDuration() != null) {
            output.append("    duration: ").append(config.getDuration()).append(",\n");
        }
        
        if (config.getEasing() != null) {
            output.append("    easing: '").append(config.getEasing()).append("',\n");
        }
        
        if (!config.getBeginState().isEmpty()) {
            output.append("    begin: {\n");
            for (Map.Entry<String, String> entry : config.getBeginState().entrySet()) {
                output.append("        '").append(entry.getKey()).append("': ");
                output.append(entry.getValue()).append(",\n");
            }
            output.append("    },\n");
        }
        
        if (!config.getEndState().isEmpty()) {
            output.append("    end: {\n");
            for (Map.Entry<String, String> entry : config.getEndState().entrySet()) {
                output.append("        '").append(entry.getKey()).append("': ");
                output.append(entry.getValue()).append(",\n");
            }
            output.append("    },\n");
        }
        
        if (!config.getKeyframes().isEmpty()) {
            output.append("    when: [\n");
            for (AnimationKeyframeNode keyframe : config.getKeyframes()) {
                output.append("        ");
                keyframe.accept(this);
                output.append(",\n");
            }
            output.append("    ],\n");
        }
        
        if (config.getLoop() != null) {
            output.append("    loop: ").append(config.getLoop()).append(",\n");
        }
        
        if (config.getDirection() != null) {
            output.append("    direction: '").append(config.getDirection()).append("',\n");
        }
        
        if (config.getDelay() != null) {
            output.append("    delay: ").append(config.getDelay()).append(",\n");
        }
        
        if (config.getCallback() != null) {
            output.append("    callback: ").append(config.getCallback()).append("\n");
        }
        
        output.append("}");
    }
    
    private void generateDelegationCode() {
        if (delegationMap.isEmpty()) {
            return;
        }
        
        output.append("\n// Event Delegation\n");
        
        for (Map.Entry<String, List<DelegationInfo>> entry : delegationMap.entrySet()) {
            String parentId = entry.getKey();
            List<DelegationInfo> delegations = entry.getValue();
            
            // 按事件类型分组
            Map<String, List<DelegationInfo>> byEvent = new HashMap<>();
            for (DelegationInfo info : delegations) {
                byEvent.computeIfAbsent(info.eventName, k -> new ArrayList<>()).add(info);
            }
            
            // 为每个事件类型生成委托代码
            for (Map.Entry<String, List<DelegationInfo>> eventEntry : byEvent.entrySet()) {
                String eventName = eventEntry.getKey();
                List<DelegationInfo> infos = eventEntry.getValue();
                
                output.append("document.getElementById('").append(parentId).append("')");
                output.append(".addEventListener('").append(eventName).append("', function(event) {\n");
                
                for (DelegationInfo info : infos) {
                    for (String selector : info.targetSelectors) {
                        output.append("    if (event.target.matches('").append(selector).append("')) {\n");
                        output.append("        ");
                        generateEventHandler(info.handler);
                        output.append(".call(event.target, event);\n");
                        output.append("    }\n");
                    }
                }
                
                output.append("});\n");
            }
        }
    }
    
    private String generateTempVar() {
        return "_tmp" + (++selectorIdCounter);
    }
    
    private String getOrCreateParentId(String parentVar) {
        // TODO: 实现获取或创建父元素ID的逻辑
        return "parent-" + parentVar;
    }
    
    /**
     * 事件委托信息
     */
    private static class DelegationInfo {
        String parentId;
        List<String> targetSelectors;
        String eventName;
        EventHandlerNode handler;
    }
}