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
可以理解为Editor代码，编辑器UI
## Global Objects
### figma.widget
#### figma.widget.register(component: FunctionalWidget<any>): void
## Component Types
### AutoLayout
### Frame

### Text
```tsx
<Text fontFamily="Inter" fontSize={20}>
    Hello Widget
</Text>
```
### Span
## Data Types
### AlignItems
### ArcData