# Chtholly Module - 珂朵莉官方主题模块

> "世界上最幸福的女孩" - 献给珂朵莉·诺塔·瑟尼欧里斯

## 模块介绍

Chtholly是CHTL的官方主题模块，以《末日时在做什么？有没有空？可以来拯救吗？》中的珂朵莉为主题设计。这个模块包含了完整的UI组件库和JavaScript扩展功能。

### 特色功能

- 🎨 **珂朵莉主题配色** - 天空蓝、金色、羽毛白等专属色彩
- 🪶 **羽毛飘落效果** - 动态的羽毛飘落动画
- 💝 **记忆卡片组件** - 展示珂朵莉的故事
- 🌟 **渐变文字效果** - 华丽的渐变文字生成
- 🖼️ **字符画转换** - 将珂朵莉的图片转换为ASCII艺术

## 安装方法

```bash
# 将模块放置在项目的module文件夹中
cp -r Chtholly /your-project/module/
```

## 使用方法

### 1. 导入模块

```chtl
// 导入UI组件
[Import] @Chtl from chtl::Chtholly

// 导入JS扩展
[Import] @CJmod from chtl::Chtholly
```

### 2. 使用主题组件

#### 创建珂朵莉主题页面

```chtl
@Element chtholly::ChthollyPage {
    // 页面内容
}
```

#### 使用珂朵莉卡片

```chtl
div {
    @Style chtholly::ChthollyCard;
    h3 { text "标题"; }
    p { text "内容"; }
}
```

#### 创建珂朵莉引言

```chtl
@Element chtholly::ChthollyQuote[
    "请把我，变成只为你一人存在的东西吧。",
    "珂朵莉"
];
```

### 3. 使用JS扩展功能

#### 图片转字符画

```chtl
script {
    const asciiArt = printMylove({
        url: "image.jpg",
        mode: "ASCII",    // 或 "Pixel"
        width: 80,
        height: 40
    });
    console.log(asciiArt);
}
```

#### 创建羽毛飘落效果

```chtl
div id="container" {
    script {
        createFeatherRain("container", 20);
    }
}
```

#### 创建渐变文字

```chtl
h1 {
    script {
        const gradient = createGradientText(
            "珂朵莉",
            ["#FFD700", "#87CEEB", "#FFD700"]
        );
        {{&}}.innerHTML = gradient;
    }
}
```

## 组件列表

### 样式组件 (@Style)

- `ChthollyTheme` - 基础主题样式
- `ChthollySkyBlue` - 天空蓝渐变
- `ChthollyGolden` - 金色光晕
- `ChthollyCard` - 卡片样式
- `ChthollyButton` - 按钮样式
- `ChthollyBadge` - 徽章样式
- `ChthollyGradient` - 渐变背景
- `ChthollyGlow` - 光晕效果
- `ChthollyShadow` - 阴影效果

### 元素组件 (@Element)

- `ChthollyPage` - 页面容器
- `ChthollyContainer` - 内容容器
- `ChthollyHero` - 英雄区域
- `ChthollyNav` - 导航栏
- `ChthollyModal` - 模态框
- `ChthollyGallery` - 画廊
- `ChthollyQuote` - 引言
- `ChthollyMemory` - 记忆卡片
- `ChthollyFeather` - 羽毛装饰
- `ChthollySection` - 内容区块

### 变量组件 (@Var)

- `ChthollyColors` - 颜色变量
- `ChthollyFonts` - 字体变量
- `ChthollySpacing` - 间距变量
- `ChthollyAnimations` - 动画变量

## 主题配色

```css
--chtholly-sky-blue: #87CEEB;     /* 天空蓝 */
--chtholly-golden: #FFD700;        /* 金色 */
--chtholly-red: #DC143C;           /* 红色 */
--chtholly-feather: #F0F8FF;       /* 羽毛白 */
```

## 关于珂朵莉

珂朵莉·诺塔·瑟尼欧里斯（Chtholly Nota Seniorious）是轻小说《末日时在做什么？有没有空？可以来拯救吗？》的女主角。她是黄金妖精，拥有着蓝色的头发和红色的眼睛。

这个模块是为了纪念这位"世界上最幸福的女孩"而创建的。通过使用这个模块，我们希望珂朵莉的美好能够永远留在代码中。

## 许可证

MIT License - 愿所有人都能找到属于自己的幸福

---

*"即使明天就是世界末日，我今天也要种下苹果树。" - 珂朵莉*