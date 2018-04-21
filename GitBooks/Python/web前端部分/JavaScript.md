# JavaScript初级

#### 1、变量

##### （1）变量类型

- 5种基本数据类型：number、string、boolean、undefined、null


- 1种复合类型：object

##### （2）定义多个变量

```javascript
 //JavaScript 是一种弱类型语言，javascript的变量类型由它的值来决定。 定义变量需要用关键字 'var'
 var a = 123;
 var b = 'asd';

 //同时定义多个变量可以用","隔开，公用一个‘var’关键字
 var c = 45,d='qwe',f='68';
```
##### （3）变量、函数、属性、函数参数命名规范

- 区分大小写
- 第一个字符必须是字母、下划线（_）或者美元符号（$）
- 其他字符可以是字母、下划线、美元符或数字



#### 2、获取元素

##### （1）将js语句放到最后面

```javascript
....
<div id="div1">这是一个div元素</div>
....

<script type="text/javascript">
    var oDiv = document.getElementById('div1');
</script>
</body>
```

##### （2）使用window.onload

```javascript
<script type="text/javascript">
    window.onload = function(){
        var oDiv = document.getElementById('div1');
    }
</script>

....

<div id="div1">这是一个div元素</div>
```



#### 3、操作元素属性

##### （1）操作属性的方法

- “.” 操作符
- “[ ]”操作符

##### （2）属性写法

- html的属性和js里面属性写法一样
- “class” 属性写成 “className”
- “style” 属性里面的属性，有横杠的改成驼峰式，比如：“font-size”，改成”style.fontSize”

##### （3）例子

```javascript
<script type="text/javascript">

    window.onload = function(){
        var oInput = document.getElementById('input1');
        var oA = document.getElementById('link1');
        // 读取属性值
        var val = oInput.value;
        var typ = oInput.type;
        var nam = oInput.name;
        var links = oA.href;
        // 写(设置)属性
        oA.style.color = 'red';
        oA.style.fontSize = val;
        oA.style[fontSize] = val;
    }

</script>

......

<input type="text" name="setsize" id="input1" value="20px">
<a href="http://www.itcast.cn" id="link1">传智播客</a>
```



#### 4、innerHTML 

innerHTML可以读取或者写入标签包裹的内容

```javascript
<script type="text/javascript">
    window.onload = function(){
        var oDiv = document.getElementById('div1');
        //读取
        var txt = oDiv.innerHTML;
        alert(txt);
        //写入
        oDiv.innerHTML = '<a href="http://www.itcast.cn">传智播客<a/>';
    }
</script>

......

<div id="div1">这是一个div元素</div>
```



#### 5、函数

##### （1）提取行间事件 

在html行间调用的事件可以提取到javascript中调用，从而做到结构与行为分离。

```javascript
<!--行间事件调用函数   -->
<script type="text/javascript">        
    function myalert(){
        alert('ok!');
    }
</script>
......
<input type="button" name="" value="弹出" onclick="myalert()">

<!-- 提取行间事件 -->
<script type="text/javascript">

window.onload = function(){
    var oBtn = document.getElementById('btn1');
    oBtn.onclick = myalert;
    function myalert(){
        alert('ok!');
    }
}    
</script>
......
<input type="button" name="" value="弹出" id="btn1">
```

##### （2）匿名函数

```javascript
<script type="text/javascript">

window.onload = function(){
    var oBtn = document.getElementById('btn1');
    /*
    oBtn.onclick = myalert;
    function myalert(){
        alert('ok!');
    }
    */
    // 直接将匿名函数赋值给绑定的事件

    oBtn.onclick = function (){
        alert('ok!');
    }
}

</script>
```

##### （3）'return'关键字 

函数中'return'关键字的作用：
1、返回函数执行的结果
2、结束函数的运行
3、阻止默认行为

```javascript
<script type="text/javascript">
function add(a,b){
    var c = a + b;
    return c;
    alert('here!');
}

var d = add(3,4);
alert(d);  //弹出7
</script>
```



# JavaScript中级

#### 1、条件语句

**运算符 **
1、算术运算符： +(加)、 -(减)、 *(乘)、 /(除)、 %(求余)
2、赋值运算符：=、 +=、 -=、 *=、 /=、 %=
3、条件运算符：==、===、>、>=、<、<=、!=、&&(而且)、||(或者)、!(否)

##### （1）if...else...

