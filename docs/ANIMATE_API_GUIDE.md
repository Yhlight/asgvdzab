# CHTL JS animate函数API指南

## 概述

`animate`是CHTL JS提供的全局函数，用于创建流畅的Web动画。该函数通过`target`参数指定要动画的元素，并返回一个动画控制对象，允许开发者精确控制动画的播放状态。

## 基本用法

```chtl
script {
    // 使用增强选择器作为target
    const anim = animate({
        target: {{.box}},      // 必需：指定动画目标
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

## target参数支持的格式

`target`参数支持以下几种格式：

1. **增强选择器**：`{{.class}}`、`{{#id}}`、`{{element}}`
2. **DOM元素**：直接传入DOM元素对象
3. **选择器数组**：`[{{.class1}}, {{.class2}}]` - 同时动画多个元素
4. **DOM元素数组**：`[element1, element2]`
5. **NodeList/HTMLCollection**：`document.querySelectorAll('.items')`

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
| `target` | 见上方说明 | **必需** | 动画目标元素 |
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
                    // 创建复杂动画
                    const anim = animate({
                        target: {{.box}},  // 指定目标元素
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

### 1. 使用增强选择器数组

```chtl
script {
    // 同时动画多个元素
    const anim = animate({
        target: [{{.card1}}, {{.card2}}, {{.card3}}],
        duration: 500,
        begin: { opacity: 0 },
        end: { opacity: 1 }
    });
}
```

### 2. 使用DOM元素

```chtl
script {
    // 获取DOM元素后使用
    const box = {{.box}};
    
    animate({
        target: box,
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
            // 使用this作为target
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

### 4. 使用NodeList

```chtl
script {
    // 直接使用querySelectorAll的结果
    const cards = document.querySelectorAll('.card');
    
    animate({
        target: cards,  // NodeList作为target
        duration: 600,
        begin: { opacity: 0, transform: 'translateY(20px)' },
        end: { opacity: 1, transform: 'translateY(0)' }
    });
}
```

## 注意事项

1. **`target`参数是必需的** - 必须指定要动画的元素
2. `target`支持多种格式：增强选择器、DOM元素、数组等
3. 动画自动开始播放，无需手动调用`play()`
4. 支持链式调用：`anim.pause().onComplete(fn).play()`
5. `reverse()`方法会交换`begin`和`end`状态
6. 暂停时会记录当前进度，恢复时从暂停位置继续
7. `stop()`方法会将元素重置到`begin`状态
8. 支持所有CSS属性的动画，包括transform、opacity、color等
9. 数值属性会自动插值，非数值属性在50%时切换
10. 当target包含多个元素时，返回的控制器会同时控制所有元素

## CHTL JS核心API

CHTL JS提供了三个核心功能：

```chtl
// 1. listen - 事件监听（元素方法）
{{.element}}.listen({
    click: handler,
    mouseenter: handler
});

// 2. delegate - 事件委托（元素方法）
{{.container}}.delegate({
    target: {{.item}},  // 或元素数组
    click: handler,
    mouseenter: handler
});

// 3. animate - 动画（全局函数）
const anim = animate({
    target: {{.element}} || [{{.elem1}}, {{.elem2}}] || DOM对象,
    duration: 1000,
    begin: { opacity: 0 },
    end: { opacity: 1 }
});
```

`listen`和`delegate`是元素方法，需要明确的对象调用。而`animate`是全局函数，通过`target`参数指定动画对象，返回控制器以便精确控制动画。