# CHTL JS animate方法API指南

## 概述

`animate`是CHTL JS为DOM元素扩展的方法，用于创建流畅的Web动画。当通过增强选择器`{{}}`获取元素时，元素会自动拥有`animate`方法。该方法返回一个动画控制对象，允许开发者精确控制动画的播放状态。

## 基本用法

```chtl
script {
    // 通过增强选择器获取元素，自动拥有animate方法
    const box = {{.box}};
    
    // 直接在元素上调用animate方法
    const animation = box.animate({
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

`animate`方法返回一个控制对象，提供以下方法：

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
                    
                    // 在元素上调用animate方法
                    const anim = box.animate({
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

### 1. 链式调用

```chtl
script {
    // 可以链式调用多个方法
    {{.card}}
        .animate({
            duration: 500,
            begin: { opacity: 0 },
            end: { opacity: 1 }
        })
        .onComplete(function() {
            console.log('淡入完成');
        });
}
```

### 2. 同时动画多个属性

```chtl
script {
    // 获取元素并动画多个属性
    {{.card}}.animate({
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

### 3. 在事件处理器中使用

```chtl
script {
    // 配合事件处理器
    {{#myButton}}.listen({
        click: function() {
            // this已经是扩展后的元素，直接调用animate
            this.animate({
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

### 4. 多元素动画

```chtl
script {
    // 为多个元素添加相同动画
    const cards = document.querySelectorAll('.card');
    
    cards.forEach(card => {
        // 扩展元素并添加动画
        _chtl.extendElement(card).animate({
            duration: 600,
            delay: 100 * Array.from(cards).indexOf(card), // 交错动画
            begin: { opacity: 0, transform: 'translateY(20px)' },
            end: { opacity: 1, transform: 'translateY(0)' }
        });
    });
}
```

## 注意事项

1. **通过增强选择器`{{}}`获取的元素会自动拥有`animate`方法**
2. 对于原生DOM元素，需要使用`_chtl.extendElement()`扩展后才能使用`animate`
3. 动画自动开始播放，无需手动调用`play()`
4. 支持链式调用：`element.animate(...).pause().onComplete(fn).play()`
5. `reverse()`方法会交换`begin`和`end`状态
6. 暂停时会记录当前进度，恢复时从暂停位置继续
7. `stop()`方法会将元素重置到`begin`状态
8. 支持所有CSS属性的动画，包括transform、opacity、color等
9. 数值属性会自动插值，非数值属性在50%时切换

## API一致性

CHTL JS为DOM元素扩展了三个核心方法：

```chtl
// 1. listen - 事件监听
{{.element}}.listen({
    click: handler,
    mouseenter: handler
});

// 2. delegate - 事件委托
{{.container}}.delegate({
    target: {{.item}},  // 或元素数组
    click: handler,
    mouseenter: handler
});

// 3. animate - 动画
{{.element}}.animate({
    duration: 1000,
    begin: { opacity: 0 },
    end: { opacity: 1 }
});
```

这三个方法提供了统一、直观的API，让DOM操作更加优雅。