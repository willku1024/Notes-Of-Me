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

（1）Process中使用Queue

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

（2）Pool中使用Queue

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

