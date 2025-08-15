package com.chtl.compiler.chtljs.animation;

import com.chtl.ast.*;
import com.chtl.ast.node.*;
import com.chtl.compiler.chtljs.CHTLJSSystem;

import java.util.*;

/**
 * 动画处理器
 * 负责处理animate函数
 * 严格遵循CHTL语法文档
 */
public class AnimationProcessor {
    private final CHTLJSSystem jsSystem;
    
    public AnimationProcessor(CHTLJSSystem jsSystem) {
        this.jsSystem = jsSystem;
    }
    
    /**
     * 处理动画配置
     * animate({
     *     duration: 1000,
     *     easing: 'linear',
     *     begin: { opacity: 0 },
     *     end: { opacity: 1 },
     *     when: {
     *         50: () => { ... }
     *     },
     *     loop: 1,
     *     direction: 'normal',
     *     delay: 0,
     *     callback: () => { ... }
     * });
     */
    public void processAnimation(CHTLJSAnimateNode animate, StringBuilder js) {
        js.append("  animate({\n");
        
        Map<String, CHTLASTNode> config = animate.getConfig();
        
        // 处理基本配置
        processBasicConfig(config, js);
        
        // 处理begin/end状态
        processBeginEndStates(config, js);
        
        // 处理when时间点
        processWhenPoints(config, js);
        
        // 处理回调
        processCallback(config, js);
        
        js.append("  });\n");
    }
    
    /**
     * 处理基本配置
     */
    private void processBasicConfig(Map<String, CHTLASTNode> config, StringBuilder js) {
        // duration
        CHTLASTNode duration = config.get("duration");
        if (duration != null) {
            js.append("    duration: ").append(getNodeValue(duration)).append(",\n");
        }
        
        // easing
        CHTLASTNode easing = config.get("easing");
        if (easing != null) {
            js.append("    easing: ").append(getNodeValue(easing)).append(",\n");
        }
        
        // loop
        CHTLASTNode loop = config.get("loop");
        if (loop != null) {
            js.append("    loop: ").append(getNodeValue(loop)).append(",\n");
        }
        
        // direction
        CHTLASTNode direction = config.get("direction");
        if (direction != null) {
            js.append("    direction: ").append(getNodeValue(direction)).append(",\n");
        }
        
        // delay
        CHTLASTNode delay = config.get("delay");
        if (delay != null) {
            js.append("    delay: ").append(getNodeValue(delay)).append(",\n");
        }
    }
    
    /**
     * 处理begin/end状态
     */
    private void processBeginEndStates(Map<String, CHTLASTNode> config, StringBuilder js) {
        // begin状态
        CHTLASTNode begin = config.get("begin");
        if (begin instanceof CHTLJSObjectNode) {
            js.append("    begin: ");
            processStateObject((CHTLJSObjectNode) begin, js);
            js.append(",\n");
        }
        
        // end状态
        CHTLASTNode end = config.get("end");
        if (end instanceof CHTLJSObjectNode) {
            js.append("    end: ");
            processStateObject((CHTLJSObjectNode) end, js);
            js.append(",\n");
        }
        
        // at状态（特定时刻的状态）
        CHTLASTNode at = config.get("at");
        if (at instanceof CHTLJSObjectNode) {
            js.append("    at: ");
            processAtStates((CHTLJSObjectNode) at, js);
            js.append(",\n");
        }
    }
    
    /**
     * 处理状态对象
     */
    private void processStateObject(CHTLJSObjectNode state, StringBuilder js) {
        js.append("{\n");
        
        Map<String, CHTLASTNode> properties = state.getProperties();
        int count = 0;
        
        for (Map.Entry<String, CHTLASTNode> entry : properties.entrySet()) {
            if (count > 0) js.append(",\n");
            
            String property = entry.getKey();
            CHTLASTNode value = entry.getValue();
            
            js.append("      ").append(property).append(": ");
            
            if (value instanceof CHTLJSSelectorNode) {
                // 动画目标
                js.append("document.querySelector('")
                  .append(getSelectorString((CHTLJSSelectorNode) value))
                  .append("')");
            } else {
                js.append(getNodeValue(value));
            }
            
            count++;
        }
        
        if (count > 0) js.append("\n");
        js.append("    }");
    }
    
