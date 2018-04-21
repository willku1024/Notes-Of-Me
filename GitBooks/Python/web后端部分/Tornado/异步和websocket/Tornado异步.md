# Tornado异步

因为epoll主要是用来解决网络IO的并发问题，所以Tornado的异步编程也主要体现在网络IO的异步上，即异步Web请求。

## 1. tornado.httpclient.AsyncHTTPClient

Tornado提供了一个异步Web请求客户端tornado.httpclient.AsyncHTTPClient用来进行异步Web请求。

### fetch(request, callback=None)

用于执行一个web请求request，并异步返回一个tornado.httpclient.HTTPResponse响应。

request可以是一个url，也可以是一个tornado.httpclient.HTTPRequest对象。如果是url，fetch会自己构造一个HTTPRequest对象。

### HTTPRequest

HTTP请求类，HTTPRequest的构造函数可以接收众多构造参数，最常用的如下：

- **url** (string) – 要访问的url，此参数必传，除此之外均为可选参数
- **method** (string) – HTTP访问方式，如“GET”或“POST”，默认为GET方式
- **headers** (HTTPHeaders or dict) – 附加的HTTP协议头
- **body** – HTTP请求的请求体

### HTTPResponse

HTTP响应类，其常用属性如下：

- **code**: HTTP状态码，如 200 或 404
- **reason**: 状态码描述信息
- **body**: 响应体字符串
- **error**: 异常（可有可无）

## 2. 测试接口

新浪IP地址库

接口说明

1.请求接口（GET）：

<http://int.dpool.sina.com.cn/iplookup/iplookup.php?format=json&ip=[ip地址字串>]

2.响应信息：

（json格式的）国家 、省（自治区或直辖市）、市（县）、运营商

3.返回数据格式：

```
{"ret":1,"start":-1,"end":-1,"country":"\u4e2d\u56fd","province":"\u5317\u4eac","city":"\u5317\u4eac","district":"","isp":"","type":"","desc":""}
```

## 3. 回调异步

```
class IndexHandler(tornado.web.RequestHandler):
    @tornado.web.asynchronous  # 不关闭连接，也不发送响应
    def get(self):
        http = tornado.httpclient.AsyncHTTPClient()
        http.fetch("http://int.dpool.sina.com.cn/iplookup/iplookup.php?format=json&ip=14.130.112.24",
                   callback=self.on_response)

    def on_response(self, response):
        if response.error:
            self.send_error(500)
        else:
            data = json.loads(response.body)
            if 1 == data["ret"]:
                self.write(u"国家：%s 省份: %s 城市: %s" % (data["country"], data["province"], data["city"]))
            else:
                self.write("查询IP信息错误")
        self.finish() # 发送响应信息，结束请求处理
```

### tornado.web.asynchronous

此装饰器用于回调形式的异步方法，并且应该仅用于HTTP的方法上（如get、post等）。

此装饰器不会让被装饰的方法变为异步，而只是告诉框架被装饰的方法是异步的，当方法返回时响应尚未完成。只有在request handler调用了finish方法后，才会结束本次请求处理，发送响应。

不带此装饰器的请求在get、post等方法返回时自动完成结束请求处理。

## 4. 协程异步

在上一节中我们自己封装的装饰器get_coroutine在Tornado中对应的是tornado.gen.coroutine。

```
class IndexHandler(tornado.web.RequestHandler):
    @tornado.gen.coroutine
    def get(self):
        http = tornado.httpclient.AsyncHTTPClient()
        response = yield http.fetch("http://int.dpool.sina.com.cn/iplookup/iplookup.php?format=json&ip=14.130.112.24")
        if response.error:
            self.send_error(500)
        else:
            data = json.loads(response.body)
            if 1 == data["ret"]:
                self.write(u"国家：%s 省份: %s 城市: %s" % (data["country"], data["province"], data["city"]))
            else:
                self.write("查询IP信息错误")
```

也可以将异步Web请求单独出来：

