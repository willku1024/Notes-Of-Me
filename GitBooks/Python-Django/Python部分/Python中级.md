# Python中级



#### 1、面向过程

##### (1)特殊函数

| 函数     | 格式                                 | 用法   |
| ------ | ---------------------------------- | ---- |
| map    | map( func, seq1[, seq2...] )       |      |
| zip    | zip(*iterater)                     |      |
| reduce | reduce( func, seq[, init] )        |      |
| filter | filter(function or None, sequence) |      |



##### (2)lambda的特殊用法

```python
def test(a,b,func)
	return func(a,b)

func_new = input("请输入一个表达式") #resume func_new: lambda x,y:x+y 
func_new = eval(func_new) #python 3
result = test(3,4,func_new)
print(result)
```



#### 2、面向对象

##### (1) 类


- 变量标识符



| 标识符       | 含义                                       |
| --------- | ---------------------------------------- |
| \_x       | (1)解释器中使用：重复上一句命令(2)临时变量，不关心取值：`for _ in range(10):print("_")`(3)约定俗成， 应该被视为API中非公开的部分(4) “from <模块/包名> import *”中不会导入，除非以模块导入“import  <模块/包名>”,访问：<模块/包名>.<属性名> |
| \_\_x     | 相当于C++的private，只能在本类中使用。（实际上可以用，只不过发生了name mangling而已，可以用\_Class\_\_x 的方式访问） |
| \_\_x\_\_ | 系统（build-in）内置变量，如：\_\_init\_\_(), \_\_name\_\_ |




- \_\_init\_\_()方法和\_\_new\_\_()方法

  - \_\_new\_\_()只负责创建对象，然后return对象
  - \_\_init\_\_() 利用 \_\_new\_\_()创建后的对象，对其初始化

- 类属性、实例属性

  ```python
  class Tool(object):
      # 类属性
      num = 0

      # 方法
      def __init__(self, new_name):
          # 实例属性
          self.name = new_name
          # 对类属性+=1
          Tool.num += 1
          
  if __name__ == "__main__":
  	tool1 = Tool("铁锹")
  	tool2 = Tool("工兵铲")
  	tool3 = Tool("水桶")
  	print(Tool.num)
  ```



- 类方法、实例方法、静态方法

  ```python
  class Game(object):

      #类属性
      num = 0

      #实例方法
      def __init__(self):
          #实例属性
          self.name = "laowang"

      #类方法
      @classmethod
      def add_num(cls):
          cls.num = 100

      #静态方法
      @staticmethod
      def print_menu():
          print("----------------------")
          print(" 穿越火线V11.1 ")
          print(" 1. 开始游戏")
          print(" 2. 结束游戏")
          print("----------------------")

  game = Game()
  # 调用类方法的方式一：可以通过类的名字调用类方法
  Game.add_num()#
  # 调用类方法的方式二：还可以通过这个类创建出来的对象 去调用这个类方法
  game.add_num()
  print(Game.num)

  #Game.print_menu()#通过类 去调用静态方法
  game.print_menu()#通过实例对象 去调用静态方法
  ```

- 子类调用父类的重写方法

  ```python
  class Animal:
      def eat(self):
          print("-----吃----")
      def drink(self):
          print("-----喝----")
      def sleep(self):
          print("-----睡觉----")
      def run(self):
          print("-----跑----")

  class Dog(Animal):
      def bark(self):
          print("----汪汪叫---")
          
  class Xiaotq(Dog):    
      def fly(self):
        print("----飞----")

  	def bark(self):
        print("----狂叫-----")
        
        #第1种调用被重写的父类的方法
        #Dog.bark(self)

        #第2种
        super().bark()
          
    xiaotq = Xiaotq()
    xiaotq.fly()
    xiaotq.bark()
    xiaotq.eat()
  ```



- python实现类似C++伪多态

  ```python
  class Dog(object):
  def print_self(self):
  print("Dog...")
  class Xiaotq(Dog):
  def print_self(self):
  print("Xiaotq...")
  def introduce(temp):
  temp.print_self()

  if __name__ == "__main__":
  dog1 = Dog()
  dog2 = Xiaotq()
  introduce(dog1)
  introduce(dog2)
  ```

  ​


##### (2)异常

- 普通异常

  ```python
    #coding=utf-8

    try:
        num = input("xxx:")
        int(num)
        #11/0
        #open("xxx.txt")
        #print(num)
        print("-----1----")
    # python2这样写：except NameError,FileNotFoundError：
    except (NameError,FileNotFoundError):
        print("如果捕获到异常后做的 处理....")
    # python2这样写：except 
    except Exception as ret:
        print("如果用了Exception,那么意味着只要上面的except没有捕获到异常,这个except一定会捕获到")
        print(ret)
    else:
        print("没有异常才会执行的功能")
    finally:
        print("------finally-----")

    print("-----2----")
  ```

- 异常类

  ```python
  class ShortInputException(Exception):
      # 用户自定义异常
      def __init__(self, length, atleast):
          self.length = length
          self.atleast = atleast

  def main():
      try:
          s = input("请输入")
          if len(s) < 3:
              # 引发自己的异常
              raise ShortInputException(len(s), 3)
      # result指向了创建的ShortInputExcception对象
      except ShortInputExcception as result:
          print("ShortInputException:输入的长度是 %d,长度至少应该是 %d",%(result.length,result.atleast))
      else:
          print("没有发生异常")
  ```

  ​

