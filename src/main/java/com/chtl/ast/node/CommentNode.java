package com.chtl.ast.node;

import com.chtl.ast.*;

/**
 * 注释节点
 * 对应CHTL中的三种注释：
 * 1. 行注释 (双斜线)
 * 2. 块注释 (斜线星号)
 * 3. 生成器识别的注释 (双横线)
 */
public class CommentNode extends AbstractCHTLASTNode {
    
    public enum CommentType {
        LINE_COMMENT,        // //
        BLOCK_COMMENT,       // /* */
        GENERATOR_COMMENT    // --
    }
    
    private CommentType commentType;
    private String content;
    
    public CommentNode(CommentType commentType, String content) {
        super(NodeType.COMMENT);
        this.commentType = commentType;
        this.content = content;
    }
    
    public CommentType getCommentType() {
        return commentType;
    }
    
    public void setCommentType(CommentType commentType) {
        this.commentType = commentType;
    }
    
    public String getContent() {
        return content;
    }
    
    public void setContent(String content) {
        this.content = content;
    }
    
    /**
     * 判断是否是生成器注释
     */
    public boolean isGeneratorComment() {
        return commentType == CommentType.GENERATOR_COMMENT;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitComment(this);
    }
}