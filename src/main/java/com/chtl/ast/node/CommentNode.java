package com.chtl.ast.node;

import com.chtl.ast.*;

/**
 * 注释节点
 * 对应CHTL中的三种注释：
 * - // 行注释
 * - /* */ 块注释
 * - -- 生成器识别的注释
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