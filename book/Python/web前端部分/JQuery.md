# JQuery初级

#### 1、加载

将获取元素的语句写到页面头部，会因为元素还没有加载而出错，jquery提供了ready方法解决这个问题，它的速度比原生的 window.onload 更快。

```
<script type="text/javascript">

$(document).ready(function(){

     ......

});

</script>

```

可以简写为：

```
<script type="text/javascript">

$(function(){

     ......

});

</script>
```

#### 2、选择器

##### （1）jquery选择器 

jquery选择器可以快速地选择元素，选择规则和css样式相同，使用length属性判断是否选择成功。

```
$(document) //选择整个文档对象
$('li') //选择所有的li元素
$('#myId') //选择id为myId的网页元素
$('.myClass') // 选择class为myClass的元素
$('input[name=first]') // 选择name属性等于first的input元素
$('#ul1 li span') //选择id为为ul1元素下的所有li下的span元素

```

##### （2）对选择集进行修饰过滤(类似CSS伪类)

```
$('#ul1 li:first') //选择id为ul1元素下的第一个li
$('#ul1 li:odd') //选择id为ul1元素下的li的奇数行
$('#ul1 li:eq(2)') //选择id为ul1元素下的第3个li
$('#ul1 li:gt(2)') // 选择id为ul1元素下的前三个之后的li
$('#myForm :input') // 选择表单中的input元素
$('div:visible') //选择可见的div元素

```

##### （3）对选择集进行函数过滤

```
$('div').has('p'); // 选择包含p元素的div元素
$('div').not('.myClass'); //选择class不等于myClass的div元素
$('div').filter('.myClass'); //选择class等于myClass的div元素
$('div').first(); //选择第1个div元素
$('div').eq(5); //选择第6个div元素

```

##### （4）选择集转移

```
$('div').prev('p'); //选择div元素前面的第一个p元素
$('div').next('p'); //选择div元素后面的第一个p元素
$('div').closest('form'); //选择离div最近的那个form父元素
$('div').parent(); //选择div的父元素
$('div').children(); //选择div的所有子元素
$('div').siblings(); //选择div的同级元素
$('div').find('.myClass'); //选择div内的class等于myClass的元素
```

##### （5）示例

```css
<script type="text/javascript">
$(function(){
	// next() 同级的下一个元素，nextAll同级的下面所有的元素
	// prev() 同级的上一个元素，prevAll同级的上面所有的元素
	$('#div1').nextAll('p').css({color:'red'});
	//选择上一级的父元素
    $('#span01').parent().css({width:'100px',height:'100px',background:'gold'});
	//选择离最近的元素，元素可以是父级，可以是子集			  $('#span02').closest('div').css({width:'200px',height:'200px',background:'pink'});
	// $('.list li')：不能回到父级
	// $('.list').children()：可以通过end() 回到父级
  $('.list').children().css({background:'gold',height:'30px',marginBottom:'10px'}).end().css({background:'green'});
$('.list2 li:eq(2)').css({background:'gold'}).siblings().css({background:'green'});
$('#div2').find('.link1').css({color:'red'});

})
</script>
```

---

```
<script type="text/javascript">
$(function(){
	$('#btns input').click(function() {
	// this是原生的对象
	$(this).addClass('cur').siblings().removeClass('cur');
				
	//$(this).index() 获取当前按钮所在层级范围的索引值
	$('#contents div').eq($(this).index()).addClass('active').siblings().removeClass('active');
			});
		})
		
</script>
```



#### 3、样式操作

##### （1）操作行间样式

```
// 获取div的样式
$("div").css("width");
$("div").css("color");


//设置div的样式
$("div").css("width","30px");
$("div").css("height","30px");
$("div").css({fontSize:"30px",color:"red"});

```

##### （2）特别注意 

选择器获取的多个元素，获取信息获取的是第一个，比如：$("div").css("width")，获取的是第一个div的width。

##### （3）操作样式类名

```
$("#div1").addClass("divClass2") //为id为div1的对象追加样式divClass2
$("#div1").removeClass("divClass")  //移除id为div1的对象的class名为divClass的样式
$("#div1").removeClass("divClass divClass2") //移除多个样式
$("#div1").toggleClass("anotherClass") //重复切换anotherClass样式
```



#### 4、jquery属性操作

##### （1）html() 取出或设置html内容