##### (3)设计模式

- 简单工厂模式：通过剥离CarStore类，解耦特定的方法

  ```python
  class CarStore(object):
      def __init__(self):
          self.factory = Factory()

      def order(self, car_type):
          return self.factory.select_car_by_type(car_type)

  class Factory(object):
      def select_car_by_type(self, car_type):
          if car_type=="索纳塔":
              return Suonata()
          elif car_type=="名图":
              return Mingtu()
          elif car_type=="ix35":
              return Ix35()

  class Car(object):
      def move(self):
          print("车在移动....")
      def music(self):
          print("正在播放音乐....")
      def stop(self):
          print("车在停止....")

  class Suonata(Car):
      pass

  class Mingtu(Car):
      pass

  class Ix35(Car):
      pass

  car_store = CarStore()
  car = car_store.order("索纳塔")
  car.move()
  car.music()
  car.stop()
  ```

- 工厂方法模式:父类只设计接口，子类通过继承负责实现

  ```python
  from abc import ABCMeta, abstractmethod
  class Store(object):
      __metaclass__ = ABCMeta
  	
      @abstractmethod
      def select_car(self):
          pass

      def order(self, car_type):
          return self.select_car(car_type)

  class BMWCarStore(Store):
      def select_car(self, car_type):
          return BMWFactory().select_car_by_type(car_type)

  class BMWFactory(object):
      def select_car_by_type(self, car_type):
          pass

  bmw_store = BMWCarStore()
  bmw = bmw_store.order("720li")
  ```

- 单例模式

  ```python
  class Dog(object):
      # 类的私有属性，用来保存初次创建的对象实例
      __instance = None
      __init_flag = False
      def __new__(cls, *more):
          if cls.__instance is None:
              cls.__instance = object.__new__(cls)
              return cls.__instance
          else:
              # return 上一次初次创建对象的引用
              return cls.__instance

      def __init__(self, name):
          if Dog.__init_flag is False:
              self.name = name
              Dog.__init_flag = True
              
   if "main" == name:
      a = Dog("旺财")
      b = Dog("哮天犬")
      print(id(a))
      print(a.name)
      print(id(b), b.name)
  ```



#### 3、包和模块

- \_\_all__ 在模块中的使用

  ```python
  # sendmsg.py
  # test2不可被其他模块使用
  __all__ = ['test1','num','Test']

  def test1():
      print('-------test1---------')

  def test2():
      print('-------test2---------')
      num = 100

  class Test(object)
      pass
  ```


- \_\_init__.py在包中的使用

  ```python
  __all__ = ['sendmsg']

  from . import sendmsg
  ```

- python模块的发布

  - 在要发布包（一个目录）的同级目录建立setup.py文件

  - 编辑setup.py文件,py_modules指明包中的模块

    ```python
    from distutils.core import setup

    setup(name="willxin", 
          version="1.0", 
          description="willxin's module",
          author="willxin",
          py_modules=['TestMsg.sendmsg', 'TestMsg.recvmsg'])
    ```

    ​

  - 构建bulid目录：$ python setup.py  build

  - 创建tar包：$ python setup.py  sdist

  - 他人安装包：python setup.py install





#### 4、命名空间

就像Python之禅中的最后一句所说，Python对命名空间（NameSpace）频繁使用

| 命名空间 | 本质       | 备注                            |
| ---- | -------- | ----------------------------- |
| 包    | 文件夹      | 包含许多.py文件，可用\_\_init\_\_.py定制 |
| 模块   | .py文件    | 包含各种属性和方法，可用\_\_all\_\_定制     |
| 类    | Class    | 定义了各种属性和方法                    |
| 函数   | Method   | 包含各种属性和[内嵌]()[方法]()           |
| 变量   | 对应的实例化对象 | 包含各种属性和方法                     |

#### 5、生成器

 （1）创建的方式

- b = (x*2 for x in range(10))
- yield

```python
def createNum():
    a, b =  0, 1
    for i in range(5):
        yield b
        a,b = b, a+b
       
result = createNum()
next(result)
```

- send()

```python
def createNum():
    a, b =  0, 1
    for i in range(5):
        if i == 0:
        	temp = yield b
            print(temp)
        else:
            yield b
        a,b = b, a+b
        
       
result = createNum()
result.send("haha")  # 等价 next(result)，并传参数给temp
```

- yield模拟多任务（协程）

```python
def task_1():
    while True:
        print("--1--")
        yield None
        
def task_2():
    while True:
        print("--2--")
        yield None
        
t1 = task_1()
t2 = task_2()
while True:
    t1.__next__()
    t2.__next__()
```



#### 6、迭代器

- 凡是可作用于for循环的对象都是Iterable类型
- 凡是可作用于next()函数的对象都是Iterable类型
- 集合数据类型如set、list、tuple、dict等都是Iterable，但不是Iterator。可以通过iter()函数生成为Iterator。

（1）可以迭代（Iterable）：

一类是list，tuple，dict，set，“abcd’”

一类是generate function或yield

（2）判断是否可以迭代

```python
>>> from collections import Iterator
>>> isinstance([11,22,33], Iterator)
False
>>> isinstance([i for i in range(10)], Iterator)
True
```

（3）iter()函数

```python
>>> from collections import Iterator
>>> isinstance(iter([11,22,33]), Iterator)
True

# example
>>> b = [11,22,33]
>>> c = iter(b)
>>> next(c)
11
```


#### 