```javascript
var a = 6;
if(a==1)
{
    alert('语文');
}
else if(a==2)
{
    alert('数学');
}
else
{
    alert('不补习');
}
```

##### （2）switch

```javascript
var a = 6;

switch (a){
    case 1:
        alert('语文');
        break;
    case 2:
        alert('数学');
        break;
    default:
        alert('不补习');
}
```



#### 2、循环语句

程序中进行有规律的重复性操作，需要用到循环语句。

##### （1）for循环

```javascript
for(var i=0;i<len;i++)
{
    ......
}

```

##### （2）while循环

```javascript
var i=0;

while(i<8){

    ......

    i++;

}

```

##### （3）数组去重

```javascript
var aList = [1,2,3,4,4,3,2,1,2,3,4,5,6,5,5,3,3,4,2,1];

var aList2 = [];

for(var i=0;i<aList.length;i++)
{
    if(aList.indexOf(aList[i])==i)
    {
        aList2.push(aList[i]);
    }
}

alert(aList2);
```



#### 3、数组以及操作方法

数组就是一组数据的集合，javascript中，数组里面的数据可以是不同类型的。

##### （1）定义数组的方法

```javascript
//对象的实例创建
var aList = new Array(1,2,3);

//直接量创建
var aList2 = [1,2,3,'asd'];

```

##### （2）操作数组中数据的方法 

- 获取数组的长度：aList.length;

```javascript
var aList = [1,2,3,4];
alert(aList.length); // 弹出4
```

- 用下标操作数组的某个数据：aList[0];

```javascript
var aList = [1,2,3,4];
alert(aList[0]); // 弹出1
```

- join() 将数组成员通过一个分隔符合并成字符串

```javascript
var aList = [1,2,3,4];
alert(aList.join('-')); // 弹出 1-2-3-4
```

- push() 和 pop() 从数组最后增加成员或删除成员

```javascript
var aList = [1,2,3,4];
aList.push(5);
alert(aList); //弹出1,2,3,4,5
aList.pop();
alert(aList); // 弹出1,2,3,4
```

- unshift()和 shift() 从数组前面增加成员或删除成员

```javascript
var aList = [1,2,3,4];
aList.unshift(5);
alert(aList); //弹出5,1,2,3,4
aList.shift();
alert(aList); // 弹出1,2,3,4
```

- reverse() 将数组反转

```javascript
var aList = [1,2,3,4];
aList.reverse();
alert(aList);  // 弹出4,3,2,1
```

- indexOf() 返回数组中元素第一次出现的索引值

```javascript
var aList = [1,2,3,4,1,3,4];
alert(aList.indexOf(1));
```

- splice() 在数组中增加或删除成员

```javascript
var aList = [1,2,3,4];
aList.splice(2,1,7,8,9); //从第2个元素开始，删除1个元素，然后在此位置增加'7,8,9'三个元素
alert(aList); //弹出 1,2,7,8,9,4
```

##### （3）多维数组 

多维数组指的是数组的成员也是数组的数组。

```javascript
var aList = [[1,2,3],['a','b','c']];

alert(aList[0][1]); //弹出2;
```

##### （4）获取元素的第二种方法 

document.getElementsByTagName(''),获取的是一个选择集，不是数组，但是可以用下标的方式操作选择集里面的dom元素。



#### 4、字符串

##### （1）常用的方法

- 字符串合并操作：“ + ”
- parseInt() 将数字字符串转化为整数
- parseFloat() 将数字字符串转化为小数
- split() 把一个字符串分隔成字符串组成的数组
- charAt() 获取字符串中的某一个字符
- indexOf() 查找字符串是否含有某字符
- substring() 截取字符串 用法： substring(start,end)（不包括end）
- toUpperCase() 字符串转大写
- toLowerCase() 字符串转小写

##### （2）字符串反转

```
var str = 'asdfj12jlsdkf098';
var str2 = str.split('').reverse().join('');
alert(str2);
```



#### 5、定时器

**定时器在javascript中的作用**
1、制作动画
2、异步操作
3、函数缓冲与节流

##### （1）示例

```
/*
    定时器：
    setTimeout  只执行一次的定时器 
    clearTimeout 关闭只执行一次的定时器
    setInterval  反复执行的定时器
    clearInterval 关闭反复执行的定时器

*/

var time1 = setTimeout(myalert,2000);
var time2 = setInterval(myalert,2000);
/*
clearTimeout(time1);
clearInterval(time2);
*/
function myalert(){
    alert('ok!');
}
```

