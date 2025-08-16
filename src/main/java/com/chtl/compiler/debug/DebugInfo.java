package com.chtl.compiler.debug;

import com.chtl.context.ErrorType;
import com.chtl.compiler.chtl.Token;
import com.chtl.scanner.State;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.node.CustomDefinitionNode;
import com.chtl.ast.node.TemplateDefinitionNode;
import com.chtl.context.CompilationContext;
import com.chtl.context.CompilationError;
import com.chtl.context.CompilationWarning;
import com.chtl.model.ElementNode;
import com.chtl.context.*;
import com.chtl.scanner.ScannerStateMachine;
import java.io.PrintStream;

/**
 * 调试信息输出器
 * 提供详细的编译过程调试信息
 * 严格遵循CHTL语法文档
 */
public class DebugInfo {
    private final CompilationContext context;
    private final ScannerStateMachine stateMachine;
    private final PrintStream out;
    
    // 调试级别
    public enum DebugLevel {
        NONE,       // 不输出调试信息
        BASIC,      // 基本信息
        DETAILED,   // 详细信息
        VERBOSE     // 冗长信息
    }
    
    private DebugLevel level = DebugLevel.BASIC;
    private boolean showTokens = false;
    private boolean showAST = false;
    private boolean showStates = false;
    private boolean showContext = false;
    
    public DebugInfo(CompilationContext context, PrintStream out) {
        this.context = context;
        this.stateMachine = new ScannerStateMachine(context);
        this.out = out != null ? out : System.out;
    }
    
    /**
     * 输出编译开始信息
     */
    public void compilationStart(String sourcePath) {
        if (level == DebugLevel.NONE) return;
        
        out.println(ColorUtil.cyan("=== 开始编译 CHTL 文件 ==="));
        out.println("源文件: " + sourcePath);
        out.println("时间: " + new Date());
        
        if (level.ordinal() >= DebugLevel.DETAILED.ordinal()) {
            out.println("编译选项:");
            out.println("  调试级别: " + level);
            out.println("  显示Token: " + showTokens);
            out.println("  显示AST: " + showAST);
            out.println("  显示状态: " + showStates);
        }
        
        out.println();
    }
    
    /**
     * 输出编译结束信息
     */
    public void compilationEnd(boolean success, long elapsedTime) {
        if (level == DebugLevel.NONE) return;
        
        out.println();
        if (success) {
            out.println(ColorUtil.green("=== 编译成功 ==="));
        } else {
            out.println(ColorUtil.red("=== 编译失败 ==="));
        }
        
        out.println("耗时: " + elapsedTime + " ms");
        
        // 输出统计信息
        if (level.ordinal() >= DebugLevel.BASIC.ordinal()) {
            outputStatistics();
        }
    }
    
    /**
     * 输出词法分析信息
     */
    public void lexingStart(String phase) {
        if (!showTokens || level == DebugLevel.NONE) return;
        
        out.println(ColorUtil.blue("--- 词法分析: " + phase + " ---"));
    }
    
    /**
     * 输出Token
     */
    public void outputToken(Object token) {
        if (!showTokens || level == DebugLevel.NONE) return;
        
        out.println("  Token: " + formatToken(token));
    }
    
    /**
     * 输出语法分析信息
     */
    public void parsingStart(String phase) {
        if (level == DebugLevel.NONE) return;
        
        out.println(ColorUtil.blue("--- 语法分析: " + phase + " ---"));
    }
    
    /**
     * 输出AST节点
     */
    public void outputASTNode(CHTLASTNode node, int depth) {
        if (!showAST || level == DebugLevel.NONE) return;
        
        String indent = "  ".repeat(depth);
        out.println(indent + formatASTNode(node));
        
        // 递归输出子节点
        if (node instanceof AbstractCHTLASTNode) {
            AbstractCHTLASTNode abstractNode = (AbstractCHTLASTNode) node;
            for (CHTLASTNode child : abstractNode.getChildren()) {
                outputASTNode(child, depth + 1);
            }
        }
    }
    
    /**
     * 输出状态变化
     */
    public void stateChange(CompilationContext.State oldState, CompilationContext.State newState) {
        if (!showStates || level == DebugLevel.NONE) return;
        
        out.println(ColorUtil.purple("状态变化: " + oldState + " -> " + newState));
    }
    
    /**
     * 输出上下文信息
     */
    public void contextInfo(String info) {
        if (!showContext || level.ordinal() < DebugLevel.DETAILED.ordinal()) return;
        
        out.println(ColorUtil.gray("上下文: " + info));
    }
    
    /**
     * 输出优化信息
     */
    public void optimizationInfo(String optimization, String before, String after) {
        if (level.ordinal() < DebugLevel.DETAILED.ordinal()) return;
        
        out.println(ColorUtil.yellow("优化: " + optimization));
        if (level == DebugLevel.VERBOSE) {
            out.println("  优化前: " + truncate(before, 100));
            out.println("  优化后: " + truncate(after, 100));
        }
    }
    
    /**
     * 输出统计信息
     */
    private void outputStatistics() {
        out.println("\n统计信息:");
        
        List<CompilationError> errors = context.getErrors();
        List<CompilationWarning> warnings = context.getWarnings();
        
        out.println("  错误数: " + errors.size());
        out.println("  警告数: " + warnings.size());
        
        if (level.ordinal() >= DebugLevel.DETAILED.ordinal()) {
            // 按类型统计错误
            Map<CompilationError.ErrorType, Integer> errorStats = new HashMap<Object, Object>();
            for (CompilationError error : errors) {
                errorStats.put(error.getType(), 
                    errorStats.getOrDefault(error.getType(), 0) + 1);
            }
            
            if (!errorStats.isEmpty()) {
                out.println("\n  错误类型分布:");
                for (Map.Entry<CompilationError.ErrorType, Integer> entry : errorStats.entrySet()) {
                    out.println("    " + entry.getKey() + ": " + entry.getValue());
                }
            }
        }
    }
    
    /**
     * 格式化Token
     */
    private String formatToken(Object token) {
        // 简化实现
        return String.valueOf(token);
    }
    
    /**
     * 格式化AST节点
     */
    private String formatASTNode(CHTLASTNode node) {
        String type = node.getClass().getSimpleName();
        String value = "";
        
        // 根据节点类型提取关键信息
        if (node instanceof com.chtl.ast.node.ElementNode) {
            value = " <" + ((com.chtl.ast.node.ElementNode) node).getTagName() + ">";
        } else if (node instanceof com.chtl.ast.node.TemplateDefinitionNode) {
            value = " " + ((com.chtl.ast.node.TemplateDefinitionNode) node).getName();
        } else if (node instanceof com.chtl.ast.node.CustomDefinitionNode) {
            value = " " + ((com.chtl.ast.node.CustomDefinitionNode) node).getName();
        }
        
        return type + value;
    }
    
    /**
     * 截断字符串
     */
    private String truncate(String str, int maxLength) {
        if (str == null) return "";
        if (str.length() <= maxLength) return str;
        return str.substring(0, maxLength) + "...";
    }
    
    // Getters and setters
    public void setLevel(DebugLevel level) {
        this.level = level;
    }
    
    public void setShowTokens(boolean showTokens) {
        this.showTokens = showTokens;
    }
    
    public void setShowAST(boolean showAST) {
        this.showAST = showAST;
    }
    
    public void setShowStates(boolean showStates) {
        this.showStates = showStates;
    }
    
    public void setShowContext(boolean showContext) {
        this.showContext = showContext;
    }
}
