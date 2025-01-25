---
sidebar_position: 1
---
# React
## side effect 副效应
函数如果涉及到如生成日志、存储数据、改变状态等之类的逻辑称之为副效应。react使用钩子来实现。
## useEffect
`useEffect`是通用的副效应的钩子。
```tsx
import React, { useEffect } from 'react';

function Welcome(props) {
    useEffect(() => {
        document.title = '加载完成';
    });
    return <h1>Hello, {props.name}</h1>;
}
```
