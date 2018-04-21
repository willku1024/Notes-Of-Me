# Python网络编程

#### 1、UDP

##### （1）发送，接收

```python
#coding=utf-8
from socket import *
#1. 创建套接字
udpSocket = socket(AF_INET, SOCK_DGRAM)
#2. 准备接收的地址
sendAddr = ('192.168.1.103', 8080)
#3. 从键盘获取数据
sendData = b"hello,world" # 或 sendData = "hello,world".encode("utf-8")
#4. 发送数据到指定的电脑上
udpSocket.sendto(sendData, sendAddr)
#5. 等待接收对方发送的数据
recvData = udpSocket.recvfrom(1024) # 1024表示本次接收的最大字节数
#6. 显示对方发送的数据
print(recvData)
#7. 关闭套接字
udpSocket.close()

```

##### （2）绑定端口（server端）

```python
#coding=utf-8
from socket import *
#1. 创建套接字
udpSocket = socket(AF_INET, SOCK_DGRAM)
#2. 绑定本地的相关信息，如果一个网络程序不绑定，则系统会随机分配
bindAddr = ('', 7788) # ip地址和端口号，ip一般不写，表示本机的任何一个ip
udpSocket.bind(bindAddr)
#3. 等待接收对方发送的数据
recvData = udpSocket.recvfrom(1024) # 1024表示本次接收的最大字节数
#4. 显示接收到的数据
print recvData
#5. 关闭套接字
udpSocket.close()
```

##### （3）广播

```python
#coding=utf-8
import socket, sys
# 自动推断所在的广播段
dest = ('<broadcast>', 7788)
# 创建udp套接字
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# 对这个需要发送广播数据的套接字进行修改设置，否则不能发送发播数据
s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST,1)
# 以广播的形式发送数据到本网络的所有电脑中
s.sendto("Hi", dest)
print "等待对方回复（按ctrl+c退出）"
while True:
    (buf, address) = s.recvfrom(2048)
    print "Received from %s: %s" % (address, buf)
```



#### 2、pack和unpack

![pack和unpack](../images/pack和unpack.png)

#### 3、TCP

##### （1）发送、接收

```python
#coding=utf-8
from socket import *
# 创建socket
tcpSerSocket = socket(AF_INET, SOCK_STREAM)
# 绑定本地信息
address = ('', 7788)
tcpSerSocket.bind(address)
# 使用socket创建的套接字默认的属性是主动的，使⽤listen将其变为被动的，这样就可以接收别⼈的链接了
tcpSerSocket.listen(5)
# 如果有新的客户端来链接服务器，那么就产生一个新的套接字专门为这个客户端服务器
# newSocket用来为这个客户端服务
# tcpSerSocket就可以省下来专门等待其他新客户端的链接
newSocket, clientAddr = tcpSerSocket.accept()
# 接收对方发送过来的数据，最大接收1024个字节
recvData = newSocket.recv(1024)
print '接收到的数据为:',recvData
# 发送一些数据到客户端
newSocket.send("thank you !")
# 关闭为这个客户端服务的套接字，只要关闭了，就意味着为不能再为这个客户端服务了，如果还需要服务，只能再次重新连接
newSocket.close()
# 关闭监听套接字，只要这个套接字关闭了，就意味着整个程序不能再接收任何新的客户端的连接
tcpSerSocket.close()
```

##### （2）客户端

```python
#coding=utf-8
from socket import *
# 创建socket
tcpClientSocket = socket(AF_INET, SOCK_STREAM)
# 链接服务器
serAddr = ('192.168.1.102', 7788)
tcpClientSocket.connect(serAddr)
while True:
    # 提示用户输入数据
    sendData = raw_input("send：")
    if len(sendData)>0:
        tcpClientSocket.send(sendData)
        else:
            break
            # 接收对方发送过来的数据，最大接收1024个字节
            recvData = tcpClientSocket.recv(1024)
            print 'recv:',recvData
            # 关闭套接字
            tcpClientSocket.close()
```

##### （3）服务端

