package com.chtl.compiler.chtljs;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.node.CHTLJSAnimateNode;
import com.chtl.ast.node.CHTLJSExpressionNode;
import com.chtl.ast.node.CHTLJSMethodCallNode;
import com.chtl.ast.node.CHTLJSSelectorNode;
import com.chtl.ast.node.NumberLiteralNode;
import com.chtl.ast.node.ScriptBlockNode;
import com.chtl.ast.node.StringLiteralNode;
import com.chtl.context.CompilationError;
import com.chtl.model.TextNode;
import com.chtl.ast.*;
import com.chtl.context.CompilationContext;
import com.chtl.scanner.ScannerStateMachine;
import com.chtl.compiler.chtljs.selector.SelectorParser;
import com.chtl.compiler.chtljs.event.EventBindingProcessor;
import com.chtl.compiler.chtljs.animation.AnimationProcessor;

/**
 * CHTL JS系统
 * 负责处理CHTL JS特有语法：选择器、事件绑定、动画等
 * 严格遵循CHTL语法文档
 */
public class CHTLJSSystem {
    private final CompilationContext context;
    private final ScannerStateMachine stateMachine;
    
    // 选择器解析器
    private final SelectorParser selectorParser;
    
    // 事件绑定处理器
    private final EventBindingProcessor eventProcessor;
    
    // 动画处理器
    private final AnimationProcessor animationProcessor;
    
    // 生成的JavaScript代码
    private final StringBuilder generatedJS;
    
    // 选择器缓存（避免重复生成）
    private final Map<String, String> selectorCache;
    
    // 事件委托注册表
    private final Map<String, DelegationInfo> delegationRegistry;
    
    /**
     * 事件委托信息
     */
    private static class DelegationInfo {
        final String parentSelector;
        final Map<String, List<DelegationTarget>> eventTargets;
        
        DelegationInfo(String parentSelector) {
            this.parentSelector = parentSelector;
            this.eventTargets = new HashMap<>();
        }
    }
    
    /**
     * 委托目标
     */
    private static class DelegationTarget {
        final String targetSelector;
        final String handlerFunction;
        
        DelegationTarget(String targetSelector, String handlerFunction) {
            this.targetSelector = targetSelector;
            this.handlerFunction = handlerFunction;
        }
    }
    
    public CHTLJSSystem(CompilationContext context) {
        this.context = context;
        this.stateMachine = new ScannerStateMachine(context);
        this.selectorParser = new SelectorParser(context);
        this.eventProcessor = new EventBindingProcessor(this);
        this.animationProcessor = new AnimationProcessor(this);
        this.generatedJS = new StringBuilder();
        this.selectorCache = new HashMap<>();
        this.delegationRegistry = new HashMap<>();
    }
    
    /**
     * 处理脚本块
     */
    public void processScriptBlock(ScriptBlockNode scriptBlock) {
        try (var stateGuard = context.enterState(CompilationContext.State.ELEMENT)) {
            // 初始化运行时支持代码
            initializeRuntime();
            
            // 处理脚本内容
            processScriptContent(scriptBlock);
            
            // 生成事件委托代码
            generateDelegationCode();
        }
    }
    
    /**
     * 初始化CHTL JS运行时
     */
    private void initializeRuntime() {
        // 只初始化一次
        if (generatedJS.length() == 0) {
            generatedJS.append("// CHTL JS Runtime\n");
            generatedJS.append("(function() {\n");
            generatedJS.append("  'use strict';\n\n");
            
            // 添加CHTL JS辅助函数
            addHelperFunctions();
        }
    }
    