```
// 取出html内容

var $htm = $('#div1').html();

// 设置html内容

$('#div1').html('<span>添加文字</span>');

```

##### （2）text() 取出或设置text内容

```
// 取出文本内容

var $htm = $('#div1').text();

// 设置文本内容

$('#div1').text('<span>添加文字</span>');

```

##### （3）attr() 取出或设置某个属性的值

```
// 取出图片的地址

var $src = $('#img1').attr('src');

// 设置图片的地址和alt属性

$('#img1').attr({ src: "test.jpg", alt: "Test Image" });
```



# JQuery中级

#### 1、绑定事件

```
$('#btn1').click(function(){

    // 内部的this指的是原生对象

    // 使用jquery对象用 $(this)

})
```



#### 2、jquery特殊效果

```
fadeIn() 淡入

    $btn.click(function(){

        $('#div1').fadeIn(1000,'swing',function(){
            alert('done!');
        });

    });

fadeOut() 淡出
fadeToggle() 切换淡入淡出
hide() 隐藏元素
show() 显示元素
toggle() 依次展示或隐藏某个元素
slideDown() 向下展开
slideUp() 向上卷起
slideToggle() 依次展开或卷起某个元素
```



#### 3、尺寸相关、滚动事件

##### （1）获取和设置元素的尺寸

```
width()、height()    获取元素width和height  
innerWidth()、innerHeight()  包括padding的width和height  
outerWidth()、outerHeight()  包括padding和border的width和height  
outerWidth(true)、outerHeight(true)   包括padding和border以及margin的width和height

```

##### （2）获取元素相对页面的绝对位置

```
offse()

```

##### （3）获取可视区高度

```
$(window).height();

```

##### （4）获取页面高度

```
$(document).height();

```

##### （5）获取页面滚动距离

```
$(document).scrollTop();  
$(document).scrollLeft();

```

##### （6）页面滚动事件

```
$(window).scroll(function(){  
    ......  
})
```



#### 4、jquery事件

##### （1）事件函数列表：

```
blur() 元素失去焦点
focus() 元素获得焦点
change() 表单元素的值发生变化
click() 鼠标单击
dblclick() 鼠标双击
mouseover() 鼠标进入（进入子元素也触发）
mouseout() 鼠标离开（离开子元素也触发）
mouseenter() 鼠标进入（进入子元素不触发）
mouseleave() 鼠标离开（离开子元素不触发）
hover() 同时为mouseenter和mouseleave事件指定处理函数
mouseup() 松开鼠标
mousedown() 按下鼠标
mousemove() 鼠标在元素内部移动
keydown() 按下键盘
keypress() 按下键盘
keyup() 松开键盘
load() 元素加载完毕
ready() DOM加载完成
resize() 浏览器窗口的大小发生改变
scroll() 滚动条的位置发生变化
select() 用户选中文本框中的内容
submit() 用户递交表单
toggle() 根据鼠标点击的次数，依次运行多个函数
unload() 用户离开页面

```

##### （2）绑定事件的其他方式

```
$(function(){
    $('#div1').bind('mouseover click', function(event) {
        alert($(this).html());
    });
});

```

##### （3）取消绑定事件

```
$(function(){
    $('#div1').bind('mouseover click', function(event) {
        alert($(this).html());

        // $(this).unbind();
        $(this).unbind('mouseover');

    });
});
```

##### （4）事件委托

事件委托就是利用冒泡的原理，把事件加到父级上，通过判断事件来源的子集，执行相应的操作，事件委托首先可以极大减少事件绑定次数，提高性能；其次可以让新加入的子元素也可以拥有相同的操作。

**一般绑定事件的写法**

```
$(function(){
    $ali = $('#list li');
    $ali.click(function(event) {
        $(this).css({background:'red'});
    });
})
...
<ul id="list">
    <li>1</li>
    <li>2</li>
    <li>3</li>
    <li>4</li>
    <li>5</li>
</ul>

```

**事件委托的写法**

```
$(function(){
    $list = $('#list');
    $list.delegate('li', 'click', function(event) {
        $(this).css({background:'red'});
    });
})
...
<ul id="list">
    <li>1</li>
    <li>2</li>
    <li>3</li>
    <li>4</li>
    <li>5</li>
</ul>

```

**取消事件委托**

```
// ev.delegateTarge 委托对象
$(ev.delegateTarge).undelegate();

// 上面的例子可使用 $list.undelegate();
```

#### 5、主动触发与自定义事件