```python
#coding=utf-8
from socket import *
# 创建socket
tcpSerSocket = socket(AF_INET, SOCK_STREAM)
# 绑定本地信息
address = ('', 7788)
tcpSerSocket.bind(address)
# socket创建的套接字默认的属性是主动的，使用listen将其变为被动的，这样就可以接收别的链接了
tcpSerSocket.listen(5)
while True:
    # 如果有新的客户端来链接服务器，那么就产⽣⼀个信⼼的套接字专⻔为这个客户端服务器
    # newSocket⽤来为这个客户端服务
    # tcpSerSocket就可以省下来专⻔等待其他新客户端的链接
    newSocket, clientAddr = tcpSerSocket.accept()
    while True:
        # 接收对方发送过来的数据，最⼤接收1024个字节
        recvData = newSocket.recv(1024)
        # 如果接收的数据的长度为0，则意味着客户端关闭了链接
        if len(recvData)>0:
            print 'recv:',recvData
        else:
            break
            # 发送一些数据到客户端
            sendData = raw_input("send:")
            newSocket.send(sendData)
            # 关闭为这个客户端服务的套接字，意味着为不再为这个客户端服务，
            # 如果还需要服务，只能再次重新连接
            newSocket.close()

    # 关闭监听套接字，就意味着整个程序不能再接收任何新的客户端的连接
    tcpSerSocket.close()
```



#### 4、服务器端编程

##### （1）多进程版

```python
from socket import *
from multiprocessing import *
from time import sleep


# 处理客户端的请求并为其服务
def dealWithClient(newSocket, destAddr):
    while True:
        recvData = newSocket.recv(1024)
        if len(recvData) > 0:
            print('recv[%s]:%s' % (str(destAddr), recvData))
        else:
            print('[%s]客户端已经关闭' % str(destAddr))
            break
    newSocket.close()


def main():
    serSocket = socket(AF_INET, SOCK_STREAM)
    serSocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    localAddr = ('', 7788)
    serSocket.bind(localAddr)
    serSocket.listen(5)
    try:
        while True:
            print('-----主进程，等待新客户端的到来------')
            newSocket, destAddr = serSocket.accept()
            print('-----主进程，接下来创建一个新的进程负责数据处理[%s]-----'
            client = Process(target=dealWithClient, args=(newSocket, destAddr))
            client.start()
            # 因为已经向进进程中copy了一份（引用），并且父进程中这个套接字也没有用处了
            # 所以关闭
            newSocket.close()
    finally:
        # 当为所有的客户端服务完之后再关闭，表示不再接收新的客户端的链接
        serSocket.close()
        
if __name__ == '__main__':
    main()
```

##### （2）多线程版

```python
# coding=utf-8
from socket import *
from threading import Thread
from time import sleep


# 处理客户端的请求并执⾏事情
def dealWithClient(newSocket, destAddr):
    while True:
        recvData = newSocket.recv(1024)
        if len(recvData) > 0:
            print('recv[%s]:%s' % (str(destAddr), recvData))
        else:
            print('[%s]客户端已经关闭' % str(destAddr))
            break
    newSocket.close()


def main():

    serSocket = socket(AF_INET, SOCK_STREAM)
    serSocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    localAddr = ('', 7788)
    serSocket.bind(localAddr)
    serSocket.listen(5)
    try:
        while True:
            print('-----主进程，等待新客户端的到来------')
            newSocket, destAddr = serSocket.accept()
            print('-----主进程，接下来创建一个新的进程负责数据处理[%s]-----'

            client = Thread(target=dealWithClient, args=(newSocket, destAddr))
            client.start()
    # 因为线程中共享这个套接字，如果关闭了会导致这个套接字不可用，
    # 但是此时在线程中这个套接字可能还在收数据，因此不能关闭
    # newSocket.close()
    finally:
        serSocket.close()

if __name__ == '__main__':
    main()
```

##### （3）单进程无阻塞版

```python
# coding=utf-8
from socket import *
import time

# 用来存储所有的新链接的socket
g_socketList = []

def main():
    serSocket = socket(AF_INET, SOCK_STREAM)
    serSocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    localAddr = ('', 7788)
    serSocket.bind(localAddr)
    # 可以适当修改listen中的值来看看不同的现象
    serSocket.listen(1000)
    # 将套接字设置为非堵塞
    # 设置为非堵塞后，如果accept时，恰巧没有客户端connect，那么accept会
    # 产生一个异常，所以需要try来进⾏处理
    serSocket.setblocking(False)
    while True:
        # 用来测试
        # time.sleep(0.5)
        try:
            newClientInfo = serSocket.accept()
        except Exception as result:
            pass
        else:
            print("⼀个新的客户端到来:%s" % str(newClientInfo))
            newClientInfo[0].setblocking(False)

            g_socketList.append(newClientInfo)
            # 用来存储需要删除的客户端信息
            needDelClientInfoList = []
            for clientSocket, clientAddr in g_socketList:
                try:
                    recvData = clientSocket.recv(1024)
                    if len(recvData) > 0:
                        print('recv[%s]:%s' % (str(clientAddr), recvData))
                    else:
                        print('[%s]客户端已经关闭' % str(clientAddr))
                        clientSocket.close()
                        needDelClientInfoList.append((clientSocket, clientAddr))
                except Exception as result:
                    pass
            
            for needDelClientInfo in needDelClientInfoList:
                g_socketList.remove(needDelClientInfo)
                
if __name__ == '__main__':
    main()
```