    /**
     * 添加辅助函数
     */
    private void addHelperFunctions() {
        // 选择器缓存
        generatedJS.append("  // 选择器缓存\n");
        generatedJS.append("  const _chtl_cache = new Map();\n\n");
        
        // 增强选择器函数
        generatedJS.append("  // 增强选择器\n");
        generatedJS.append("  function _chtl_select(selector, index) {\n");
        generatedJS.append("    const key = selector + (index !== undefined ? '[' + index + ']' : '');\n");
        generatedJS.append("    if (_chtl_cache.has(key)) return _chtl_cache.get(key);\n");
        generatedJS.append("    \n");
        generatedJS.append("    let result;\n");
        generatedJS.append("    if (index !== undefined) {\n");
        generatedJS.append("      const elements = document.querySelectorAll(selector);\n");
        generatedJS.append("      result = elements[index] || null;\n");
        generatedJS.append("    } else {\n");
        generatedJS.append("      result = document.querySelector(selector);\n");
        generatedJS.append("    }\n");
        generatedJS.append("    \n");
        generatedJS.append("    _chtl_cache.set(key, result);\n");
        generatedJS.append("    return result;\n");
        generatedJS.append("  }\n\n");
        
        // listen方法扩展
        generatedJS.append("  // 增强事件监听\n");
        generatedJS.append("  Element.prototype.listen = function(events) {\n");
        generatedJS.append("    for (const [event, handler] of Object.entries(events)) {\n");
        generatedJS.append("      this.addEventListener(event, handler);\n");
        generatedJS.append("    }\n");
        generatedJS.append("    return this;\n");
        generatedJS.append("  };\n\n");
        
        // delegate方法扩展
        generatedJS.append("  // 事件委托\n");
        generatedJS.append("  Element.prototype.delegate = function(config) {\n");
        generatedJS.append("    const targets = Array.isArray(config.target) ? config.target : [config.target];\n");
        generatedJS.append("    \n");
        generatedJS.append("    for (const [event, handler] of Object.entries(config)) {\n");
        generatedJS.append("      if (event === 'target') continue;\n");
        generatedJS.append("      \n");
        generatedJS.append("      this.addEventListener(event, function(e) {\n");
        generatedJS.append("        for (const target of targets) {\n");
        generatedJS.append("          if (e.target.matches(target)) {\n");
        generatedJS.append("            handler.call(e.target, e);\n");
        generatedJS.append("            break;\n");
        generatedJS.append("          }\n");
        generatedJS.append("        }\n");
        generatedJS.append("      });\n");
        generatedJS.append("    }\n");
        generatedJS.append("    return this;\n");
        generatedJS.append("  };\n\n");
        
        // animate函数
        generatedJS.append("  // 动画函数\n");
        generatedJS.append("  window.animate = function(config) {\n");
        generatedJS.append("    const duration = config.duration || 1000;\n");
        generatedJS.append("    const easing = config.easing || 'linear';\n");
        generatedJS.append("    const loop = config.loop || 1;\n");
        generatedJS.append("    const direction = config.direction || 'normal';\n");
        generatedJS.append("    const delay = config.delay || 0;\n");
        generatedJS.append("    \n");
        generatedJS.append("    let startTime = null;\n");
        generatedJS.append("    let currentLoop = 0;\n");
        generatedJS.append("    \n");
        generatedJS.append("    function frame(timestamp) {\n");
        generatedJS.append("      if (!startTime) startTime = timestamp + delay;\n");
        generatedJS.append("      if (timestamp < startTime) {\n");
        generatedJS.append("        requestAnimationFrame(frame);\n");
        generatedJS.append("        return;\n");
        generatedJS.append("      }\n");
        generatedJS.append("      \n");
        generatedJS.append("      const elapsed = timestamp - startTime;\n");
        generatedJS.append("      const progress = Math.min(elapsed / duration, 1);\n");
        generatedJS.append("      \n");
        generatedJS.append("      // 应用缓动函数\n");
        generatedJS.append("      const easedProgress = applyEasing(progress, easing);\n");
        generatedJS.append("      \n");
        generatedJS.append("      // 执行动画帧\n");
        generatedJS.append("      if (config.frame) config.frame(easedProgress);\n");
        generatedJS.append("      \n");
        generatedJS.append("      // 检查时间点回调\n");
        generatedJS.append("      for (const [time, callback] of Object.entries(config.when || {})) {\n");
        generatedJS.append("        const targetProgress = parseFloat(time) / 100;\n");
        generatedJS.append("        if (Math.abs(progress - targetProgress) < 0.01) {\n");
        generatedJS.append("          callback();\n");
        generatedJS.append("        }\n");
        generatedJS.append("      }\n");
        generatedJS.append("      \n");
        generatedJS.append("      if (progress < 1) {\n");
        generatedJS.append("        requestAnimationFrame(frame);\n");
        generatedJS.append("      } else {\n");
        generatedJS.append("        currentLoop++;\n");
        generatedJS.append("        if (currentLoop < loop || loop === -1) {\n");
        generatedJS.append("          startTime = timestamp;\n");
        generatedJS.append("          requestAnimationFrame(frame);\n");
        generatedJS.append("        } else if (config.callback) {\n");
        generatedJS.append("          config.callback();\n");
        generatedJS.append("        }\n");
        generatedJS.append("      }\n");
        generatedJS.append("    }\n");
        generatedJS.append("    \n");
        generatedJS.append("    requestAnimationFrame(frame);\n");
        generatedJS.append("  };\n\n");
        
        // 缓动函数
        generatedJS.append("  // 缓动函数\n");
        generatedJS.append("  function applyEasing(t, type) {\n");
        generatedJS.append("    switch(type) {\n");
        generatedJS.append("      case 'easeIn': return t * t;\n");
        generatedJS.append("      case 'easeOut': return t * (2 - t);\n");
        generatedJS.append("      case 'easeInOut': return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t;\n");
        generatedJS.append("      default: return t;\n");
        generatedJS.append("    }\n");
        generatedJS.append("  }\n\n");
    }
    
