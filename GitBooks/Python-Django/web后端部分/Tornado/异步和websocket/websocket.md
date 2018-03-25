# WebSocket

WebSocket是HTML5规范中新提出的客户端-服务器通讯协议，协议本身使用新的ws://URL格式。

WebSocket 是独立的、创建在 TCP 上的协议，和 HTTP 的唯一关联是使用 HTTP 协议的101状态码进行协议切换，使用的 TCP 端口是80，可以用于绕过大多数防火墙的限制。

WebSocket 使得客户端和服务器之间的数据交换变得更加简单，允许服务端直接向客户端推送数据而不需要客户端进行请求，两者之间可以创建持久性的连接，并允许数据进行双向传送。

目前常见的浏览器如 Chrome、IE、Firefox、Safari、Opera 等都支持 WebSocket，同时需要服务端程序支持 WebSocket。

## 1. Tornado的WebSocket模块

Tornado提供支持WebSocket的模块是tornado.websocket，其中提供了一个WebSocketHandler类用来处理通讯。

#### WebSocketHandler.open()

当一个WebSocket连接建立后被调用。

#### WebSocketHandler.on_message(message)

当客户端发送消息message过来时被调用，**注意此方法必须被重写**。

#### WebSocketHandler.on_close()

当WebSocket连接关闭后被调用。

#### WebSocketHandler.write_message(message, binary=False)

向客户端发送消息messagea，message可以是字符串或字典（字典会被转为json字符串）。若binary为False，则message以utf8编码发送；二进制模式（binary=True）时，可发送任何字节码。

#### WebSocketHandler.close()

关闭WebSocket连接。

#### WebSocketHandler.check_origin(origin)

判断源origin，对于符合条件（返回判断结果为True）的请求源origin允许其连接，否则返回403。可以重写此方法来解决WebSocket的跨域请求（如始终return True）。

## 2. 前端JavaScript编写

在前端JS中使用WebSocket与服务器通讯的常用方法如下：

```
var ws = new WebSocket("ws://127.0.0.1:8888/websocket"); // 新建一个ws连接
ws.onopen = function() {  // 连接建立好后的回调
   ws.send("Hello, world");  // 向建立的连接发送消息
};
ws.onmessage = function (evt) {  // 收到服务器发送的消息后执行的回调
   alert(evt.data);  // 接收的消息内容在事件参数evt的data属性中
};
```

## 3. 在线聊天室的小Demo

#### 后端代码 server.py

```
# coding:utf-8

import tornado.web
import tornado.ioloop
import tornado.httpserver
import tornado.options
import os
import datetime

from tornado.web import RequestHandler
from tornado.options import define, options
from tornado.websocket import WebSocketHandler

define("port", default=8000, type=int)

class IndexHandler(RequestHandler):
    def get(self):
        self.render("index.html")

class ChatHandler(WebSocketHandler):

    users = set()  # 用来存放在线用户的容器

    def open(self):
        self.users.add(self)  # 建立连接后添加用户到容器中
        for u in self.users:  # 向已在线用户发送消息
            u.write_message(u"[%s]-[%s]-进入聊天室" % (self.request.remote_ip, datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")))

    def on_message(self, message):
        for u in self.users:  # 向在线用户广播消息
            u.write_message(u"[%s]-[%s]-说：%s" % (self.request.remote_ip, datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"), message))

    def on_close(self):
        self.users.remove(self) # 用户关闭连接后从容器中移除用户
        for u in self.users:
            u.write_message(u"[%s]-[%s]-离开聊天室" % (self.request.remote_ip, datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")))

    def check_origin(self, origin):
        return True  # 允许WebSocket的跨域请求

if __name__ == '__main__':
    tornado.options.parse_command_line()
    app = tornado.web.Application([
            (r"/", IndexHandler),
            (r"/chat", ChatHandler),
        ],
        static_path = os.path.join(os.path.dirname(__file__), "static"),
        template_path = os.path.join(os.path.dirname(__file__), "template"),
        debug = True
        )
    http_server = tornado.httpserver.HTTPServer(app)
    http_server.listen(options.port)
    tornado.ioloop.IOLoop.current().start()
```

#### 前端代码index.html

```
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>聊天室</title>
</head>
<body>
    <div id="contents" style="height:500px;overflow:auto;"></div>
    <div>
        <textarea id="msg"></textarea>
        <a href="javascript:;" onclick="sendMsg()">发送</a>
    </div>
    <script src="{{static_url('js/jquery.min.js')}}"></script>
    <script type="text/javascript">
        var ws = new WebSocket("ws://192.168.114.177:8000/chat");
        ws.onmessage = function(e) {
            $("#contents").append("<p>" + e.data + "</p>");
        }
        function sendMsg() {
            var msg = $("#msg").val();
            ws.send(msg);
            $("#msg").val("");
        }
    </script>
</body>
</html>
```