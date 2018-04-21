简单、直观、强悍的前端开发框架，让web开发更迅速、简单。 来自Twitter，是目前很受欢迎的前端框架之一。 Bootrstrap是基于HTML、CSS、JavaScript的，让书写代码更容易。 移动优先，响应式布局开发。

bootstrap中文网址：<http://www.bootcss.com/>

#### bootstrap 容器

- container-fluid 流体
- container
  - 1170
  - 970
  - 750
  - 100%

#### bootstrap 栅格系统

bootstrap将页面横向分为12等分，按照12等分定义了适应不同宽度等分的样式类，这些样式类组成了一套响应式、移动设备优先的流式栅格系统：

1、col-lg-*2、col-md-*
3、col-sm-*4、col-xs-*

#### bootstrap响应式查询区间：

1、大于等于768
2、大于等于992
3、大于等于1200

#### bootstrap 表单

1、form 声明一个表单域
2、form-inline 内联表单域
3、form-horizontal 水平排列表单域
4、form-group 表单组、包括表单文字和表单控件
5、form-control 文本输入框、下拉列表控件样式
6、checkbox checkbox-inline 多选框样式
7、radio radio-inline 单选框样式
8、input-group 表单控件组
9、input-group-addon 表单控件组物件样式 10、input-group-btn 表单控件组物件为按钮的样式
10、form-group-lg 大尺寸表单
11、form-group-sm 小尺寸表单

#### bootstrap 按钮

1、btn 声明按钮
2、btn-default 默认按钮样式
3、btn-primay
4、btn-success
5、btn-info
6、btn-warning
7、btn-danger
8、btn-link
9、btn-lg
10、btn-md
11、btn-xs
12、btn-block 宽度是父级宽100%的按钮
13、active
14、disabled
15、btn-group 定义按钮组

#### bootstrap 图片

img-responsive 声明响应式图片

#### bootstrap 隐藏类

1、hidden-xs
2、hidden-sm
3、hidden-md
4、hidden-lg

#### bootstrap 字体图标

通过字体代替图标，font文件夹需要和css文件夹在同一目录

#### bootstrap 下拉菜单

1、dropdown-toggle
2、dropdown-menu

#### bootstrap 选项卡

1、nav
2、nav-tabs
3、nav-pills
4、tab-content

#### bootstrap 导航条

1、navbar 声明导航条
2、navbar-default 声明默认的导航条样式
3、navbar-inverse 声明反白的导航条样式
4、navbar-static-top 去掉导航条的圆角
5、navbar-fixed-top 固定到顶部的导航条
6、navbar-fixed-bottom 固定到底部的导航条
7、navbar-header 申明logo的容器
8、navbar-brand 针对logo等固定内容的样式
11、nav navbar-nav 定义导航条中的菜单
12、navbar-form 定义导航条中的表单
13、navbar-btn 定义导航条中的按钮
14、navbar-text 定义导航条中的文本
9、navbar-left 菜单靠左
10、navbar-right 菜单靠右

#### bootstrap 模态框

1、modal 声明一个模态框
2、modal-dialog 定义模态框尺寸
3、modal-lg 定义大尺寸模态框
4、modal-sm 定义小尺寸模态框
5、modal-header
6、modal-body
7、modal-footer



---

## 前端性能优化

从用户访问资源到资源完整的展现在用户面前的过程中，通过技术手段和优化策略，缩短每个步骤的处理时间从而提升整个资源的访问和呈现速度。网站的性能直接会影响到用户的数量，所有前端性能优化很重要。

前端性能优化分为如下几个方面：

### 一、代码部署：

1、代码的压缩与合并
2、图片、js、css等静态资源使用和主站不同域名地址存储，从而使得在传输资源时不会带上不必要的cookie信息。
3、使用内容分发网络 CDN
4、为文件设置Last-Modified、Expires和Etag
5、使用GZIP压缩传送
6、权衡DNS查找次数(使用不同域名会增加DNS查找)
7、避免不必要的重定向(加"/")

### 二、编码

#### html：

1、使用结构清晰，简单，语义化标签
2、避免空的src和href
3、不要在HTML中缩放图片

#### css：

1、精简css选择器 2、把CSS放到顶部
3、避免@import方式引入样式
4、css中使用base64图片数据取代图片文件，减少请求数，在线转base64网站：<http://tool.css-js.com/base64.html>
5、使用css动画来取代javascript动画
6、使用字体图标,图标库网站：<http://fontawesome.io/icons/，在线制作网站：www.iconfont.cn>
7、使用css sprite
8、使用svg图形
9、避免使用CSS表达式

```
a {star : expression(onfocus=this.blur)}

```

10、避免使用滤镜

#### javascript：

1、减少引用库的个数
2、使用requirejs或seajs异步加载js
3、JS放到页面底部引入
4、避免全局查找
5、减少属性查找
6、使用原生方法
7、用switch语句代替复杂的if else语句
8、减少语句数，比如说多个变量声明可以写成一句
9、使用字面量表达式来初始化数组或者对象
10、使用DocumentFragments或innerHTML取代复杂的元素注入
11、使用事件代理(事件委托)
12、避免多次访问dom选择集
13、高频触发事件设置使用函数节流，如：onmousemove、onmouseover
14、使用Web Storage缓存数据
15、使用Array的join方法来代替字符串的“+”连接(先将要连接的字符串放进数组)