# Python多进程

#### 1、fork（unix like系统）

```python
import os

# 只能在unix like系统上运行
pid = os.fork() 

if pid == 0:
    print("子进程：%d,%d"%(os.getpid(),os.getppid()))
else:
    print("父进程：%d"%os.getpid())
```



#### 2、multiprocessing（跨平台）

（1）简单示例

```python
from multiprocessing import Process
import os

# 子进程要执行的代码
def run_proc(name):
    print("子进程运行中, name=%s"%name)
    
if __name__ == "__main__":    
    print("父进程 %d" % os.getpid())
	p = Process(target=run_proc, args=('test',))
    print("子进程将要执行")
    p.start()
    p.join()
    print("子进程已经结束")
```

（2）Process语法结构

> Process([group [, target [, name [, args [, kwargs]]]]])
>
> target：表示这个进程实例所调用对象；
>
> args：表示调用对象的位置参数元组；
>
> kwargs：表示调用对象的关键字参数字典；
>
> name：为当前进程实例的别名；
>
> group：大多数情况下用不到；

（3）Process类常用方法

> is_alive()：判断进程实例是否还在执行；
>
> join([timeout])：是否等待进程实例执行结束，或等待多少秒；
>
> start()：启动进程实例（创建子进程）；
>
> run()：如果没有给定target参数，对这个对象调用start()方法时，就将执对象中的run()方法；
>
> terminate()：不管任务是否完成，立即终止；

（4）Process类常用属性：

> name：当前进程实例别名，默认为Process-N，N为从1开始递增的整数；
>
> pid：当前进程实例的PID值

（5）继承类

```python
from multiprocessing import Process
import time
import os

#继承Process类
class Process_Class(Process):
    def __init__(self,interval):
        Process.__init__(self)
        self.interval = interval
    #重写了Process类的run()方法
    def run(self):
		print("子进程(%d) 开始执行，父进程为（%d）"%(os.getpid(),os.getppid()))
        t_start = time.time()
		time.sleep(self.interval)
		t_stop = time.time()
		print("(%s)执行结束，耗时%0.2f秒"%(os.getpid(),t_stop-t_start))
        
if __name__=="__main__":
	t_start = time.time()
	print("当前程序进程(%s)"%os.getpid())
	p1 = Process_Class(2)
	#对不包含target属性的Process类执行start()方法，会运行这个类中的run()方法，即p1.run()
	p1.start()
	p1.join()
	t_stop = time.time()
	print("(%s)执行结束，耗时%0.2f"%(os.getpid(),t_stop-t_start))
```



#### 3、进程池

（1）简单示例

```python
from multiprocessing import Pool
import os,time,random
def worker(msg):
	t_start = time.time()
	print("%s开始执行,进程号为%d"%(msg,os.getpid()))
	#random.random()随机⽣成0~1之间的浮点数
	time.sleep(random.random()*2)
	t_stop = time.time()
	print(msg,"执行完毕，耗时%0.2f"%(t_stop-t_start))
    
if __name__=="__main__":    
    po=Pool(3) #定义一个进程池，最多进程数3
    for i in range(0,10):
        #Pool.apply_async(要调⽤的⽬标,(传递给⽬标的参数元祖,))
        #每次循环将会用空闲出来的子进程去调用目标
        po.apply_async(worker,(i,)) # 异步
        # po.apply(worker,(i,)) #　同步

        print("----start----")
        po.close() #关闭进程池，关闭后po不再接收新的请求
        po.join() #等待po中所有的进程执行完成，必须放在close语句之后
        print("-----end-----")
```



#### 4、进程间通信

##### （1）Process中使用Queue

```python
from multiprocessing import Process, Queue
import os, time, random
# 写数据进程执行的代码:
def write(q):
    for value in ['A', 'B', 'C']:
        print 'Put %s to queue...' % value
        q.put(value)
        time.sleep(random.random())
        
# 读数据进程执行的代码:        
def read(q):
     while True:
         if not q.empty():
         value = q.get(True)
         print 'Get %s from queue.' % value
         time.sleep(random.random())
     else:
         break
            
if __name__=='__main__':
    # 主进程创建Queue，并传给各个子进程：
    q = Queue()
    pw = Process(target=write, args=(q,))
    pr = Process(target=read, args=(q,))
    # 启动子进程pw，写入:
    pw.start()
    # 等待pw结束:
    pw.join()
    # 启动子进程pr，读取:
    pr.start()
    pr.join()
    # pr进程是死循环，等待其结束，只能强制终止
    print ''
    print '所有数据都写并且读完'
```

##### （2）Pool中使用Queue

```python
#修改import中的Queue为Manager
from multiprocessing import Manager,Pool
import os,time,random
def reader(q):
    print("reader启动(%s),⽗进程为(%s)"%(os.getpid(),os.getppid()))
    for i in range(q.qsize()):
    print("reader从Queue获取到消息：%s"%q.get(True))
    def writer(q):
    print("writer启动(%s),⽗进程为(%s)"%(os.getpid(),os.getppid()))
    for i in "dongGe":
    q.put(i)
    
if __name__=="__main__":
    print("(%s) start"%os.getpid())
    q=Manager().Queue() #使用Manager中的Queue来初始化
    po=Pool()
    #使用阻塞模式创建进程，这样就不需要在reader中使用死循环了，
    #可以让writer完全执⾏完成后，再用reader去读取
    po.apply(writer,(q,))
    po.apply(reader,(q,))
    po.close()
    po.join()
    print("(%s) End"%os.getpid())
```

