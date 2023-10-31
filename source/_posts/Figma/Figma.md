---
title: Figma
tags:
    - Figma
categories:
    - Figma
---
# Plugin APIs
可以理解为Runtime代码，场景逻辑交互
## Global Objects
### figma.ui
### figma.codegen
### figma.timer
### figma.viewport
### figma.clientStorage
### figma.paramters
### figma.variables
### figma.teamLibarry
### figma.textreview
### figma.createReactangle
### figma.createPage
### figma.createStar(): StarNode
```ts
const star = figma.createStar()
```
## MODE Types
## SHARED NODE PROPERTIES
## DATA TYPES
### StarNode

# Widget APIs
The Widget API enables you to create custom interactive on-canvas nodes with rich user interfaces that can be applied to the Figma or FigJam canvas.
## Global Objects
### figma.widget
#### figma.widget.register(component: FunctionalWidget<any>): void
## Component Types
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

# 参考
- [1][Compare the Figma APIs](https://www.figma.com/developers/compare-apis)
- [2][用 Vue 3 开发 Figma 插件](https://juejin.cn/post/7084639146915921956)