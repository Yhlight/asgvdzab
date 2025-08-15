import com.chtl.compiler.cjmod.*;
import com.chtl.compiler.cjmod.example.DateTimeCJmod;
import com.chtl.context.CHTLJSContext;
import com.chtl.ast.chtljs.*;

import java.nio.file.*;
import java.util.*;

/**
 * 测试CJmod系统
 * 演示如何使用CJmod扩展CHTL JS功能
 */
public class TestCJmodSystem {
    public static void main(String[] args) throws Exception {
        System.out.println("=== CJmod系统测试 ===\n");
        
        // 1. 测试CJmod接口
        System.out.println("1. 测试CJmod接口和基础功能...");
        testCJmodInterface();
        
        // 2. 测试CJmod管理器
        System.out.println("\n2. 测试CJmod管理器...");
        testCJmodManager();
        
        // 3. 测试方法调用
        System.out.println("\n3. 测试CJmod方法调用...");
        testMethodCalls();
        
        // 4. 测试AST处理
        System.out.println("\n4. 测试AST节点处理...");
        testASTProcessing();
        
        // 5. 生成文档
        System.out.println("\n5. 生成CJmod文档...");
        generateDocumentation();
        
        System.out.println("\n测试完成！");
    }
    
    private static void testCJmodInterface() {
        // 创建示例模块
        DateTimeCJmod dateTimeModule = new DateTimeCJmod();
        
        System.out.println("模块信息:");
        System.out.println("  名称: " + dateTimeModule.getModuleName());
        System.out.println("  版本: " + dateTimeModule.getModuleVersion());
        System.out.println("  描述: " + dateTimeModule.getDescription());
        System.out.println("  作者: " + dateTimeModule.getAuthor());
        
        // 初始化模块
        CHTLJSContext context = new CHTLJSContext();
        dateTimeModule.initialize(context);
        
        System.out.println("\n提供的方法:");
        for (String method : dateTimeModule.getProvidedMethods().keySet()) {
            System.out.println("  - " + method);
        }
        
        System.out.println("\n提供的对象:");
        for (String object : dateTimeModule.getProvidedObjects().keySet()) {
            System.out.println("  - " + object);
        }
    }
    
    private static void testCJmodManager() {
        CHTLJSContext context = new CHTLJSContext();
        CJmodManager manager = new CJmodManager(context);
        
        // 手动注册示例模块（实际使用中应该从.cjmod文件加载）
        DateTimeCJmod dateTimeModule = new DateTimeCJmod();
        
        // 通过反射模拟注册
        try {
            java.lang.reflect.Method registerMethod = CJmodManager.class
                .getDeclaredMethod("registerModule", CJmodInterface.class);
            registerMethod.setAccessible(true);
            registerMethod.invoke(manager, dateTimeModule);
            
            System.out.println("\n模块注册成功！");
            
            // 获取模块信息
            List<CJmodManager.ModuleInfo> modules = manager.getAllModuleInfo();
            for (CJmodManager.ModuleInfo info : modules) {
                System.out.println("\n已注册模块: " + info.getName());
                System.out.println("  方法: " + String.join(", ", info.getMethods()));
                System.out.println("  对象: " + String.join(", ", info.getObjects()));
            }
            
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    private static void testMethodCalls() {
        CHTLJSContext context = new CHTLJSContext();
        DateTimeCJmod module = new DateTimeCJmod();
        module.initialize(context);
        
        System.out.println("测试方法调用:");
        
        // 测试now方法
        String nowCode = module.executeMethod("now", Arrays.asList("'iso'"), context);
        System.out.println("\nDateTime.now('iso'):");
        System.out.println("  生成的JS代码: " + nowCode);
        
        // 测试format方法
        String formatCode = module.executeMethod("format", 
            Arrays.asList("'2024-01-20'", "'yyyy年MM月dd日'"), context);
        System.out.println("\nDateTime.format('2024-01-20', 'yyyy年MM月dd日'):");
        System.out.println("  生成的JS代码: " + formatCode);
        
        // 测试add方法
        String addCode = module.executeMethod("add", 
            Arrays.asList("'2024-01-20'", "7", "'days'"), context);
        System.out.println("\nDateTime.add('2024-01-20', 7, 'days'):");
        System.out.println("  生成的JS代码: " + addCode);
        
        // 测试diff方法
        String diffCode = module.executeMethod("diff", 
            Arrays.asList("'2024-01-20'", "'2024-01-27'", "'days'"), context);
        System.out.println("\nDateTime.diff('2024-01-20', '2024-01-27', 'days'):");
        System.out.println("  生成的JS代码: " + diffCode);
    }
    
    private static void testASTProcessing() {
        CHTLJSContext context = new CHTLJSContext();
        DateTimeCJmod module = new DateTimeCJmod();
        module.initialize(context);
        
        System.out.println("测试AST处理:");
        
        // 创建测试AST节点
        ExpressionNode expr1 = new ExpressionNode("new DateRange('2024-01-01', '2024-12-31')");
        String result1 = module.processASTNode(expr1, context);
        System.out.println("\n输入: new DateRange('2024-01-01', '2024-12-31')");
        System.out.println("输出: " + (result1 != null ? result1 : "未处理"));
        
        ExpressionNode expr2 = new ExpressionNode("console.log('test')");
        String result2 = module.processASTNode(expr2, context);
        System.out.println("\n输入: console.log('test')");
        System.out.println("输出: " + (result2 != null ? result2 : "未处理"));
    }
    
    private static void generateDocumentation() {
        CHTLJSContext context = new CHTLJSContext();
        CJmodManager manager = new CJmodManager(context);
        
        // 注册模块
        try {
            DateTimeCJmod dateTimeModule = new DateTimeCJmod();
            java.lang.reflect.Method registerMethod = CJmodManager.class
                .getDeclaredMethod("registerModule", CJmodInterface.class);
            registerMethod.setAccessible(true);
            registerMethod.invoke(manager, dateTimeModule);
            
            // 生成文档
            String documentation = manager.generateDocumentation();
            
            // 保存文档
            Files.writeString(Paths.get("CJmod-Documentation.md"), documentation);
            System.out.println("文档已生成: CJmod-Documentation.md");
            
            // 显示部分文档
            String[] lines = documentation.split("\n");
            System.out.println("\n文档预览:");
            for (int i = 0; i < Math.min(20, lines.length); i++) {
                System.out.println(lines[i]);
            }
            if (lines.length > 20) {
                System.out.println("...");
            }
            
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}