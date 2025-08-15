package com.chtl.compiler.chtljs.event;

import com.chtl.ast.*;
import com.chtl.ast.node.*;
import com.chtl.compiler.chtljs.CHTLJSSystem;

import java.util.*;

/**
 * 事件绑定处理器
 * 负责处理listen和delegate方法
 * 严格遵循CHTL语法文档
 */
public class EventBindingProcessor {
    private final CHTLJSSystem jsSystem;
    
    public EventBindingProcessor(CHTLJSSystem jsSystem) {
        this.jsSystem = jsSystem;
    }
    
    /**
     * 处理listen方法
     * {{selector}}->listen({
     *     click: () => { ... },
     *     mouseenter: function() { ... }
     * });
     */
    public void processListenMethod(CHTLJSMethodCallNode methodCall, 
                                   String targetVar, 
                                   StringBuilder js) {
        List<CHTLASTNode> args = methodCall.getArguments();
        if (args.isEmpty()) {
            js.append("  ").append(targetVar).append(".listen({});\n");
            return;
        }
        
        js.append("  ").append(targetVar).append(".listen({\n");
        
        // 处理事件配置对象
        CHTLASTNode configArg = args.get(0);
        if (configArg instanceof CHTLJSObjectNode) {
            processEventConfig((CHTLJSObjectNode) configArg, js, "    ");
        }
        
        js.append("  });\n");
    }
    
    /**
     * 处理delegate方法
     * {{parent}}->delegate({
     *     target: {{selector}} | [{{selector1}}, {{selector2}}],
     *     click: function() { ... }
     * });
     */
    public void processDelegateMethod(CHTLJSMethodCallNode methodCall,
                                     String targetVar,
                                     StringBuilder js) {
        List<CHTLASTNode> args = methodCall.getArguments();
        if (args.isEmpty()) {
            js.append("  ").append(targetVar).append(".delegate({});\n");
            return;
        }
        
        js.append("  ").append(targetVar).append(".delegate({\n");
        
        // 处理委托配置对象
        CHTLASTNode configArg = args.get(0);
        if (configArg instanceof CHTLJSObjectNode) {
            processDelegateConfig((CHTLJSObjectNode) configArg, targetVar, js);
        }
        
        js.append("  });\n");
    }
    
    /**
     * 处理事件配置对象
     */
    private void processEventConfig(CHTLJSObjectNode config, 
                                   StringBuilder js, 
                                   String indent) {
        Map<String, CHTLASTNode> properties = config.getProperties();
        
        int count = 0;
        for (Map.Entry<String, CHTLASTNode> entry : properties.entrySet()) {
            String event = entry.getKey();
            CHTLASTNode handler = entry.getValue();
            
            if (count > 0) js.append(",\n");
            
            js.append(indent).append(event).append(": ");
            
            if (handler instanceof CHTLJSFunctionNode) {
                processFunctionHandler((CHTLJSFunctionNode) handler, js, indent);

            } else if (handler instanceof CHTLJSIdentifierNode) {
                // 引用已存在的函数
                js.append(((CHTLJSIdentifierNode) handler).getName());
            }
            
            count++;
        }
        
        if (count > 0) js.append("\n");
    }
    
    /**
     * 处理委托配置对象
     */
    private void processDelegateConfig(CHTLJSObjectNode config,
                                      String parentVar,
                                      StringBuilder js) {
        Map<String, CHTLASTNode> properties = config.getProperties();
        
        // 提取target
        CHTLASTNode targetNode = properties.get("target");
        if (targetNode != null) {
            js.append("    target: ");
            
            if (targetNode instanceof CHTLJSArrayNode) {
                processTargetArray((CHTLJSArrayNode) targetNode, js);
            } else if (targetNode instanceof CHTLJSSelectorNode) {
                js.append("'").append(getSelectorString((CHTLJSSelectorNode) targetNode)).append("'");
            }
            
            js.append(",\n");
        }
        
        // 处理事件处理器
        for (Map.Entry<String, CHTLASTNode> entry : properties.entrySet()) {
            String key = entry.getKey();
            if (key.equals("target")) continue;
            
            CHTLASTNode handler = entry.getValue();
            
            js.append("    ").append(key).append(": ");
            
            if (handler instanceof CHTLJSFunctionNode) {
                processFunctionHandler((CHTLJSFunctionNode) handler, js, "    ");

            } else if (handler instanceof CHTLJSIdentifierNode) {
                js.append(((CHTLJSIdentifierNode) handler).getName());
            }
            
            js.append(",\n");
            
            // 注册到事件委托系统
            if (targetNode instanceof CHTLJSSelectorNode) {
                String targetSelector = getSelectorString((CHTLJSSelectorNode) targetNode);
                String handlerName = generateHandlerName(key, parentVar);
                jsSystem.registerDelegation(parentVar, key, targetSelector, handlerName);
            }
        }
    }
    
    /**
     * 处理目标数组
     */
    private void processTargetArray(CHTLJSArrayNode array, StringBuilder js) {
        js.append("[");
        
        List<CHTLASTNode> elements = array.getElements();
        for (int i = 0; i < elements.size(); i++) {
            if (i > 0) js.append(", ");
            
            CHTLASTNode element = elements.get(i);
            if (element instanceof CHTLJSSelectorNode) {
                js.append("'").append(getSelectorString((CHTLJSSelectorNode) element)).append("'");
            }
        }
        
        js.append("]");
    }
    
    /**
     * 处理普通函数处理器
     */
    private void processFunctionHandler(CHTLJSFunctionNode func, 
                                       StringBuilder js, 
                                       String indent) {
        js.append("function(");
        
        // 参数
        List<String> params = func.getParameters();
        for (int i = 0; i < params.size(); i++) {
            if (i > 0) js.append(", ");
            js.append(params.get(i));
        }
        
        js.append(") {\n");
        
        // 函数体
        js.append(indent).append("  ").append(func.getBody()).append("\n");
        js.append(indent).append("}");
    }
    

    
    /**
     * 获取选择器字符串
     */
    private String getSelectorString(CHTLJSSelectorNode selector) {
        // 简化处理，实际需要根据选择器类型生成
        return selector.getSelector();
    }
    
    /**
     * 生成处理器名称
     */
    private String generateHandlerName(String event, String target) {
        return "_" + target + "_" + event + "_handler";
    }
}