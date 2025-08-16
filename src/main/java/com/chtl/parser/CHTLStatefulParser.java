package com.chtl.parser;

import com.chtl.model.TemplateType;
import com.chtl.scanner.State;


import org.antlr.v4.runtime.Parser;

import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Collectors;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.node.CustomDefinitionNode;
import com.chtl.ast.node.CustomOperationNode;
import com.chtl.ast.node.NumberLiteralNode;
import com.chtl.ast.node.RootNode;
import com.chtl.ast.node.StringLiteralNode;
import com.chtl.ast.node.StyleBlockNode;
import com.chtl.ast.node.TemplateDefinitionNode;
import com.chtl.ast.node.UnquotedLiteralNode;
import com.chtl.ast.node.VarGroupDefinitionNode;
import com.chtl.model.ElementNode;
import com.chtl.ast.*;
import com.chtl.context.CompilationContext;
import com.chtl.scanner.CHTLUnifiedScanner;
import com.chtl.scanner.ScannerStateMachine;
import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;

/**
 * 基于状态机的CHTL解析器
 * 利用CHTLUnifiedScanner和CompilationContext提供精确的解析
 * 严格遵循CHTL语法文档
 */
public class CHTLStatefulParser {
    private final CompilationContext context;
    private final CHTLUnifiedScanner scanner;
    private List<CodeFragment> fragments;
    private int currentFragment;
    
    // 解析状态
    private enum ParseState {
        INITIAL,
        IN_ELEMENT,
        IN_TEMPLATE,
        IN_CUSTOM,
        IN_VAR_GROUP,
        IN_STYLE_BLOCK,
        IN_SCRIPT_BLOCK,
        IN_INDEX_ACCESS,
        IN_INSERT_OPERATION,
        IN_DELETE_OPERATION
    }
    
    private ParseState currentState;
    private final Stack<ParseState> stateStack;
    
    public CHTLStatefulParser() {
        this.context = new CompilationContext();
        this.scanner = new CHTLUnifiedScanner();
        this.stateStack = new Stack<>();
        this.currentState = ParseState.INITIAL;
    }
    
    /**
     * 解析CHTL源代码
     */
    public RootNode parse(String source) {
        // 使用扫描器切分代码
        fragments = scanner.scan(source);
        currentFragment = 0;
        
        RootNode root = new RootNode();
        
        // 基于片段类型进行解析
        while (hasMoreFragments()) {
            CodeFragment fragment = getCurrentFragment();
            CHTLASTNode node = parseFragment(fragment);
            if (node != null) {
                root.addChild(node);
            }
            advance();
        }
        
        return root;
    }
    
    /**
     * 解析代码片段
     */
    private CHTLASTNode parseFragment(CodeFragment fragment) {
        String content = fragment.getContent().trim();
        
        switch (fragment.getType()) {
            case CHTL:
                return parseCHTLFragment(content);
            case CHTL_JS:
                return parseCHTLJSFragment(content);
            case CSS:
                return parseCSSFragment(content);
            case JAVASCRIPT:
                return parseJavaScriptFragment(content);
            default:
                return null;
        }
    }
    
    /**
     * 解析CHTL片段
     */
    private CHTLASTNode parseCHTLFragment(String content) {
        // 使用状态机和上下文进行精确解析
        try (var stateGuard = context.enterState(CompilationContext.State.ELEMENT)) {
            
            // 模板定义
            if (content.startsWith("[Template]")) {
                return parseTemplateWithState(content);
            }
            
            // 自定义定义
            if (content.startsWith("[Custom]")) {
                return parseCustomWithState(content);
            }
            
            // 元素（可能带索引）
            if (isElement(content)) {
                return parseElementWithState(content);
            }
            
            // 其他CHTL结构
            return parseCHTLStructure(content);
        }
    }
    
    /**
     * 使用状态机解析模板定义
     */
    private CHTLASTNode parseTemplateWithState(String content) {
        pushState(ParseState.IN_TEMPLATE);
        
        try {
            // 提取模板类型和名称
            String[] parts = content.split("\\s+", 4);
            if (parts.length < 3) return null;
            
            String typeStr = parts[1]; // @Element, @Style, @Var
            String name = parts[2];
            
            // 根据类型创建不同的节点
            if (typeStr.equals("@Var")) {
                return parseVarGroupDefinition(name, true, content);
            } else {
                TemplateDefinitionNode.TemplateType type = parseTemplateType(typeStr);
                TemplateDefinitionNode template = new TemplateDefinitionNode(type, name);
                parseTemplateContent(template, content);
                return template;
            }
        } finally {
            popState();
        }
    }
    
