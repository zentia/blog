---
sidebar_position: 1
---

# 数组
```js
let a = [1,2,3]     // number[]
let d = [1, 'a']    // (string | number)[]

let f = ['red']
f.push('blue')
f.push(true)        // Error TS2345: Argument of type 'true' is not assignable to parameter of type 'string'.
```

一般情况下，数组应该保持同质。也就是说，不要在同一个数组中存储苹果、橙子和数字。