    /**
     * 处理脚本内容
     */
    private void processScriptContent(ScriptBlockNode scriptBlock) {
        generatedJS.append("  // 用户脚本\n");
        
        for (CHTLASTNode child : scriptBlock.getChildren()) {
            if (child instanceof CHTLJSExpressionNode) {
                processCHTLJSExpression((CHTLJSExpressionNode) child);
            } else if (child instanceof TextNode) {
                // 普通JavaScript代码
                generatedJS.append("  ").append(((TextNode) child).getContent()).append("\n");
            }
        }
    }
    
    /**
     * 处理CHTL JS表达式
     */
    private void processCHTLJSExpression(CHTLJSExpressionNode expr) {
        if (expr instanceof CHTLJSSelectorNode) {
            processSelectorExpression((CHTLJSSelectorNode) expr);
        } else if (expr instanceof CHTLJSMethodCallNode) {
            processMethodCall((CHTLJSMethodCallNode) expr);
        } else if (expr instanceof CHTLJSAnimateNode) {
            processAnimate((CHTLJSAnimateNode) expr);
        }
    }
    
    /**
     * 处理选择器表达式
     */
    private void processSelectorExpression(CHTLJSSelectorNode selector) {
        String jsSelector = selectorParser.parseToJSSelector(selector);
        String varName = generateSelectorVariable(jsSelector);
        
        generatedJS.append("  const ").append(varName).append(" = ");
        
        if (selector.hasIndex()) {
            generatedJS.append("_chtl_select('").append(jsSelector)
                .append("', ").append(selector.getIndex()).append(")");
        } else {
            generatedJS.append("_chtl_select('").append(jsSelector).append("')");
        }
        
        generatedJS.append(";\n");
        
        // 缓存选择器变量名
        selectorCache.put(selector.toString(), varName);
    }
    
    /**
     * 处理方法调用
     */
    private void processMethodCall(CHTLJSMethodCallNode methodCall) {
        String targetVar = selectorCache.get(methodCall.getTarget().toString());
        if (targetVar == null) {
            context.addError(new com.chtl.context.CompilationError(
                "未定义的选择器: " + methodCall.getTarget(),
                com.chtl.context.CompilationError.ErrorType.UNDEFINED_REFERENCE
            ));
            return;
        }
        
        String methodName = methodCall.getMethodName();
        
        if (methodName.equals("listen")) {
            eventProcessor.processListenMethod(methodCall, targetVar, generatedJS);
        } else if (methodName.equals("delegate")) {
            eventProcessor.processDelegateMethod(methodCall, targetVar, generatedJS);
        } else {
            // 普通方法调用
            generatedJS.append("  ").append(targetVar).append(".")
                .append(methodName).append("(");
            
            // 添加参数
            List<CHTLASTNode> args = methodCall.getArguments();
            for (int i = 0; i < args.size(); i++) {
                if (i > 0) generatedJS.append(", ");
                generatedJS.append(generateExpression(args.get(i)));
            }
            
            generatedJS.append(");\n");
        }
    }
    
