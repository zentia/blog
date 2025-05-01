---
sidebar_position: 1
---

# Transform

```ts
type Transform = [
    [number, number, number],
    [number, number, number]
]
```

A transformation matrix is standard way in computer graphics to represent translation and rotation. These are the top tow rows of a 3x3 matrix. The bottom row of the matrix is assumed to be [0, 0, 1]. This is known as an affine transform and is enough to represent translation,rotation, and skew.

The identity transform is [[1,0,0],[0,1,0]].

A translation matrix will typically look like:
```ts
[[1,0,tx],
[0,1,ty]]
```
and a totation matrix will typically look like:
```ts
[[cos(angle), sin(angle),0],
[-sin(angle), cos(angle), 0]]
```

Another way to think about this transform is as three vectors:

- The x axis `(t[0][0], t[1][0])`
- The y axis `(t[0][1], t[1][1])`
- The translation offset `(t[0][2], t[1][2])`

The most commmon usage of the Transform matrix is the relativeTransform property. This particular usage of the matrix has a few additional restrictions.