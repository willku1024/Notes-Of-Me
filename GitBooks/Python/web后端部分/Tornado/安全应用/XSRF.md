#  XSRF跨站请求伪造

先建立一个网站127.0.0.1:8000，使用上一节中的Cookie计数器：

```
class IndexHandler(RequestHandler):
    def get(self):
        cookie = self.get_secure_cookie("count")
        count = int(cookie) + 1 if cookie else 1
        self.set_secure_cookie("count", str(count))
        self.write(
            '<html><head><title>Cookie计数器</title></head>'
            '<body><h1>您已访问本页%d次。</h1>' % count +
            '</body></html>'
        )
```

再建立一个网站127.0.0.1:9000，

```
class IndexHandler(RequestHandler):
    def get(self):
        self.write('<html><head><title>被攻击的网站</title></head>'
        '<body><h1>此网站的图片链接被修改了</h1>'
        '<img alt="这应该是图片" src="http://127.0.0.1:8000/?f=9000/">'
        '</body></html>'
        )
```

在9000网站我们模拟攻击者修改了我们的图片源地址为8000网站的Cookie计数器页面网址。当我们访问9000网站的时候，在我们不知道、未授权的情况下8000网站的Cookie被使用了，以至于让8000网址认为是我们自己调用了8000网站的逻辑。这就是CSRF（Cross-site request forgery）跨站请求伪造（跨站攻击或跨域攻击的一种），通常缩写为CSRF或者XSRF。

我们刚刚使用的是GET方式模拟的攻击，为了防范这种方式的攻击，任何会产生副作用的HTTP请求，比如点击购买按钮、编辑账户设置、改变密码或删除文档，都应该使用HTTP POST方法（或PUT、DELETE）。但是，这并不足够：一个恶意站点可能会通过其他手段来模拟发送POST请求，保护POST请求需要额外的策略。

## XSRF保护

> 浏览器有一个很重要的概念——**同源策略**(Same-Origin Policy)。 所谓同源是指，域名，协议，端口相同。 不同源的客户端脚本(javascript、ActionScript)在没明确授权的情况下，不能读写对方的资源。

由于第三方站点没有访问cookie数据的权限（同源策略），所以我们可以要求每个请求包括一个特定的参数值作为令牌来匹配存储在cookie中的对应值，如果两者匹配，我们的应用认定请求有效。而第三方站点无法在请求中包含令牌cookie值，这就有效地防止了不可信网站发送未授权的请求。

### 开启XSRF保护

要开启XSRF保护，需要在Application的构造函数中添加xsrf_cookies参数：

```
app = tornado.web.Application(
    [(r"/", IndexHandler),],
    cookie_secret = "2hcicVu+TqShDpfsjMWQLZ0Mkq5NPEWSk9fi0zsSt3A=",
    xsrf_cookies = True
)
```

当这个参数被设置时，Tornado将拒绝请求参数中不包含正确的_xsrf值的POST、PUT和DELETE请求。

```
class IndexHandler(RequestHandler):
    def post(self):
        self.write("hello itcast")
```

用不带_xsrf的post请求时，报出了`HTTP 403: Forbidden ('_xsrf' argument missing from POST)`的错误。

### 模板应用

在模板中使用XSRF保护，只需在模板中添加

```
{% module xsrf_form_html() %}
```

如新建一个模板index.html

```
<!DOCTYPE html>
<html>
<head>
    <title>测试XSRF</title>
</head>
<body>
    <form method="post">
      {% module xsrf_form_html() %}
      <input type="text" name="message"/>
      <input type="submit" value="Post"/>
    </form>
</body>
</html>
```

后端

```
class IndexHandler(RequestHandler):
    def get(self):
        self.render("index.html")

    def post(self):
        self.write("hello itcast")
```

模板中添加的语句帮我们做了两件事：

- 为浏览器设置了_xsrf的Cookie（注意此Cookie浏览器关闭时就会失效）
- 为模板的表单中添加了一个隐藏的输入名为_xsrf，其值为_xsrf的Cookie值

渲染后的页面原码如下：

```
<!DOCTYPE html>
<html>
    <head>
        <title>测试XSRF</title>
    </head>
    <body>
        <form method="post">
            <input type="hidden" name="_xsrf" value="2|543c2206|a056ff9e49df23eaffde0a694cde2b02|1476443353"/>
            <input type="text" name="message"/>
            <input type="submit" value="Post"/>
        </form>
    </body>
</html>
```

### 非模板应用

对于不使用模板的应用来说，首先要设置_xsrf的Cookie值，可以在任意的Handler中通过获取**self.xsrf_token**的值来生成_xsrf并设置Cookie。

下面两种方式都可以起到设置_xsrf Cookie的作用。

```
class XSRFTokenHandler(RequestHandler):
    """专门用来设置_xsrf Cookie的接口"""
    def get(self):
        self.xsrf_token
        self.write("Ok")

class StaticFileHandler(tornado.web.StaticFileHandler):
    """重写StaticFileHandler，构造时触发设置_xsrf Cookie"""
    def __init__(self, *args, **kwargs):
        super(StaticFileHandler, self).__init__(*args, **kwargs)
        self.xsrf_token
```

对于请求携带_xsrf参数，有两种方式：

- 若请求体是表单编码格式的，可以在请求体中添加_xsrf参数
- 若请求体是其他格式的（如json或xml等），可以通过设置HTTP头X-XSRFToken来传递_xsrf值

#### 1. 请求体携带_xsrf参数

新建一个页面xsrf.html：

```
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>测试XSRF</title>
</head>
<body>
    <a href="javascript:;" onclick="xsrfPost()">发送POST请求</a>
    <script src="http://cdn.bootcss.com/jquery/3.1.1/jquery.min.js"></script>
    <script type="text/javascript">
        //获取指定Cookie的函数
        function getCookie(name) {
            var r = document.cookie.match("\\b" + name + "=([^;]*)\\b");
            return r ? r[1] : undefined;
        }
        //AJAX发送post请求，表单格式数据
        function xsrfPost() {
            var xsrf = getCookie("_xsrf");
            $.post("/new", "_xsrf="+xsrf+"&key1=value1", function(data) {
                alert("OK");
            });
        }
    </script>
</body>
</html>
```

#### 2. HTTP头X-XSRFToken

新建一个页面json.html：

```
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>测试XSRF</title>
</head>
<body>
    <a href="javascript:;" onclick="xsrfPost()">发送POST请求</a>
    <script src="http://cdn.bootcss.com/jquery/3.1.1/jquery.min.js"></script>
    <script type="text/javascript">
        //获取指定Cookie的函数
        function getCookie(name) {
            var r = document.cookie.match("\\b" + name + "=([^;]*)\\b");
            return r ? r[1] : undefined;
        }
        //AJAX发送post请求，json格式数据
        function xsrfPost() {
            var xsrf = getCookie("_xsrf");
            var data = {
                key1:1,
                key1:2
            };
            var json_data = JSON.stringify(data);
            $.ajax({
                url: "/new",
                method: "POST",
                headers: {
                    "X-XSRFToken":xsrf,
                },
                data:json_data,
                success:function(data) {
                    alert("OK");
                }
            })
        }
    </script>
</body>
</html>
```