import com.chtl.context.*;

/**
 * RAII状态管理模式演示
 * 展示如何使用CompilationContext和CHTLJSContext
 */
public class TestRAIIDemo {
    public static void main(String[] args) {
        System.out.println("=== RAII自动化管理模式演示 ===\n");
        
        // 演示CHTL编译上下文
        System.out.println("1. CHTL编译上下文演示:");
        demonstrateCHTLContext();
        
        // 演示CHTL JS上下文
        System.out.println("\n2. CHTL JS上下文演示:");
        demonstrateCHTLJSContext();
        
        // 演示约束管理器
        System.out.println("\n3. 约束管理器演示:");
        demonstrateConstraintManager();
    }
    
    private static void demonstrateCHTLContext() {
        CompilationContext context = new CompilationContext();
        
        System.out.println("初始状态: " + context.getCurrentState());
        System.out.println("初始作用域: " + context.getCurrentScope().getType());
        
        // 演示嵌套状态管理
        System.out.println("\n嵌套状态管理:");
        try (var state1 = context.enterState(CompilationContext.State.NAMESPACE)) {
            System.out.println("  进入NAMESPACE: " + context.getCurrentState());
            
            try (var state2 = context.enterState(CompilationContext.State.TEMPLATE_DEFINITION)) {
                System.out.println("    进入TEMPLATE_DEFINITION: " + context.getCurrentState());
                
                // 在模板定义中进入样式块
                try (var state3 = context.enterState(CompilationContext.State.STYLE_BLOCK)) {
                    System.out.println("      进入STYLE_BLOCK: " + context.getCurrentState());
                    
                    // 验证状态
                    System.out.println("      isInStyle(): " + context.isInStyle());
                    System.out.println("      isInTemplate(): " + context.isInTemplate());
                } // 自动退出STYLE_BLOCK
                
                System.out.println("    返回TEMPLATE_DEFINITION: " + context.getCurrentState());
            } // 自动退出TEMPLATE_DEFINITION
            
            System.out.println("  返回NAMESPACE: " + context.getCurrentState());
        } // 自动退出NAMESPACE
        
        System.out.println("返回ROOT: " + context.getCurrentState());
        
        // 演示作用域和符号管理
        System.out.println("\n作用域和符号管理:");
        try (var ns = context.enterNamespace("com.example")) {
            System.out.println("  进入命名空间: com.example");
            
            try (var scope = context.enterScope(CompilationContext.ScopeType.TEMPLATE, "MyTemplate")) {
                System.out.println("  进入模板作用域: MyTemplate");
                
                // 注册模板
                var template = new com.chtl.ast.node.TemplateDefinitionNode(
                    com.chtl.ast.node.TemplateDefinitionNode.TemplateType.ELEMENT, 
                    "Card"
                );
                context.registerTemplate(template);
                System.out.println("  注册模板: Card");
                
                // 查找模板
                var found = context.findTemplate(
                    com.chtl.ast.node.TemplateDefinitionNode.TemplateType.ELEMENT, 
                    "Card"
                );
                System.out.println("  查找模板Card: " + (found != null ? "成功" : "失败"));
                
                // 获取完全限定名
                System.out.println("  完全限定名: " + context.getFullyQualifiedName("Card"));
            }
        }
        
        // 演示错误处理
        System.out.println("\n错误和警告管理:");
        context.addError(new CompilationError(
            "未定义的模板引用: UnknownTemplate",
            CompilationError.ErrorType.UNDEFINED_REFERENCE
        ));
        context.addWarning(new CompilationWarning(
            "未使用的模板定义: UnusedTemplate",
            CompilationWarning.WarningType.UNUSED_DEFINITION
        ));
        
        System.out.println("  错误数: " + context.getErrors().size());
        System.out.println("  警告数: " + context.getWarnings().size());
        
        for (CompilationError error : context.getErrors()) {
            System.out.println("  错误: " + error);
        }
        for (CompilationWarning warning : context.getWarnings()) {
            System.out.println("  警告: " + warning);
        }
    }
    