    /**
     * 处理at状态（特定时刻的状态）
     */
    private void processAtStates(CHTLJSObjectNode atStates, StringBuilder js) {
        js.append("{\n");
        
        Map<String, CHTLASTNode> states = atStates.getProperties();
        int count = 0;
        
        for (Map.Entry<String, CHTLASTNode> entry : states.entrySet()) {
            if (count > 0) js.append(",\n");
            
            String time = entry.getKey(); // 时间点（如"50"表示50%）
            CHTLASTNode state = entry.getValue();
            
            js.append("      '").append(time).append("': ");
            
            if (state instanceof CHTLJSObjectNode) {
                processStateObject((CHTLJSObjectNode) state, js);
            }
            
            count++;
        }
        
        if (count > 0) js.append("\n");
        js.append("    }");
    }
    
    /**
     * 处理when时间点
     */
    private void processWhenPoints(Map<String, CHTLASTNode> config, StringBuilder js) {
        CHTLASTNode when = config.get("when");
        if (when instanceof CHTLJSObjectNode) {
            js.append("    when: {\n");
            
            Map<String, CHTLASTNode> points = ((CHTLJSObjectNode) when).getProperties();
            int count = 0;
            
            for (Map.Entry<String, CHTLASTNode> entry : points.entrySet()) {
                if (count > 0) js.append(",\n");
                
                String time = entry.getKey(); // 百分比时间点
                CHTLASTNode callback = entry.getValue();
                
                js.append("      '").append(time).append("': ");
                
                if (callback instanceof CHTLJSFunctionNode) {
                    processFunctionNode((CHTLJSFunctionNode) callback, js, "      ");
                } else if (callback instanceof CHTLJSArrowFunctionNode) {
                    processArrowFunction((CHTLJSArrowFunctionNode) callback, js, "      ");
                }
                
                count++;
            }
            
            if (count > 0) js.append("\n");
            js.append("    },\n");
        }
    }
    
    /**
     * 处理回调函数
     */
    private void processCallback(Map<String, CHTLASTNode> config, StringBuilder js) {
        CHTLASTNode callback = config.get("callback");
        if (callback != null) {
            js.append("    callback: ");
            
            if (callback instanceof CHTLJSFunctionNode) {
                processFunctionNode((CHTLJSFunctionNode) callback, js, "    ");
            } else if (callback instanceof CHTLJSArrowFunctionNode) {
                processArrowFunction((CHTLJSArrowFunctionNode) callback, js, "    ");
            }
            
            js.append("\n");
        }
    }
    
    /**
     * 处理函数节点
     */
    private void processFunctionNode(CHTLJSFunctionNode func, StringBuilder js, String indent) {
        js.append("function(");
        
        List<String> params = func.getParameters();
        for (int i = 0; i < params.size(); i++) {
            if (i > 0) js.append(", ");
            js.append(params.get(i));
        }
        
        js.append(") {\n");
        js.append(indent).append("  ").append(func.getBody()).append("\n");
        js.append(indent).append("}");
    }
    
    /**
     * 处理箭头函数
     */
    private void processArrowFunction(CHTLJSArrowFunctionNode func, StringBuilder js, String indent) {
        js.append("(");
        
        List<String> params = func.getParameters();
        for (int i = 0; i < params.size(); i++) {
            if (i > 0) js.append(", ");
            js.append(params.get(i));
        }
        
        js.append(") => ");
        
        if (func.isExpression()) {
            js.append(func.getBody());
        } else {
            js.append("{\n");
            js.append(indent).append("  ").append(func.getBody()).append("\n");
            js.append(indent).append("}");
        }
    }
    
    /**
     * 获取节点值
     */
    private String getNodeValue(CHTLASTNode node) {
        if (node instanceof StringLiteralNode) {
            return "'" + ((StringLiteralNode) node).getValue() + "'";
        } else if (node instanceof NumberLiteralNode) {
            return ((NumberLiteralNode) node).getValue();
        } else if (node instanceof UnquotedLiteralNode) {
            String value = ((UnquotedLiteralNode) node).getValue();
            // 检查是否是已知的字符串值
            if (isKnownStringValue(value)) {
                return "'" + value + "'";
            }
            return value;
        }
        
        return node.toString();
    }
    
    /**
     * 检查是否是已知的字符串值
     */
    private boolean isKnownStringValue(String value) {
        return value.equals("linear") || value.equals("easeIn") || 
               value.equals("easeOut") || value.equals("easeInOut") ||
               value.equals("normal") || value.equals("reverse") ||
               value.equals("alternate");
    }
    
    /**
     * 获取选择器字符串
     */
    private String getSelectorString(CHTLJSSelectorNode selector) {
        return selector.getSelector();
    }
}