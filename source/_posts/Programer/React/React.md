---
title: React
tags:
- React
categories:
- React
---

# Hook

## useEffect
useEffect的参数是一个函数，它就是所要完成的副效应。组件加载以后，React就会执行这个函数。
`useEffect`的作用就是指定一个副效应函数，组件每渲染一次，该函数就自动执行一次。
有时候，我们不希望`useEffect`每次渲染都执行，这时可以使用它的第二个函数，使用一个数组指定副效应函数的依赖项，只有依赖项发生变化，才会重新渲染。

## useState