    private static void demonstrateCHTLJSContext() {
        CHTLJSContext context = new CHTLJSContext();
        
        System.out.println("初始状态: " + context.getCurrentState());
        
        // 演示选择器缓存
        System.out.println("\n选择器缓存管理:");
        String selector1 = "#myButton";
        String var1 = context.generateSelectorVariable();
        context.cacheSelector(selector1, var1);
        System.out.println("  缓存选择器: " + selector1 + " -> " + var1);
        
        String selector2 = ".status";
        String var2 = context.generateSelectorVariable();
        context.cacheSelector(selector2, var2);
        System.out.println("  缓存选择器: " + selector2 + " -> " + var2);
        
        System.out.println("  查找#myButton: " + context.getCachedSelector(selector1));
        System.out.println("  查找.status: " + context.getCachedSelector(selector2));
        
        // 演示事件委托注册
        System.out.println("\n事件委托管理:");
        var handler = new com.chtl.ast.chtljs.EventHandlerNode("click");
        var delegationInfo = new CHTLJSContext.DelegationInfo(
            "document.body",
            java.util.Arrays.asList(".btn", ".link"),
            "click",
            handler
        );
        context.registerDelegation(delegationInfo);
        System.out.println("  注册事件委托: " + delegationInfo.getParentSelector() + 
                          " -> " + delegationInfo.getTargetSelectors());
        
        // 演示状态转换
        System.out.println("\n状态转换:");
        try (var state1 = context.enterState(CHTLJSContext.State.ENHANCED_SELECTOR)) {
            System.out.println("  进入ENHANCED_SELECTOR: " + context.getCurrentState());
            System.out.println("  isInSelector(): " + context.isInSelector());
            
            try (var state2 = context.enterState(CHTLJSContext.State.ARROW_OPERATION)) {
                System.out.println("    进入ARROW_OPERATION: " + context.getCurrentState());
                System.out.println("    canUseListen(): " + context.canUseListen());
                
                try (var state3 = context.enterState(CHTLJSContext.State.LISTEN_CALL)) {
                    System.out.println("      进入LISTEN_CALL: " + context.getCurrentState());
                    System.out.println("      isInListenCall(): " + context.isInListenCall());
                }
            }
        }
        
        System.out.println("返回ROOT: " + context.getCurrentState());
        
        // 演示临时变量生成
        System.out.println("\n临时变量生成:");
        for (int i = 0; i < 3; i++) {
            System.out.println("  生成临时变量: " + context.generateTempVariable());
        }
    }
    
    private static void demonstrateConstraintManager() {
        CompilationContext context = new CompilationContext();
        ConstraintManager manager = context.getConstraintManager();
        
        System.out.println("约束管理器功能:");
        
        // 添加全局约束
        var globalConstraint = new com.chtl.ast.node.ConstraintNode(
            com.chtl.ast.node.ConstraintNode.ConstraintTarget.STYLE
        );
        manager.addGlobalConstraint(globalConstraint);
        System.out.println("  添加全局约束: 禁止STYLE");
        
        // 添加元素级约束
        var elementConstraint = new com.chtl.ast.node.ConstraintNode(
            com.chtl.ast.node.ConstraintNode.ConstraintTarget.TEMPLATE
        );
        manager.addElementConstraint("div/span", elementConstraint);
        System.out.println("  添加元素约束: div/span 禁止TEMPLATE");
        
        // 测试约束作用域
        System.out.println("\n约束作用域管理:");
        var constraints = java.util.Arrays.asList(
            new com.chtl.ast.node.ConstraintNode(
                com.chtl.ast.node.ConstraintNode.ConstraintTarget.CUSTOM
            )
        );
        
        System.out.println("  进入约束作用域...");
        try (var constraintScope = manager.createScope(constraints)) {
            System.out.println("  约束作用域活动中");
            
            // 测试节点是否被允许
            var styleNode = new com.chtl.ast.node.StyleBlockNode(true);
            boolean allowed = manager.isAllowed(styleNode, context);
            System.out.println("  StyleBlockNode是否允许: " + allowed);
        }
        System.out.println("  退出约束作用域");
    }
}