```
class IndexHandler(tornado.web.RequestHandler):
    @tornado.gen.coroutine
    def get(self):
        rep = yield self.get_ip_info("14.130.112.24")
        if 1 == rep["ret"]:
            self.write(u"国家：%s 省份: %s 城市: %s" % (rep["country"], rep["province"], rep["city"]))
        else:
            self.write("查询IP信息错误")

    @tornado.gen.coroutine
    def get_ip_info(self, ip):
        http = tornado.httpclient.AsyncHTTPClient()
        response = yield http.fetch("http://int.dpool.sina.com.cn/iplookup/iplookup.php?format=json&ip=" + ip)
        if response.error:
            rep = {"ret:0"}
        else:
            rep = json.loads(response.body)
        raise tornado.gen.Return(rep)  # 此处需要注意
```

**代码中我们需要注意的地方是get_ip_info返回值的方式，在python 2中，使用了yield的生成器可以使用不返回任何值的return，但不能return value，因此Tornado为我们封装了用于在生成器中返回值的特殊异常tornado.gen.Return，并用raise来返回此返回值。**

### 并行协程

Tornado可以同时执行多个异步，并发的异步可以使用列表或字典，如下：

```
class IndexHandler(tornado.web.RequestHandler):
    @tornado.gen.coroutine
    def get(self):
        ips = ["14.130.112.24",
            "15.130.112.24",
            "16.130.112.24",
            "17.130.112.24"]
        rep1, rep2 = yield [self.get_ip_info(ips[0]), self.get_ip_info(ips[1])]
        rep34_dict = yield dict(rep3=self.get_ip_info(ips[2]), rep4=self.get_ip_info(ips[3]))
        self.write_response(ips[0], rep1) 
        self.write_response(ips[1], rep2) 
        self.write_response(ips[2], rep34_dict['rep3']) 
        self.write_response(ips[3], rep34_dict['rep4']) 

    def write_response(self, ip, response):
        self.write(ip) 
        self.write(":<br/>") 
        if 1 == response["ret"]:
            self.write(u"国家：%s 省份: %s 城市: %s<br/>" % (response["country"], response["province"], response["city"]))
        else:
            self.write("查询IP信息错误<br/>")

    @tornado.gen.coroutine
    def get_ip_info(self, ip):
        http = tornado.httpclient.AsyncHTTPClient()
        response = yield http.fetch("http://int.dpool.sina.com.cn/iplookup/iplookup.php?format=json&ip=" + ip)
        if response.error:
            rep = {"ret:1"}
        else:
            rep = json.loads(response.body)
        raise tornado.gen.Return(rep)
```

## 5. 关于数据库的异步说明

网站基本都会有数据库操作，而Tornado是单线程的，这意味着如果数据库查询返回过慢，整个服务器响应会被堵塞。

数据库查询，实质上也是远程的网络调用；理想情况下，是将这些操作也封装成为异步的；但Tornado对此并**没有**提供任何支持。

这是Tornado的**设计**，而不是缺陷。

一个系统，要满足高流量；是必须解决数据库查询速度问题的！

数据库若存在查询性能问题，整个系统无论如何优化，数据库都会是瓶颈，拖慢整个系统！

异步并**不能**从本质上提到系统的性能；它仅仅是避免多余的网络响应等待，以及切换线程的CPU耗费。

如果数据库查询响应太慢，需要解决的是数据库的性能问题；而不是调用数据库的前端Web应用。

对于实时返回的数据查询，理想情况下需要确保所有数据都在内存中，数据库硬盘IO应该为0；这样的查询才能足够快；而如果数据库查询足够快，那么前端web应用也就无将数据查询封装为异步的必要。

就算是使用协程，异步程序对于同步程序始终还是会提高复杂性；需要衡量的是处理这些额外复杂性是否值得。

**如果后端有查询实在是太慢，无法绕过，Tornaod的建议是将这些查询在后端封装独立封装成为HTTP接口，然后使用Tornado内置的异步HTTP客户端进行调用。**