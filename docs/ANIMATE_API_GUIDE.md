# CHTL JS animate函数API指南

## 概述

`animate`函数是CHTL JS提供的强大动画API，用于创建流畅的Web动画。该函数返回一个动画控制对象，允许开发者精确控制动画的播放状态。

## 基本用法

```chtl
script {
    // 获取目标元素（必须）
    const element = {{.box}};
    
    // 创建动画并获取控制对象
    const animation = animate({
        target: element,       // 目标元素（必需参数）
        duration: 1000,        // 动画持续时间（毫秒）
        easing: ease-in-out,   // 缓动函数
        delay: 100,           // 延迟开始（毫秒）
        loop: 3,              // 循环次数（-1为无限循环）
        direction: normal,     // 播放方向
        
        // 起始状态
        begin: {
            opacity: 0,
            transform: 'scale(0.8)'
        },
        
        // 关键帧
        when: [
            {
                at: 0.5,      // 50%时刻
                opacity: 0.8,
                transform: 'scale(1.1)'
            }
        ],
        
        // 结束状态
        end: {
            opacity: 1,
            transform: 'scale(1)'
        },
        
        // 完成回调
        callback: function() {
            console.log('动画完成');
        }
    });
}
```

## 动画控制对象

`animate`函数返回一个控制对象，提供以下方法：

### 方法列表

#### `play()`
开始或恢复动画播放。
```chtl
animation.play();
```

#### `pause()`
暂停动画。
```chtl
animation.pause();
```

#### `stop()`
停止动画并重置到初始状态。
```chtl
animation.stop();
```

#### `reverse()`
反转动画方向（交换起始和结束状态）。
```chtl
animation.reverse();
```

#### `onComplete(callback)`
添加动画完成回调。
```chtl
animation.onComplete(function() {
    console.log('额外的完成回调');
});
```

#### `isPlaying()`
检查动画是否正在播放。
```chtl
if (animation.isPlaying()) {
    console.log('动画正在播放');
}
```

#### `isPaused()`
检查动画是否已暂停。
```chtl
if (animation.isPaused()) {
    console.log('动画已暂停');
}
```

## 参数说明

所有参数都是可选的，可以按任意顺序提供：

| 参数 | 类型 | 默认值 | 说明 |
|------|------|--------|------|
| `target` | Element/string | **必需** | 目标DOM元素或选择器 |
| `duration` | number | 1000 | 动画持续时间（毫秒） |
| `easing` | string | 'linear' | 缓动函数：linear, ease, ease-in, ease-out, ease-in-out |
| `delay` | number | 0 | 动画开始前的延迟（毫秒） |
| `loop` | number | 1 | 循环次数，-1表示无限循环 |
| `direction` | string | 'normal' | 播放方向：normal, reverse, alternate |
| `begin` | object | {} | 动画起始状态的CSS属性 |
| `end` | object | {} | 动画结束状态的CSS属性 |
| `when` | array | [] | 关键帧数组，每个元素包含at(0-1)和CSS属性 |
| `callback` | function | null | 动画完成时的回调函数 |

## 完整示例

```chtl
div {
    class = "box";
    
    button {
        text { 播放动画 }
        
        script {
            {{&}}.listen({
                click: function() {
                    // 获取要动画的元素
                    const box = {{.box}};
                    
                    // 创建复杂动画
                    const anim = animate({
                        target: box,        // 指定目标元素
                        duration: 2000,
                        easing: ease-in-out,
                        
                        when: [
                            {
                                at: 0.0,
                                transform: 'translateX(0) rotate(0deg)',
                                background: '#ff0000'
                            },
                            {
                                at: 0.25,
                                transform: 'translateX(100px) rotate(90deg)',
                                background: '#00ff00'
                            },
                            {
                                at: 0.5,
                                transform: 'translateX(200px) rotate(180deg)',
                                background: '#0000ff'
                            },
                            {
                                at: 0.75,
                                transform: 'translateX(100px) rotate(270deg)',
                                background: '#ffff00'
                            },
                            {
                                at: 1.0,
                                transform: 'translateX(0) rotate(360deg)',
                                background: '#ff0000'
                            }
                        ],
                        
                        loop: -1,  // 无限循环
                        direction: alternate  // 交替播放
                    });
                    
                    // 5秒后暂停
                    setTimeout(() => anim.pause(), 5000);
                    
                    // 8秒后继续
                    setTimeout(() => anim.play(), 8000);
                    
                    // 12秒后停止
                    setTimeout(() => anim.stop(), 12000);
                }
            });
        }
    }
}
```

## 更多示例

### 1. 使用选择器作为目标

```chtl
script {
    // 可以直接传入选择器字符串
    animate({
        target: '.my-element',
        duration: 500,
        begin: { opacity: 0 },
        end: { opacity: 1 }
    });
}
```

### 2. 同时动画多个属性

```chtl
script {
    const card = {{.card}};
    
    animate({
        target: card,
        duration: 800,
        easing: ease-out,
        begin: {
            opacity: 0,
            transform: 'translateY(20px) scale(0.9)',
            'background-color': '#ffffff'
        },
        end: {
            opacity: 1,
            transform: 'translateY(0) scale(1)',
            'background-color': '#f0f0f0'
        }
    });
}
```

### 3. 使用增强选择器直接动画

```chtl
script {
    // 配合CHTL JS的增强选择器
    {{#myButton}}.listen({
        click: function() {
            // 动画自身
            animate({
                target: this,
                duration: 200,
                when: [
                    { at: 0.0, transform: 'scale(1)' },
                    { at: 0.5, transform: 'scale(0.95)' },
                    { at: 1.0, transform: 'scale(1)' }
                ]
            });
        }
    });
}
```

## 注意事项

1. **`target`参数是必需的** - 必须指定要动画的DOM元素
2. 动画自动开始播放，无需手动调用`play()`
3. 支持链式调用：`animation.pause().onComplete(fn).play()`
4. `reverse()`方法会交换`begin`和`end`状态
5. 暂停时会记录当前进度，恢复时从暂停位置继续
6. `stop()`方法会将元素重置到`begin`状态
7. 支持所有CSS属性的动画，包括transform、opacity、color等
8. 数值属性会自动插值，非数值属性在50%时切换