    /**
     * 使用状态机解析自定义定义
     */
    private CHTLASTNode parseCustomWithState(String content) {
        pushState(ParseState.IN_CUSTOM);
        
        try {
            // 提取自定义类型和名称
            String[] parts = content.split("\\s+", 4);
            if (parts.length < 3) return null;
            
            String typeStr = parts[1]; // @Element, @Style, @Var
            String name = parts[2];
            
            // 根据类型创建不同的节点
            if (typeStr.equals("@Var")) {
                return parseVarGroupDefinition(name, false, content);
            } else {
                TemplateDefinitionNode.TemplateType type = parseTemplateType(typeStr);
                CustomDefinitionNode custom = new CustomDefinitionNode(type, name);
                parseCustomContent(custom, content);
                return custom;
            }
        } finally {
            popState();
        }
    }
    
    /**
     * 解析变量组定义
     */
    private VarGroupDefinitionNode parseVarGroupDefinition(String name, boolean isTemplate, String content) {
        pushState(ParseState.IN_VAR_GROUP);
        
        try {
            VarGroupDefinitionNode varGroup = new VarGroupDefinitionNode(name, isTemplate);
            
            // 提取变量定义内容
            int braceStart = content.indexOf('{');
            int braceEnd = content.lastIndexOf('}');
            if (braceStart != -1 && braceEnd != -1) {
                String varContent = content.substring(braceStart + 1, braceEnd).trim();
                parseVarGroupContent(varGroup, varContent);
            }
            
            return varGroup;
        } finally {
            popState();
        }
    }
    
    /**
     * 解析变量组内容
     */
    private void parseVarGroupContent(VarGroupDefinitionNode varGroup, String content) {
        // 分割变量定义
        String[] lines = content.split(";");
        
        for (String line : lines) {
            line = line.trim();
            if (line.isEmpty()) continue;
            
            int colonIndex = line.indexOf(':');
            if (colonIndex != -1) {
                String varName = line.substring(0, colonIndex).trim();
                String varValue = line.substring(colonIndex + 1).trim();
                
                // 创建值节点
                CHTLASTNode valueNode = parseVarValue(varValue);
                varGroup.addVariable(varName, valueNode);
            }
        }
    }
    
    /**
     * 解析变量值
     */
    private CHTLASTNode parseVarValue(String value) {
        value = value.trim();
        
        // 字符串字面量
        if (value.startsWith("\"") && value.endsWith("\"")) {
            return new StringLiteralNode(value.substring(1, value.length() - 1), '"');
        }
        if (value.startsWith("'") && value.endsWith("'")) {
            return new StringLiteralNode(value.substring(1, value.length() - 1), '\'');
        }
        
        // 数字（可能带单位）
        if (Character.isDigit(value.charAt(0))) {
            return new NumberLiteralNode(value);
        }
        
        // 其他作为无引号字面量
        return new UnquotedLiteralNode(value);
    }
    
    /**
     * 使用状态机解析元素（支持索引访问）
     */
    private ElementNode parseElementWithState(String content) {
        pushState(ParseState.IN_ELEMENT);
        
        try {
            // 使用正则表达式提取元素名和索引
            String elementName = null;
            Integer index = null;
            
            // 检查是否有索引访问 element[index]
            int bracketStart = content.indexOf('[');
            if (bracketStart != -1) {
                elementName = content.substring(0, bracketStart).trim();
                int bracketEnd = content.indexOf(']', bracketStart);
                if (bracketEnd != -1) {
                    String indexStr = content.substring(bracketStart + 1, bracketEnd).trim();
                    try {
                        index = Integer.parseInt(indexStr);
                        pushState(ParseState.IN_INDEX_ACCESS);
                    } catch (NumberFormatException e) {
                        // 不是有效的索引
                    } finally {
                        popState();
                    }
                }
            } else {
                // 提取元素名（第一个单词）
                String[] parts = content.split("\\s+", 2);
                elementName = parts[0];
            }
            
            ElementNode element = new ElementNode(elementName);
            if (index != null) {
                element.setIndex(index);
            }
            
            // 解析元素内容
            parseElementContent(element, content);
            
            return element;
        } finally {
            popState();
        }
    }
    
