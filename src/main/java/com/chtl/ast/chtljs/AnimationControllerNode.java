package com.chtl.ast.chtljs;import com.chtl.ast.NodeType;


import com.chtl.chtljs.ast.AbstractCHTLJSASTNode;



/**
 * 动画控制器节点
 * 表示animate函数返回的动画控制对象
 */
public class AnimationControllerNode extends AbstractCHTLJSASTNode {
    private String animationId;
    private AnimationConfigNode config;
    
    public AnimationControllerNode() {
        // // super(); // NodeType not available // NodeType not available
    }
    
    public String getAnimationId() {
        return animationId;
    }
    
    public void setAnimationId(String animationId) {
        this.animationId = animationId;
    }
    
    public AnimationConfigNode getConfig() {
        return config;
    }
    
    public void setConfig(AnimationConfigNode config) {
        this.config = config;
    }
    
    @Override
    public void accept(CHTLJSASTVisitor visitor) {
        visitor.visitAnimationController(this);
    }
}