##### （2）定时器制作时钟

```
<script type="text/javascript">
    window.onload = function(){    
        var oDiv = document.getElementById('div1');
        function timego(){
            var now = new Date();
            var year = now.getFullYear();
            var month = now.getMonth()+1;
            var date = now.getDate();
            var week = now.getDay();
            var hour = now.getHours();
            var minute = now.getMinutes();
            var second = now.getSeconds();
            var str = '当前时间是：'+ year + '年'+month+'月'+date+'日 '+toweek(week)+' '+todou(hour)+':'+todou(minute)+':'+todou(second);
            oDiv.innerHTML = str;
        }
        timego();
        setInterval(timego,1000);
    }

    function toweek(n){
        if(n==0)
        {
            return '星期日';
        }
        else if(n==1)
        {
            return '星期一';
        }
        else if(n==2)
        {
            return '星期二';
        }
        else if(n==3)
        {
            return '星期三';
        }
        else if(n==4)
        {
            return '星期四';
        }
        else if(n==5)
        {
            return '星期五';
        }
        else
        {
            return '星期六';
        }
    }


    function todou(n){
        if(n<10)
        {
            return '0'+n;
        }
        else
        {
            return n;
        }
    }
</script>
......
<div id="div1"></div>

```

##### （3）定时器制作倒计时

```
<script type="text/javascript">
    window.onload = function(){
        var oDiv = document.getElementById('div1');
        function timeleft(){
            var now = new Date();
            var future = new Date(2016,8,12,24,0,0);
            var lefts = parseInt((future-now)/1000);
            var day = parseInt(lefts/86400);
            var hour = parseInt(lefts%86400/3600);
            var min = parseInt(lefts%86400%3600/60);
            var sec = lefts%60;
            str = '距离2016年9月12日晚24点还剩下'+day+'天'+hour+'时'+min+'分'+sec+'秒';
            oDiv.innerHTML = str; 
        }
        timeleft();
        setInterval(timeleft,1000);        
    }

</script>
......
<div id="div1"></div>
```



#### 6、类型转换

##### （1）直接转换 parseInt() 与 parseFloat()

```
alert('12'+7); //弹出127
alert( parseInt('12') + 7 );  //弹出19 
alert( parseInt(5.6));  // 弹出5
alert('5.6'+2.3);  // 弹出5.62.3
alert(parseFloat('5.6')+2.3);  // 弹出7.8999999999999995
alert(0.1+0.2); //弹出 0.3000000000000004
alert((0.1*100+0.2*100)/100); //弹出0.3
alert((parseFloat('5.6')*100+2.3*100)/100); //弹出7.9

```

##### （2）隐式转换 “==” 和 “-”

```
if('3'==3)
{
    alert('相等');
}

// 弹出'相等'
alert('10'-3);  // 弹出7

```

##### （4）NaN 和 isNaN

```
alert( parseInt('123abc') );  // 弹出123
alert( parseInt('abc123') );  // 弹出Na
```



# JavaScript高级

#### 1、变量作用域

变量作用域指的是变量的作用范围，javascript中的变量分为全局变量和局部变量。

- 全局变量：在函数之外定义的变量，为整个页面公用，函数内部外部都可以访问。
- 局部变量：在函数内部定义的变量，只能在定义该变量的函数内部访问，外部无法访问。

```
<script type="text/javascript">
    //全局变量
    var a = 12;
    function myalert()
    {
        //局部变量
        var b = 23;
        alert(a);
        alert(b);
    }
    myalert(); //弹出12和23
    alert(a);  //弹出12    
    alert(b);  //出错
</script>
```



#### 2、封闭函数

封闭函数：

```
(function(){
    var oDiv = document.getElementById('div1');
    oDiv.style.color = 'red';
})();

```

还可以在函数定义前加上“~”和“!”等符号来定义匿名函数

```
!function(){
    var oDiv = document.getElementById('div1');
    oDiv.style.color = 'red';
}()
```



#### 3、闭包

**什么是闭包 **
函数嵌套函数，内部函数可以引用外部函数的参数和变量，参数和变量不会被垃圾回收机制收回

##### （1）示例

```
function aaa(a){      
      var b = 5;      
      function bbb(){
           a++;
           b++;
         alert(a);
         alert(b);
      }
      return bbb;
  }

 var ccc = aaa(2);

 ccc();
 ccc();

```

改写成封闭函数的形式：