    /**
     * 处理动画
     */
    private void processAnimate(CHTLJSAnimateNode animate) {
        animationProcessor.processAnimation(animate, generatedJS);
    }
    
    /**
     * 生成选择器变量名
     */
    private String generateSelectorVariable(String selector) {
        // 将选择器转换为有效的变量名
        String varName = selector.replaceAll("[^a-zA-Z0-9]", "_");
        if (Character.isDigit(varName.charAt(0))) {
            varName = "_" + varName;
        }
        return varName;
    }
    
    /**
     * 生成表达式代码
     */
    private String generateExpression(CHTLASTNode node) {
        if (node instanceof StringLiteralNode) {
            return "'" + ((StringLiteralNode) node).getValue() + "'";
        } else if (node instanceof NumberLiteralNode) {
            return ((NumberLiteralNode) node).getValue();
        } else if (node instanceof CHTLJSSelectorNode) {
            String varName = selectorCache.get(node.toString());
            return varName != null ? varName : "null";
        }
        
        return node.toString();
    }
    
    /**
     * 生成事件委托代码
     */
    private void generateDelegationCode() {
        if (!delegationRegistry.isEmpty()) {
            generatedJS.append("\n  // 事件委托注册\n");
            
            for (DelegationInfo info : delegationRegistry.values()) {
                String parentVar = selectorCache.get(info.parentSelector);
                if (parentVar == null) continue;
                
                generatedJS.append("  ").append(parentVar).append(".delegate({\n");
                
                // 添加targets
                Set<String> allTargets = new HashSet<>();
                for (List<DelegationTarget> targets : info.eventTargets.values()) {
                    for (DelegationTarget target : targets) {
                        allTargets.add(target.targetSelector);
                    }
                }
                
                generatedJS.append("    target: [");
                boolean first = true;
                for (String target : allTargets) {
                    if (!first) generatedJS.append(", ");
                    generatedJS.append("'").append(target).append("'");
                    first = false;
                }
                generatedJS.append("],\n");
                
                // 添加事件处理器
                for (Map.Entry<String, List<DelegationTarget>> entry : info.eventTargets.entrySet()) {
                    String event = entry.getKey();
                    generatedJS.append("    ").append(event).append(": function(e) {\n");
                    
                    for (DelegationTarget target : entry.getValue()) {
                        generatedJS.append("      if (e.target.matches('")
                            .append(target.targetSelector).append("')) {\n");
                        generatedJS.append("        ").append(target.handlerFunction)
                            .append(".call(this, e);\n");
                        generatedJS.append("      }\n");
                    }
                    
                    generatedJS.append("    },\n");
                }
                
                generatedJS.append("  });\n");
            }
        }
    }
    
    /**
     * 完成代码生成
     */
    public String finalizeJS() {
        if (generatedJS.length() > 0) {
            generatedJS.append("\n})();\n");
        }
        return generatedJS.toString();
    }
    
    /**
     * 获取生成的JavaScript代码
     */
    public String getGeneratedJS() {
        return finalizeJS();
    }
    
    /**
     * 注册事件委托
     */
    public void registerDelegation(String parentSelector, String event, 
                                   String targetSelector, String handler) {
        DelegationInfo info = delegationRegistry.computeIfAbsent(
            parentSelector, k -> new DelegationInfo(parentSelector)
        );
        
        info.eventTargets.computeIfAbsent(event, k -> new ArrayList<>())
            .add(new DelegationTarget(targetSelector, handler));
    }
}