##### （4）单进程服务器 -- select：回显服务器

```python
import select
import socket
import sys

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(('', 7788))
server.listen(5)

inputs = [server, sys.stdin]
running = True
while True:
    # 调 select 函数，阻塞等待
    readable, writeable, exceptional = select.select(inputs, [], [])
    # 数据抵达，循环
    for sock in readable:

        # 监听到有新的连接
        if sock == server:
            conn, addr = server.accept()
            # select 监听的socket
            inputs.append(conn)

        # 监听到键盘有输入
        elif sock == sys.stdin:
            cmd = sys.stdin.readline()
            running = False
            break

        # 监听到有数据到达
        else:
        # 读取客户端连接发送的数据
            data = sock.recv(1024)
            if data:
                sock.send(data)
            else:
                # 移除select监听的socket
                inputs.remove(sock)
                sock.close()

    # 如果检测到用户敲击键盘输入，那么就退出
    if not running:
        break

server.close()
```

##### （5）单进程服务器 -- select：包含writeList

```python
#coding=utf-8
import socket
import Queue
from select import select

SERVER_IP = ('', 9999)
# 保存客户端发送过来的消息,将消息放⼊队列中
message_queue = {}
input_list = []
output_list = []

if __name__ == "__main__":
    server = socket.socket()
    server.bind(SERVER_IP)
    server.listen(10)
    # 设置为非阻塞
    server.setblocking(False)
    # 初始化将服务端加入监听列表
    input_list.append(server)
    while True:
        # 开始 select 监听,对input_list中的服务端server进行监听
        stdinput, stdoutput, stderr = select(input_list, output_list, input_list)
        # 循环判断是否有客户端连接进来,当有客户端连接进来时select将触发
        for obj in stdinput:
            # 判断当前触发的是不是服务端对象, 当触发的对象是服务端对象时,说明有新客户端连接进来了
            if obj == server:
                # 接收客户端的连接, 获取客户端对象和客户端地址信息
                conn, addr = server.accept()
                print("Client %s connected! "%str(addr))
                # 将客户端对象也加⼊到监听的列表中, 当客户端发送消息时 select 将触发
                input_list.append(conn)
                # 为连接的客户端单独创建⼀个消息队列，⽤来保存客户端发送的消息
                message_queue[conn] = Queue.Queue()
            else:
                # 由于客户端连接进来时服务端接收客户端连接请求，将客户端加⼊到了监听列表中(input_list)，客户端发送消息将触发
                # 所以判断是否是客户端对象触发
                try:

                    recv_data = obj.recv(1024)
                    # 客户端未断开
                    if recv_data:
                        print("received %s from client %s"%(recv_data, str(addr)))
                        # 将收到的消息放⼊到各客户端的消息队列中
                        message_queue[obj].put(recv_data)

                        # 将回复操作放到output列表中，让select监听
                        if obj not in output_list:
                            output_list.append(obj)
                except ConnectionResetError:
                    # 客户端断开连接了，将客户端的监听从input列表中移除
                    input_list.remove(obj)
                    # 移除客户端对象的消息队列
                    del message_queue[obj]
                    print("\n[input] Client %s disconnected"%str(addr))

        # 如果现在没有客户端请求,也没有客户端发送消息时，开始对发送消息列表进⾏处理，是否需要发送消息
        for sendobj in output_list:
            try:
                # 如果消息队列中有消息,从消息队列中获取要发送的消息
                if not message_queue[sendobj].empty():
                    # 从该客户端对象的消息队列中获取要发送的消息
                    send_data = message_queue[sendobj].get()
                    sendobj.send(send_data)
                else:
                    # 将监听移除等待下⼀次客户端发送消息
                    output_list.remove(sendobj)
                    
            except ConnectionResetError:
                # 客户端连接断开了
                del message_queue[sendobj]
                output_list.remove(sendobj)
                print("\n[output] Client %s disconnected"%str(addr))
```

##### （6）单进程服务器 -- epoll

说明：

- EPOLLIN （可读）
- EPOLLOUT（可写）
- EPOLLET（ET模式）

