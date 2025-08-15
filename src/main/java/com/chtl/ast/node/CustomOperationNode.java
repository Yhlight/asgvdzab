package com.chtl.ast.node;

import com.chtl.ast.*;
import java.util.ArrayList;
import java.util.List;

/**
 * 自定义操作节点
 * 对应CHTL中的delete、insert等操作
 */
public class CustomOperationNode extends AbstractCHTLASTNode {
    
    public enum OperationType {
        DELETE,     // 删除
        INSERT,     // 插入
        INHERIT     // 继承
    }
    
    public enum InsertPosition {
        AFTER,      // 在...之后
        BEFORE,     // 在...之前
        REPLACE,    // 替换
        AT_TOP,     // 在顶部
        AT_BOTTOM   // 在底部
    }
    
    private OperationType operationType;
    private InsertPosition insertPosition; // 仅用于INSERT操作
    
    // DELETE操作的目标
    private List<String> deleteTargets; // 可以是属性名、选择器、索引访问等
    
    // INSERT操作的位置参考
    private String insertReference; // 如div[0]
    
    public CustomOperationNode(OperationType operationType) {
        super(NodeType.CUSTOM_OPERATION);
        this.operationType = operationType;
        this.deleteTargets = new ArrayList<>();
    }
    
    public OperationType getOperationType() {
        return operationType;
    }
    
    public void setOperationType(OperationType operationType) {
        this.operationType = operationType;
    }
    
    public InsertPosition getInsertPosition() {
        return insertPosition;
    }
    
    public void setInsertPosition(InsertPosition insertPosition) {
        this.insertPosition = insertPosition;
    }
    
    public List<String> getDeleteTargets() {
        return deleteTargets;
    }
    
    public void addDeleteTarget(String target) {
        if (target != null && !deleteTargets.contains(target)) {
            deleteTargets.add(target);
        }
    }
    
    public String getInsertReference() {
        return insertReference;
    }
    
    public void setInsertReference(String insertReference) {
        this.insertReference = insertReference;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitCustomOperation(this);
    }
}