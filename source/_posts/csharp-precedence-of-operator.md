---
title: C#运算符优先级
mathjax: true
date: 2019-05-24 17:45:07
tags:
    - CSharp
categories: CSharp
---

<table>
   <tr><th>优先级</th><th>运算符</th><th>使用形式</th><th>结合方向</th></tr>
   <tr><th rowspan="4">1</th><td>[]</td><td>数组名[整形表达式]</td><td rowspan="4">左到右</td></tr>
   <tr><td>()</td></tr>
   <tr><td>.</td></tr>
   <tr><td>-></td></tr>
   <tr><th rowspan="5">2</th><td>-</td><td>-表达式</td><td rowspan="5">右到左</td></tr>
   <tr><td>(类型)</td></tr>
   <tr><td>++</td></tr>
   <tr><td>--</td></tr>
   <tr><td>*</td><td>*指针表达式</td></tr>
   <tr><th rowspan="5">6</th><td>-</td><td>-表达式</td><td rowspan="5">左到右</td></tr>
   <tr><td>></td></tr>
   <tr><td>>=</td></tr>
   <tr><td><</td></tr>
   <tr><td> <= <td> 表达式<=表达式 
   <tr><th rowspan="2">7</th><td>==</td><td>表达式==表达式</td><td rowspan="2">左到右</td></tr>
   <tr><td>!=</td></tr>
</table>
