---
title: 约束
date: 2018-04-11 13:51:35
tags:
	- C#
---

|	|	|
|----------|---------------------------------------------------------|
|T:struct		|类型参数必须是值类型。可以指定除Nullable以外的任何值类型。|
|T:class		|类型参数必须是引用类型，包括任何类、接口、委托或数组类型。|
|T:new()		|类型参数必须具有无参数的公共构造函数。当与其他约束一起使用时，new()约束必须最后指定。|
|T:<接口名称>	|类型参数必须是指定的接口或实现指定的接口。可以指定多个接口约束。约束接口也可以是泛型的。|
|T:U			|为T提供的类型参数必须是为U提供的参数或派生自为U提供的参数。这称为螺类型约束。|

	public class MyGenericClass<T> where T:IComparable {}

	class MyClassy<T, U>
	where T : class
	where U : struct 
	{}

	public class MyGenericClass<T> where T: IComparable, new()
	{
		T item = new T();
	}

```
	interface MyI {}
	class Dictionary<TKey, TVal>
	where TKey: IComparable, IEnumerable
	where TVal: MyI
	{
		public void Add(Tkey key, TVal val)
		{

		}
	}
```

	public bool MyMethod<T> (T t) where T: IMyInterface {}

```
	class List<T>
	{
		void Add<U>(List<U> items) where U: T 
		{

		}
	}
```

default

之所以会用到default关键字，是因为需要在不知道类型参数为值类型还是引用类型的情况下，为对象实例赋初值。

```
class TestDefault<T>
{
	public T foo()
	{
		return default(T);
	}
}