（1）主动触发 
可使用jquery对象上的trigger方法来触发对象上绑定的事件。

（2）自定义事件 
除了系统事件外，可以通过bind方法自定义事件，然后用tiggle方法触发这些事件。

```
//给element绑定hello事件
element.bind("hello",function(){
    alert("hello world!");
});

//触发hello事件
element.trigger("hello");
```



#### 6、jquery元素节点操作

（1）创建节点

```
var $div = $('<div>');
var $div2 = $('<div>这是一个div元素</div>');
```

（2）插入节点 

- append()和appendTo()：在现存元素的内部，从后面插入元素

```
var $span = $('<span>这是一个span元素</span>');
$('#div1').append($span);
......
<div id="div1"></div
```

- prepend()和prependTo()：在现存元素的内部，从前面插入元素
- after()和insertAfter()：在现存元素的外部，从后面插入元素
- before()和insertBefore()：在现存元素的外部，从前面插入元素

##### （4）删除节点

```
$('#div1').remove();
```



# JQuery高级

#### 1、ajax与jsonp

##### （1）$.ajax使用方法 

常用参数：
1、url 请求地址
2、type 请求方式，默认是'GET'，常用的还有'POST'
3、dataType 设置返回的数据格式，常用的是'json'格式，也可以设置为'html'
4、data 设置发送给服务器的数据
5、success 设置请求成功后的回调函数
6、error 设置请求失败后的回调函数
7、async 设置是否异步，默认值是'true'，表示异步

以前的写法：

```
$.ajax({
    url: 'js/user.json',
    type: 'GET',
    dataType: 'json',
    data:{'aa':1}
    success:function(data){
        ......
    },
    error:function(){
        alert('服务器超时，请重试！');
    }
});

```

新的写法(推荐)：

```
$.ajax({
    url: 'js/user.json',
    type: 'GET',
    dataType: 'json',
    data:{'aa':1}
})
.done(function(data) {
    ......
})
.fail(function() {
    alert('服务器超时，请重试！');
});

```

##### （2）jsonp 

ajax只能请求同一个域下的数据或资源，有时候需要跨域请求数据，就需要用到jsonp技术，jsonp可以跨域请求数据，它的原理主要是利用了script标签可以跨域链接资源的特性。

jsonp的原理如下：

```
<script type="text/javascript">
    function aa(dat){
        alert(dat.name);
    }
</script>
<script type="text/javascript" src="....../js/data.js"></script>

```

页面上定义一个函数，引用一个外部js文件，外部js文件的地址可以是不同域的地址，外部js文件的内容如下：

```
aa({"name":"tom","age":18});

```

外部js文件调用页面上定义的函数，通过参数把数据传进去。

##### (3)正则表达式常用函数

- test

用法：正则.test(字符串) 匹配成功，就返回真，否则就返回假

- search

用法：字符串.search(正则) 匹配成功，返回位置，否则返回-1

- match

用法： 字符串.match(正则)；匹配成功，就返回匹配成功的数组，否则就返回null

- replace

用法：字符串.replace(正则，新的字符串) 匹配成功的字符去替换新的字符



#### 2、本地存储

本地存储分为cookie，以及新增的localStorage和sessionStorage

##### （1）cookie

cookie 存储在本地，容量最大4k，在同源的http请求时携带传递，损耗带宽，可设置访问路径，只有此路径及此路径的子路径才能访问此cookie，在设置的过期时间之前有效。

```
jquery 设置cookie
$.cookie('mycookie','123',{expires:7,path:'/'});
jquery 获取cookie
$.cookie('mycookie');

```

##### （2）localStorage 

localStorage 存储在本地，容量为5M或者更大，不会在请求时候携带传递，在所有同源窗口中共享，数据一直有效，除非人为删除，可作为长期数据。

```
//设置：
localStorage.setItem("dat", "456");
localStorage.dat = '456';

//获取：
localStorage.getItem("dat");
localStorage.dat

//删除
localStorage.removeItem("dat");

```

##### （3）sessionStorage

sessionStorage 存储在本地，容量为5M或者更大，不会在请求时候携带传递，在同源的当前窗口关闭前有效。

localStorage 和 sessionStorage 合称为Web Storage , Web Storage支持事件通知机制，可以将数据更新的通知监听者，Web Storage的api接口使用更方便。

iPhone的无痕浏览不支持Web Storage，只能用cookie。