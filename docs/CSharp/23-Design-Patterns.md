---
title: 23种设计模式
date: 2018-04-11 13:37:36
tags:
  - C#
---
设计模式分为三大类：
创建型模式，共五种：工厂方法模式、抽象工厂模式、单例模式、建造者模式、原型模式。
结构性模式，共七种：适配器模式、装饰器模式、代理模式、外观模式、桥接模式、组合模式、享元模式。
行为型模式，共十一种：策略模式、模板方法模式、观察者模式、迭代子模式、责任链模式、命令模式、备忘录模式、状态模式、访问者模式、中介模式、解释器模式。
其实还有两类：并发型模式和线程池模式。

设计模式六大原则：
总原则-开闭原则
对扩展开放，对修改封闭。在程序需要进行扩展的时候，不能去修改原有的代码，而是扩展原有代码，实现一个热插拔的效果。所以一句话概括就是：为了使程序的扩展性好，易于维护和升级。
想要达到这样的效果，外面需要使用接口和抽象类等，后面的具体设计中我们会提到这点。

1、单一职责原则
不要存在多余一个导致类变更的原因，也就是说每个类应该实现单一的职责，否则就应该把类拆分。

2.里氏替换原则(Liskov Substitution Principle)
任何积累可以出现的地方，子类一定可以出现

6.合成复用原则(Composite Reuse Principle)
尽量首先使用合成/聚合的方式，而不是使用继承。

# 单线程单例模式

```
    public sealed class Singleton // 密封防止继承
    {
        private static Singleton uniqueInstance; // 唯一的实例

        // 私有构造函数外部无法创建实例
        private Singleton()
        {

        }
        // 全局唯一访问点
        public static Singleton GetInstance()
        {
            if (uniqueInstance == null)
            {
                uniqueInstance = new Singleton();
            }
            return uniqueInstance;
        }
    }
```

单线程单例模式的几个要点：

1. Singleton模式的实例构造器可以设置为protected以允许子类派生。
2. Singleton模式一般不要ICloneable接口，因为这可能会导致多个对象实例，与Singleton模式的初衷违背。
3. Singleton模式一般不要支持序列化，因为这也有可能导致多个对象实例，同样与Singleton模式的初衷违背。
4. Singleton模式只考虑到了对象创建的工作，没有考虑到对象效果的工作。全局静态的放到托管堆中还可以接受？
5. 不能应对多线程环境，在多线程环境下，使用Singleton模式仍然有可能得到Singleton类的多个实例对象。

# 工厂设计模式

## 简单工厂设计模式

```
public enum FruitKind
{
    Apple,
    Pear,
}
public interface Fruit
{
    void WhatIm();
}
public class Apple : Fruit
{
    public void WhatIm()
    {
        Console.WriteLine("I am Apple");
    }
}
public class Pear : Fruit
{
    public void WhatIm()
    {
        Console.WriteLine("I am pear");
    }
}
public class FruitFactory
{
    public static Fruit CreateFruit(FruitKind type)
    {
        switch(type)
        {
            case FruitKind.Apple:
                return new Apple();
            case FruitKind.Pear:
                return new Pear();
        }
        return null;
    }
}
class Program
{
    static void Main(string[] args)
    {
        Fruit a = FruitFactory.CreateFruit(FruitKind.Pear);
        a.WhatIm();
    }
}
```
不太喜欢损失性能和不容易看到内部的逻辑，所以上述不太喜欢。上述存在一个问题，产品内部随便折腾没啥问题，但是做一个新的产品就蛋疼了。

## 工厂方法设计模式
1. 抽象产品类
2. 具体产品类
3. 抽象工厂类
4. 具体工厂类
```
public enum FruitKind
{
    Apple,
    Pear,
}
public interface Fruit
{
    void WhatIm();
}
public class Apple : Fruit
{
    public void WhatIm()
    {
        Console.WriteLine("I am Apple");
    }
}
public class Pear : Fruit
{
    public void WhatIm()
    {
        Console.WriteLine("I am pear");
    }
}
public interface FruitFactory
{
    Fruit CreateFruit();
}
public class PearFactory : FruitFactory
{
    public Fruit CreateFruit()
    {
        return new Apple();
    }
}
```

上述代码解耦了，
