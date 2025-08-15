package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.*;

/**
 * 导入语句节点
 * 对应CHTL中的[Import] ...
 */
public class ImportStatementNode extends AbstractCHTLASTNode {
    
    public enum ImportType {
        HTML,           // @Html
        STYLE,          // @Style  
        JAVASCRIPT,     // @JavaScript
        CHTL,           // @Chtl
        CJMOD,          // @CJmod
        CUSTOM_ELEMENT, // [Custom] @Element
        CUSTOM_STYLE,   // [Custom] @Style
        CUSTOM_VAR,     // [Custom] @Var
        TEMPLATE_ELEMENT, // [Template] @Element
        TEMPLATE_STYLE,   // [Template] @Style
        TEMPLATE_VAR      // [Template] @Var
    }
    
    private ImportType importType;
    private String targetName; // 要导入的元素/样式组/变量组名称
    private String fromPath;   // 文件路径
    private String asName;     // 重命名（可选）
    
    public ImportStatementNode(ImportType importType) {
        super(NodeType.IMPORT_STATEMENT);
        this.importType = importType;
    }
    
    public ImportType getImportType() {
        return importType;
    }
    
    public void setImportType(ImportType importType) {
        this.importType = importType;
    }
    
    public String getTargetName() {
        return targetName;
    }
    
    public void setTargetName(String targetName) {
        this.targetName = targetName;
    }
    
    public String getFromPath() {
        return fromPath;
    }
    
    public void setFromPath(String fromPath) {
        this.fromPath = fromPath;
    }
    
    public String getAsName() {
        return asName;
    }
    
    public void setAsName(String asName) {
        this.asName = asName;
    }
    
    /**
     * 将路径中的.转换为/
     */
    public String getNormalizedPath() {
        return fromPath != null ? fromPath.replace('.', '/') : null;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitImportStatement(this);
    }
}