#### 5、异步

```python
from multiprocessing import Pool
import time
import os
def test():
    print("---进程池中的进程---pid=%d,ppid=%d--"%(os.getpid(),os.getppid()))
    for i in range(3):
        print("----%d---"%i)
        time.sleep(1)
        return "hahah"
def test2(args):
    print("---callback func--pid=%d"%os.getpid())
    print("---callback func--args=%s"%args)
    
pool = Pool(3)
pool.apply_async(func=test,callback=test2)
time.sleep(5)
print("----主进程-pid=%d----"%os.getpid())
```





# Python多线程

##### 1、简单示例（主线程会等待所有的子线程结束后才结束）

```python
#coding=utf-8
import threading
from time import sleep,ctime
def sing():
	for i in range(3):
		print("正在唱歌...%d"%i)
		sleep(1)
        
def dance():
	for i in range(3):
	print("正在跳舞...%d"%i)
	sleep(1)
    
if __name__ == '__main__':
    print('---开始---:%s'%ctime())
    t1 = threading.Thread(target=sing)
    t2 = threading.Thread(target=dance)
    t1.start()
    t2.start()
    #sleep(5) # 屏蔽此代码，试试看，程序是否会提前结束？
    print('---结束---:%s'%ctime()
```

#### 2、线程类

```python
#coding=utf-8
import threading
import time
class MyThread(threading.Thread):
    def run(self):
        for i in range(3):
        time.sleep(1)
        msg = "I'm "+self.name+' @ '+str(i)
        print(msg)
    
def test():
    for i in range(5):
        t = MyThread()
        t.start()
        
if __name__ == '__main__':
	test()
```

#### 3、互斥锁

```python
from threading import Thread, Lock
import time
g_num = 0
def test1():
    global g_num
    for i in range(1000000):
        #True表示堵塞 即如果这个锁在上锁之前已经被上锁了，那么这个线程会在这⾥⼀直等待到解锁为⽌
        #False表示⾮堵塞，即不管本次调⽤能够成功上锁，都不会卡在这,⽽是继续执⾏下⾯的代码
        mutexFlag = mutex.acquire(True) #True表示堵塞
        if mutexFlag:
            g_num += 1
            mutex.release()
            print("---test1---g_num=%d"%g_num)
def test2():
    global g_num
    for i in range(1000000):
        if mutex.acquire(): #与上面等价
            g_num += 1
            mutex.release()
		print("---test2---g_num=%d"%g_num)

#创建一个互斥锁
#这个所默认是未上锁的状态
mutex = Lock()
p1 = Thread(target=test1)
p1.start()
p2 = Thread(target=test2)
p2.start()
print("---g_num=%d---"%g_num)
```

#### 4、同步锁

```python
from threading import Thread,Lock
from time import sleep
class Task1(Thread):
    def run(self):
        while True:
            if lock1.acquire():
                print("------Task 1 -----")
                sleep(0.5)
                lock2.release()
class Task2(Thread):
    def run(self):
        while True:
            if lock2.acquire():
                print("------Task 2 -----")
                sleep(0.5)
                lock3.release()
class Task3(Thread):
    def run(self):
        while True:
            if lock3.acquire():
                print("------Task 3 -----")
                sleep(0.5)
                lock1.release()
                
#使用Lock创建出的锁默认没有“锁上”
lock1 = Lock()
#创建另外一把锁，并且“锁上”

lock2 = Lock()
lock2.acquire()
#创建另外一把锁，并且“锁上”
lock3 = Lock()
lock3.acquire()
t1 = Task1()
t2 = Task2()
t3 = Task3()
t1.start()
t2.start()
t3.start()
```

#### 5、线程中传递非全局参数：ThreadLocal

> 一个ThreadLocal变量虽然是全局变量，但每个线程都只能读写各自线程的独
> 立副本，互不干扰。ThreadLocal解决了参数在每个线程中各个函数之间互相
> 传递的问题。
>
> ThreadLocal最常用的地方就是为每个线程绑定一个数据库连接，HTTP请
> 求，用户身份信息等，这样一个线程的所有调用到的处理函数都可以非常方
> 便地访问这些资源。

```python
import threading
# 创建全局ThreadLocal对象:
local_school = threading.local()
def process_student():
    # 获取当前线程关联的student:
    std = local_school.student
    print('Hello, %s (in %s)' % (std, threading.current_thread().name))
def process_thread(name):
    # 绑定ThreadLocal的student:
    local_school.student = name
    process_student()
    
t1 = threading.Thread(target= process_thread, args=('dongdong',), name="Thread-A")
t2 = threading.Thread(target= process_thread, args=('王二',), name="Thread-B")
t1.start()
t2.start()
t1.join()
t2.join()
```



# GIL问题的解决

由于Python的多线程是假的，所以当多核心CPU时，采取以下办法提升效率。

（1）把多线程的代码改为多进程，以便利用多核心的优势

（2）用C语言重写多线程的模块，使用Python调用

```python
# python调用C语言，$gcc xxx.c -shared -o libxxx.so
from ctypes import *
from threading import Thread

# 加载动态库
lib = cdll.LoadLibrary("./libdeadloop.so")
# 创建一个子进程，让其执行C语言编写的函数，此函数是一个死循环
t = Thread(target=lib.DeadLoop)
# 主线程
lib.DeadLoop

while True:
    pass
```