    /**
     * 解析元素内容
     */
    private void parseElementContent(ElementNode element, String content) {
        // 查找内容块的开始和结束
        int braceStart = content.indexOf('{');
        int braceEnd = content.lastIndexOf('}');
        
        if (braceStart != -1 && braceEnd != -1) {
            String innerContent = content.substring(braceStart + 1, braceEnd).trim();
            
            // 解析子元素、属性、样式等
            // 这里简化处理，实际应该递归解析
            if (innerContent.contains("style")) {
                pushState(ParseState.IN_STYLE_BLOCK);
                StyleBlockNode styleBlock = new StyleBlockNode(true);
                element.addChild(styleBlock);
                popState();
            }
        }
    }
    
    /**
     * 解析插入操作
     */
    private CustomOperationNode parseInsertOperation(String content) {
        pushState(ParseState.IN_INSERT_OPERATION);
        
        try {
            // 解析插入位置和目标
            CustomOperationNode.OperationType opType = CustomOperationNode.OperationType.INSERT;
            CustomOperationNode.InsertPosition position = CustomOperationNode.InsertPosition.AFTER;
            
            if (content.contains("before")) {
                position = CustomOperationNode.InsertPosition.BEFORE;
            } else if (content.contains("replace")) {
                position = CustomOperationNode.InsertPosition.REPLACE;
            } else if (content.contains("at top")) {
                position = CustomOperationNode.InsertPosition.AT_TOP;
            } else if (content.contains("at bottom")) {
                position = CustomOperationNode.InsertPosition.AT_BOTTOM;
            }
            
            CustomOperationNode operation = new CustomOperationNode(opType);
            operation.setInsertPosition(position);
            
            // 解析目标选择器（如div[0]）
            java.util.regex.Pattern pattern = java.util.regex.Pattern.compile("(\\w+)\\[(\\d+)\\]");
            java.util.regex.Matcher matcher = pattern.matcher(content);
            if (matcher.find()) {
                String selector = matcher.group(1) + "[" + matcher.group(2) + "]";
                operation.setInsertReference(selector);
            }
            
            return operation;
        } finally {
            popState();
        }
    }
    
    // 辅助方法
    
    private void pushState(ParseState state) {
        stateStack.push(currentState);
        currentState = state;
    }
    
    private void popState() {
        if (!stateStack.isEmpty()) {
            currentState = stateStack.pop();
        }
    }
    
    private boolean hasMoreFragments() {
        return currentFragment < fragments.size();
    }
    
    private CodeFragment getCurrentFragment() {
        return fragments.get(currentFragment);
    }
    
    private void advance() {
        currentFragment++;
    }
    
    private boolean isElement(String content) {
        return content.matches("^\\w+.*\\{.*\\}$");
    }
    
    private TemplateDefinitionNode.TemplateType parseTemplateType(String type) {
        switch (type) {
            case "@Element":
                return TemplateDefinitionNode.TemplateType.ELEMENT;
            case "@Style":
                return TemplateDefinitionNode.TemplateType.STYLE;
            case "@Var":
                return TemplateDefinitionNode.TemplateType.VAR;
            default:
                return TemplateDefinitionNode.TemplateType.ELEMENT;
        }
    }
    
    private void parseTemplateContent(TemplateDefinitionNode template, String content) {
        // 简化实现
    }
    
    private void parseCustomContent(CustomDefinitionNode custom, String content) {
        // 简化实现
    }
    
    private CHTLASTNode parseCHTLStructure(String content) {
        // 处理其他CHTL结构
        return null;
    }
    
    private CHTLASTNode parseCHTLJSFragment(String content) {
        // 处理CHTL JS片段
        return null;
    }
    
    private CHTLASTNode parseCSSFragment(String content) {
        // 处理CSS片段
        return null;
    }
    
    private CHTLASTNode parseJavaScriptFragment(String content) {
        // 处理JavaScript片段
        return null;
    }
    
    public List<String> getErrors() {
        return context.getErrors().stream()
            .map(e -> e.getMessage())
            .collect(java.util.stream.Collectors.toList());
    }
}
