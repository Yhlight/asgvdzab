# Chtholly 珂朵莉模块

> 珂朵莉对于我来说，是一个很特别的角色，是我一直喜欢着的人物，我希望我能让珂朵莉成为CHTL中重要的模块

## 模块介绍

Chtholly模块是CHTL的官方主题模块，提供了美观的UI组件和实用的CHTL JS扩展功能。模块包含两部分：

- **CMOD**：提供珂朵莉主题的样式和UI组件
- **CJMOD**：提供图像处理等扩展功能

## 安装

```chtl
// 导入CMOD（样式和组件）
[Import] @Chtl from Chtholly

// 导入CJMOD（JS扩展功能）
[Import] @CJmod from Chtholly
```

## CMOD功能

### 颜色变量

```chtl
@Var ChthollyColors from chtholly {
    primary: "#FF6B9D";       // 珂朵莉红
    secondary: "#C44569";     // 深红
    accent: "#FFE66D";        // 金色
    background: "#FFF1E6";    // 浅暖色背景
    text: "#4A4A4A";         // 文字颜色
    white: "#FFFFFF";
    shadow: "rgba(196, 69, 105, 0.15)";
}
```

### UI组件

#### ChthollyPage - 页面容器
```chtl
@Element ChthollyPage from chtholly {
    // 页面内容
}
```

#### ChthollyTitle - 标题
```chtl
@Element ChthollyTitle from chtholly {
    珂朵莉 - 世界上最幸福的女孩
}
```

#### ChthollyCard - 卡片
```chtl
@Element ChthollyCard from chtholly {
    <h3>卡片标题</h3>
    <p>卡片内容</p>
}
```

#### ChthollyButton - 按钮
```chtl
@Element ChthollyButton from chtholly {
    点击我
}
```

#### ChthollyGallery - 图片画廊
```chtl
@Element ChthollyGallery from chtholly {
    <img src="image1.jpg" alt="图片1"/>
    <img src="image2.jpg" alt="图片2"/>
    <img src="image3.jpg" alt="图片3"/>
}
```

## CJMOD功能

### printMylove

将图片转换为字符画或像素块形式，输出到控制台：

```chtl
script {
    const art = printMylove({
        url: "chtholly.jpg",
        mode: "ASCII",     // 模式：ASCII或Pixel
        width: 80,         // 宽度：支持数字、百分比、CSS单位(px, em)
        height: 40,        // 高度：支持数字、百分比、CSS单位(px, em)
        scale: 0.5         // 缩放倍数：等比缩放（可选，优先于width/height）
    });
    console.log(art);
}
```

**参数说明**：
- `url`: 图片地址（必需）
- `mode`: 转换模式
  - `"ASCII"`: ASCII字符画（默认）
  - `"Pixel"`: Unicode像素块
- `width`: 输出宽度
  - 纯数字：字符数
  - 百分比：如`"50%"`，相对原图宽度
  - CSS单位：如`"100px"`、`"5em"`
- `height`: 输出高度，格式同width
- `scale`: 等比缩放倍数（可选）
  - 如果提供，将忽略width/height
  - 如`0.5`表示缩小到原图的50%

### createFeatherRain

创建羽毛飘落效果：

```chtl
script {
    createFeatherRain("container-id", 50);  // 在指定容器创建50片羽毛
}
```

### createGradientText

为文字添加渐变效果：

```chtl
script {
    const element = document.querySelector('.title');
    createGradientText(element, ["#FF6B9D", "#FFE66D", "#C44569"]);
}
```

## 完整示例

```chtl
// 导入模块
[Import] @Chtl from Chtholly
[Import] @CJmod from Chtholly

// 使用组件创建页面
@Element ChthollyPage from chtholly {
    @Element ChthollyTitle from chtholly {
        珂朵莉的世界
    }
    
    @Element ChthollyCard from chtholly {
        <p>这是一个使用珂朵莉主题的示例页面。</p>
        
        @Element ChthollyButton from chtholly {
            text { 查看更多 }
        }
    }
    
    <div id="art-container">
        style {
            {{&}} {
                font-family: monospace;
                white-space: pre;
                line-height: 1.2;
                background: #000;
                color: #fff;
                padding: 20px;
                overflow: auto;
            }
        }
    </div>
    
    script {
        // 使用CJMOD功能
        window.onload = function() {
            // 生成字符画
            const art = printMylove({
                url: "/images/chtholly.jpg",
                mode: "ASCII",
                scale: 0.8
            });
            
            document.getElementById('art-container').textContent = art;
            
            // 添加羽毛效果
            createFeatherRain("body", 30);
        };
    }
}
```

## 版本历史

- v1.0.0 - 初始版本
  - 提供基础UI组件
  - 实现printMylove功能
  - 支持羽毛飘落和渐变文字效果