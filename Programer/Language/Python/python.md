---
title: Python
mathjax: true
date: 2019-05-27 14:39:42
categories: 
- Language
- Python
---
# `__init__.py `
## 标识该目录是一个python的模块包（module package）
   如果你是使用python的相关IDE来进行开发，那么如果目录中存在该文件，该目录就会被识别为module package。
## 简化模块导入操作
 假设我们的模块包的目录结构如下：

```python
.
└── mypackage
    ├── subpackage_1
    │   ├── test11.py
    │   └── test12.py
    ├── subpackage_2
    │   ├── test21.py
    │   └── test22.py
    └── subpackage_3
        ├── test31.py
        └── test32.py
```
如果我们使用最直接的导入方式，将整个文件拷贝到工程目录下，然后直接导入：
```python
from mypackage.subpackage_1 import test11
from mypackage.subpackage_1 import test12
from mypackage.subpackage_2 import test21
from mypackage.subpackage_2 import test22
from mypackage.subpackage_3 import test31
from mypackage.subpackage_3 import test32
```
当然这个例子里面文件比较少，如果模块比较大，目录比较深的话，可能自己都记不清如何导入。（很有可能，哪怕只想导入一个模块都要在目录中找很久）
这种情况下，`__init__.py`就很有作用了。我们先来看看该文件是如何工作的。
## `__init__.py`是如何工作的？
实际上，如果目录中包含了`__init__.py`时，当用import导入该目录时，会执行`__init__.py`里面的代码。
我们在mypackage目录下增加一个`__init__.py`文件来做一个实验：
```python
.
└── mypackage
    ├── __init__.py
    ├── subpackage_1
    │   ├── test11.py
    │   └── test12.py
    ├── subpackage_2
    │   ├── test21.py
    │   └── test22.py
    └── subpackage_3
        ├── test31.py
        └── test32.py
```
# 魔法函数
## 前言
### 什么是魔法函数
所谓魔法函数（Magic Methods），是Python的一种高级语法，允许你在类中自定义函数（函数名格式一般为`__xx__`），并绑定到类的特殊方法中。比如在类A中自定义 `__str__()`函数，则在调用str(A())时，会自动调用`__str__()`函数，并返回相应的结果。在我们平时的使用中，可能经常使用`__init__`函数（构造函数）和`__del__`函数（析构函数），其实这也是魔法函数的一种。

# `if __name__ == '__main__'`该如何理解
`if __name__=='__main__'`就相当于Python模拟的程序入口。Python本身并没有规定这么写，这只是一种编码习惯。由于模块之间互相引用，不同模块可能都有这样的定义，而入口程序只能有一个。到底哪个入口程序被 选中，这取决于`__name__`的值。

# `__main__.py`
有如下一个包：
```python
package
├── __init__.py
└── __main__.py
```
`__init__.py`
```python
import sys
print "__init__"
print sys.path
```
`__main__.py`
```python
import sys
print "__main__"
print sys.path
```
用`python -m package`运行结果：
```python
__init__
['', ...]
__main__
['', ...]
```
用`python package`运行结果
```python
__main__
['package', ...]
```
然后我们来总结一下：
1. 加上-m参数时会把当前工作目录添加到sys.path中，而不加时则会把所在目录添加到sys.path中。
2. 加上-m参数时Python会先将模块或者包导入，然后在执行。
3. `__main__.py`文件是一个包或者目录的入口程序。不管是用`python package`还是用`python -m package`运行时，`__main__.py`文件总是被执行。