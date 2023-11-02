---
title: Figma
tags:
    - Figma
categories:
    - Figma
---
Page是由Frame构成的
# Plugin APIs
## Global Objects
### [figma](https://www.figma.com/plugin-docs/api/figma/)
#### [ui](https://www.figma.com/plugin-docs/api/figma-ui/)
ui是通过[figma.showUI](https://www.figma.com/plugin-docs/api/properties/figma-showui/)创建的。
##### [onmessage](https://www.figma.com/plugin-docs/api/properties/figma-ui-onmessage/)
#### figma.codegen
#### figma.timer
#### [viewport](https://www.figma.com/plugin-docs/api/figma-viewport/)
#### [clientStorage](https://www.figma.com/plugin-docs/api/figma-clientStorage/)
#### figma.paramters
#### figma.variables
#### figma.teamLibarry
#### figma.textreview
#### figma.createReactangle
#### figma.createPage
#### figma.createStar(): StarNode
#### [showUI](https://www.figma.com/plugin-docs/api/properties/figma-showui/)

#### [createRectangle](https://www.figma.com/plugin-docs/api/properties/figma-createrectangle/)
createRectangle(): [RectangleNode](https://www.figma.com/plugin-docs/api/RectangleNode/)
#### [createFrame](https://www.figma.com/plugin-docs/api/properties/figma-createframe/)
```ts
createFrame(): FrameNode
```
#### [createComponent](https://www.figma.com/plugin-docs/api/properties/figma-createcomponent/)
createComponent(): [ComponentNode](https://www.figma.com/plugin-docs/api/ComponentNode/)
#### [createPage](https://www.figma.com/plugin-docs/api/properties/figma-createpage/)
`createPage(): PageNode`
#### [loadFontAsync](https://www.figma.com/plugin-docs/api/properties/figma-loadfontasync/)
loadFontAsync(fontName: [FontName](https://www.figma.com/plugin-docs/api/FontName/)): Promise<void>
#### [createImage](https://www.figma.com/plugin-docs/api/properties/figma-createimage/)
#### [notify](https://www.figma.com/plugin-docs/api/properties/figma-notify/)
在屏幕的下面弹出提示
notify(message: string, options?: NotificationOptions): NotificationHandler
#### on
##### ArgFreeEventType
```ts
type ArgFreeEventType =
  | 'selectionchange'
  | 'currentpagechange'
  | 'close'
  | 'timerstart'
  | 'timerstop'
  | 'timerpause'
  | 'timerresume'
  | 'timeradjust'
  | 'timerdone'

on(type: ArgFreeEventType, callback: () => void): void
on(type: 'run', callback: (event: RunEvent) => void): void
on(type: 'drop', callback: (event: DropEvent) => boolean): void
on(type: 'documentchange', callback: (event: DocumentChangeEvent) => void): void
```
```ts
const star = figma.createStar()
```
#### once
#### [off](https://www.figma.com/plugin-docs/api/properties/figma-off/)
#### [group](https://www.figma.com/plugin-docs/api/properties/figma-group/)
打组
#### [ungroup](https://www.figma.com/plugin-docs/api/properties/figma-ungroup/)
### [fetch](https://www.figma.com/plugin-docs/api/properties/global-fetch/)
网络能力
## MODE Types
## SHARED NODE PROPERTIES
## DATA TYPES
### [RectangleNode](https://www.figma.com/plugin-docs/api/RectangleNode/)
### [FontName](https://www.figma.com/plugin-docs/api/FontName/)
### StarNode
### [InstanceNode](https://www.figma.com/plugin-docs/api/InstanceNode/)
实例是组件的复制体

# Widget APIs
The Widget API enables you to create custom interactive on-canvas nodes with rich user interfaces that can be applied to the Figma or FigJam canvas.
## Global Objects
### figma.widget
#### figma.widget.register(component: FunctionalWidget<any>): void
## Component Types
组件和实例：组件是母体，实例是复制体，组件可以删除和修改，不可逆向为组。
### AutoLayout
### Frame

### [Text](https://www.figma.com/widget-docs/api/component-Text/)
```tsx
<Text fontFamily="Inter" fontSize={20}>
    Hello Widget
</Text>
```
### [Span](https://www.figma.com/widget-docs/api/component-Span/)
```tsx
<Text
  fill="#0F0"
  fontSize={20}
  fontFamily="Roboto"
  fontWeight={400}
  textCase="upper"
  textDecoration="underline"
>
  Hello{' '}
  <Span
    fontSize={50}
    fontFamily="Poppins"
    fontWeight={800}
    textCase="original"
    textDecoration="none"
  >
    Worl
    <Span fontSize={30} fill="#F00" italic>
      d
    </Span>
  </Span>
</Text>
```
### Image
```tsx
<Image
  // Pass a data uri directly as the image
  src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAsAAAALCAYAAACprHcmAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAECSURBVHgBpY87TwJBFIXPnVkhbqQQE7UQNWhrsdpLI40FsdSYGGNvbWFhZ2Jj+AWER0fFD6CAhhYCod8GGmCLJRAI2Rl22LDh1RBOc1/fzb0H2EK0WPSfzj+lxG3wMIpAOKpaZfpPpddgKxG510BllSsw6MGAEAYl0zWVMn+L8boEzOXD0oRwrI1vZF9ESRetWO94XMjEDwxb0xttTF6txyNbzbU5mHmWhhtiQ3aGSkQmTH129YJLunJjdQned9DshkbF8d7o4cRiOSB0475ld+JUnTM+/Pb1d0p8ck2eKXN49/OOFfkGOXfCLnjpdamNDfLhgfFdIyE+GOg3AJHHrpoC5YtKfAfixH0AAAAASUVORK5CYII="

  width={100}
  height={100}
/>
```

## Data Types
### AlignItems
### ArcData

# REST API

# FigJam
FigJam是Figma中的一种特殊的文件类型，可以实现多人协作。

# 快捷键
- `Home`切换不同的Frame
- `Ctrl+1`切换不同的页签
- `Ctrl+2`切换不同的页签
- `Shift+1`切换全部视图
- `Shift+2`切换当前视图
- `F`创建画板
- `T`快速唤醒文字工具


# 参考
- [1][Compare the Figma APIs](https://www.figma.com/developers/compare-apis)
- [2][用 Vue 3 开发 Figma 插件](https://juejin.cn/post/7084639146915921956)
- [3][FigJam 指南](https://figmachina.com/figjam/)
- [4][Figma插件开发浅浅谈](https://gist.github.com/banyudu/f7472f935897adfbb54cc8eb38dc5373)
- [5][Figma 插件开发 - Vite 环境搭建](https://blog.csdn.net/qq_53225741/article/details/125000207)
- [6][API Reference](https://www.figma.com/plugin-docs/api/api-reference/)
- [7][Figma 学习路径](https://www.figma.cool/learning-paths)