```
var ccc = (function(a){

  var b = 5;
  function bbb(){
       a++;
       b++;
     alert(a);
     alert(b);
  }
  return bbb;

})(2);

ccc();
ccc();


```

##### （2）用处 

- 将一个变量长期驻扎在内存当中，可用于循环中存索引值

```
<script type="text/javascript">
    window.onload = function(){
        var aLi = document.getElementsByTagName('li');
        for(var i=0;i<aLi.length;i++)
        {
            (function(i){
                aLi[i].onclick = function(){
                    alert(i);
                }
            })(i);
        }
    }
</script>
......
<ul>
    <li>111</li>
    <li>222</li>
    <li>333</li>
    <li>444</li>
    <li>555</li>
</ul>

```

- 私有变量计数器，外部无法访问，避免全局变量的污染

```
<script type="text/javascript">

var count = (function(){
    var a = 0;
    function add(){
        a++;
        return a;
    }

    return add;

})()

count();
count();

var nowcount = count();

alert(nowcount);

</script>
```



#### 4、内置对象

（1）document

```
document.referrer  //获取上一个跳转页面的地址(需要服务器环境)
```

（2）location

```
window.location.href  //获取或者重定url地址
window.location.search //获取地址参数部分
window.location.hash //获取页面锚点或者叫哈希值
```

（3）Math

```
Math.random 获取0-1的随机数
Math.floor 向下取整
Math.ceil 向上取整
```



#### 5、面向对象

（1）面向过程与面向对象编程

1、面向过程：所有的工作都是现写现用。

2、面向对象：是一种编程思想，许多功能事先已经编写好了，在使用时，只需要关注功能的运用，而不需要这个功能的具体实现过程。

（2）javascript对象 
将相关的变量和函数组合成一个整体，这个整体叫做对象，对象中的变量叫做属性，变量中的函数叫做方法。javascript中的对象类似字典。

（3）创建对象的方法 

- 单体

```
<script type="text/javascript">
var Tom = {
    name : 'tom',
    age : 18,
    showname : function(){
        alert('我的名字叫'+this.name);    
    },
    showage : function(){
        alert('我今年'+this.age+'岁');    
    }
}
</script>

```

- 工厂模式

```
<script type="text/javascript">

function Person(name,age,job){
    var o = new Object();
    o.name = name;
    o.age = age;
    o.job = job;
    o.showname = function(){
        alert('我的名字叫'+this.name);    
    };
    o.showage = function(){
        alert('我今年'+this.age+'岁');    
    };
    o.showjob = function(){
        alert('我的工作是'+this.job);    
    };
    return o;
}
var tom = Person('tom',18,'程序员');
tom.showname();

</script>

```

- 构造函数

```
<script type="text/javascript">
    function Person(name,age,job){            
        this.name = name;
        this.age = age;
        this.job = job;
        this.showname = function(){
            alert('我的名字叫'+this.name);    
        };
        this.showage = function(){
            alert('我今年'+this.age+'岁');    
        };
        this.showjob = function(){
            alert('我的工作是'+this.job);    
        };
    }
    var tom = new Person('tom',18,'程序员');
    var jack = new Person('jack',19,'销售');
    alert(tom.showjob==jack.showjob);
</script>

```

- 原型模式

```
<script type="text/javascript">
    function Person(name,age,job){        
        this.name = name;
        this.age = age;
        this.job = job;
    }
    Person.prototype.showname = function(){
        alert('我的名字叫'+this.name);    
    };
    Person.prototype.showage = function(){
        alert('我今年'+this.age+'岁');    
    };
    Person.prototype.showjob = function(){
        alert('我的工作是'+this.job);    
    };
    var tom = new Person('tom',18,'程序员');
    var jack = new Person('jack',19,'销售');
    alert(tom.showjob==jack.showjob);
</script>

```

- 继承

```
<script type="text/javascript">

        function fclass(name,age){
            this.name = name;
            this.age = age;
        }
        fclass.prototype.showname = function(){
            alert(this.name);
        }
        fclass.prototype.showage = function(){
            alert(this.age);
        }
        function sclass(name,age,job)
        {
            fclass.call(this,name,age);
            this.job = job;
        }
        sclass.prototype = new fclass();
        sclass.prototype.showjob = function(){
            alert(this.job);
        }
        var tom = new sclass('tom',19,'全栈工程师');
        tom.showname();
        tom.showage();
        tom.showjob();
    </script>
```