> epoll对文件描述符的操作有两种模式：LT(level trigger)和ET (edge trigger)
>
> LT模式是默认模式，LT模式与ET模式的区别如下：
>
> LT模式：当epoll检测到描述符事件发生并将此事件通知应用程序，应用程序可以不立即处理该事件。下次调用epoll时，会再次响应应用程序并通知此事件。
>
> ET模式：当epoll检测到描述符事件发生并将此事件通知应用程序，应用程序必须立即处理该事件。如果不处理，下次调用epoll时，不会再次响应应用程序并通知此事件。

```python
import socket
import select
# 创建套接字
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
# 设置可以重复使⽤绑定的信息
s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
# 绑定本机信息
s.bind(("",7788))
# 变为被动
s.listen(10)
# 创建一个epoll对象
epoll=select.epoll()

# 测试，用来打印套接字对应的描述符
# print s.fileno()
# print select.EPOLLIN|select.EPOLLET

# 注册事件到epoll中
# epoll.register(fd[, eventmask])
# 注意，如果fd已经注册过，则会发⽣异常
# 将创建的套接字添加到epoll的事件监听中
epoll.register(s.fileno(),select.EPOLLIN|select.EPOLLET)

connections = {}
addresses = {}
# 循环等待客户端的到来或者对⽅发送数据
while True:
    # epoll 进行 fd 扫描的地方 -- 未指定超时时间则为阻塞等待
    epoll_list = epoll.poll()
    # 对事件进⾏判断
    for fd,events in epoll_list:
        # print fd
        # print events
        # 如果是socket创建的套接字被激活
        if fd == s.fileno():
            conn,addr = s.accept()
            print('有新的客户端到来%s'%str(addr))
            
            # 将 conn 和 addr 信息分别保存起来
            connections[conn.fileno()] = conn
            addresses[conn.fileno()] = addr
            
            # 向 epoll 中注册 连接 socket 的 可读 事件
            epoll.register(conn.fileno(), select.EPOLLIN | select.EPOLLET)
        elif events == select.EPOLLIN:
            # 从激活 fd 上接收
            recvData = connections[fd].recv(1024)
            if len(recvData) > 0:
                print('recv:%s' % recvData)
            else:
                # 从 epoll 中移除该 连接 fd
                epoll.unregister(fd)
                # server 侧主动关闭该 连接 fd
                connections[fd].close()
                print("%s---offline---" % str(addresses[fd]))
```



#### 5、协程

##### （1）简单示例

```python
import time
def A():
	while True:
		print("----A---")
		yield
		time.sleep(0.5)
def B(c):
	while True:
		print("----B---")
		c.next()
		time.sleep(0.5)
        
if __name__=='__main__':
	a = A()
	B(a)
```

##### （2）gevent的使用

```python
#coding=utf-8
#请使用python 2 来执行此程序
import gevent
def f(n):
	for i in range(n):
		print gevent.getcurrent(), i
    
g1 = gevent.spawn(f, 5)
g2 = gevent.spawn(f, 5)
g3 = gevent.spawn(f, 5)
g1.join()
g2.join()
g3.join()
```

##### （3）gevent 模拟IO操作，发生异步

```python
import gevent
def f(n):
	for i in range(n):
		print gevent.getcurrent(), i
		#用来模拟一个耗时操作，注意不是time模块中的sleep
		gevent.sleep(1)
        
g1 = gevent.spawn(f, 5)
g2 = gevent.spawn(f, 5)
g3 = gevent.spawn(f, 5)
g1.join()
g2.join()
g3.join()
```

##### （4）gevent 并发下载

> 当然，实际代码中，我们不会调用gevent.sleep()去切换协程，而是在执行到IO操作时，gevent自动切换。

```python
#coding=utf-8
from gevent import monkey;
import gevent
import urllib2
#有IO才做时需要这句
monkey.patch_all()
def myDownLoad(url):
	print('GET: %s' % url)
	resp = urllib2.urlopen(url)
	data = resp.read()
	print('%d bytes received from %s.' % (len(data), url))
    
gevent.joinall([
	gevent.spawn(myDownLoad, 'http://www.baidu.com/'),
	gevent.spawn(myDownLoad, 'http://www.itcast.cn/'),
	gevent.spawn(myDownLoad, 'http://www.itheima.com/'),
])
```

##### （5）gevent - Tcp服务器

```python
import sys
import time
import gevent
from gevent import socket,monkey
monkey.patch_all()
def handle_request(conn):
	while True:
		data = conn.recv(1024)
		if not data:
			conn.close()
			break
		print("recv:", data)
		conn.send(data)
        
def server(port):
	s = socket.socket()
	s.bind(('', port))
	s.listen(5)
	while True:
		cli, addr = s.accept()
		gevent.spawn(handle_request, cli)
        
if __name__ == '__main__':
	server(7788)
```

