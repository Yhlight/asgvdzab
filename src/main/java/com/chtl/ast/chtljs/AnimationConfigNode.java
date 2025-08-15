package com.chtl.ast.chtljs;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 动画配置节点
 * 对应animate函数的配置对象
 */
public class AnimationConfigNode extends AbstractCHTLJSASTNode {
    private Integer duration; // 动画持续时间(ms)
    private String easing;    // 缓动函数
    private Map<String, String> beginState; // 起始状态CSS
    private Map<String, String> endState;   // 终止状态CSS
    private List<AnimationKeyframeNode> keyframes; // when数组
    private Integer loop;     // 循环次数，-1表示无限循环
    private String direction; // 播放方向
    private Integer delay;    // 延迟时间(ms)
    private String callback;  // 回调函数
    
    public AnimationConfigNode() {
        super(NodeType.ANIMATION_CONFIG);
        this.beginState = new HashMap<>();
        this.endState = new HashMap<>();
        this.keyframes = new ArrayList<>();
    }
    
    public Integer getDuration() {
        return duration;
    }
    
    public void setDuration(Integer duration) {
        this.duration = duration;
    }
    
    public String getEasing() {
        return easing;
    }
    
    public void setEasing(String easing) {
        this.easing = easing;
    }
    
    public Map<String, String> getBeginState() {
        return beginState;
    }
    
    public void addBeginProperty(String property, String value) {
        beginState.put(property, value);
    }
    
    public Map<String, String> getEndState() {
        return endState;
    }
    
    public void addEndProperty(String property, String value) {
        endState.put(property, value);
    }
    
    public List<AnimationKeyframeNode> getKeyframes() {
        return keyframes;
    }
    
    public void addKeyframe(AnimationKeyframeNode keyframe) {
        if (keyframe != null) {
            keyframes.add(keyframe);
        }
    }
    
    public Integer getLoop() {
        return loop;
    }
    
    public void setLoop(Integer loop) {
        this.loop = loop;
    }
    
    public String getDirection() {
        return direction;
    }
    
    public void setDirection(String direction) {
        this.direction = direction;
    }
    
    public Integer getDelay() {
        return delay;
    }
    
    public void setDelay(Integer delay) {
        this.delay = delay;
    }
    
    public String getCallback() {
        return callback;
    }
    
    public void setCallback(String callback) {
        this.callback = callback;
    }
    
    @Override
    public void accept(CHTLJSASTVisitor visitor) {
        visitor.visitAnimationConfig(this);
    }
}