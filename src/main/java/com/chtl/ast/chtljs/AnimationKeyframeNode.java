package com.chtl.ast.chtljs;

import java.util.HashMap;
import java.util.Map;

/**
 * 动画关键帧节点
 * 对应when数组中的关键帧对象
 */
public class AnimationKeyframeNode extends AbstractCHTLJSASTNode {
    private double at; // 时间点(0-1)
    private Map<String, String> cssProperties; // CSS属性
    
    public AnimationKeyframeNode(double at) {
        super(NodeType.ANIMATION_KEYFRAME);
        this.at = at;
        this.cssProperties = new HashMap<>();
    }
    
    public double getAt() {
        return at;
    }
    
    public void setAt(double at) {
        this.at = at;
    }
    
    public Map<String, String> getCssProperties() {
        return cssProperties;
    }
    
    public void addCssProperty(String property, String value) {
        cssProperties.put(property, value);
    }
    
    @Override
    public void accept(CHTLJSASTVisitor visitor) {
        visitor.visitAnimationKeyframe(this);
    }
}