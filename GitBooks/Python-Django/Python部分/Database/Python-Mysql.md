# 1、安装引入模块

- 安装mysql模块

```
sudo apt-get install python-mysql

```

- 在文件中引入模块

```
import Mysqldb
```



# 2、Connection对象

- 用于建立与数据库的连接
- 创建对象：调用connect()方法

```
conn=connect(参数列表)

```

- 参数host：连接的mysql主机，如果本机是'localhost'
- 参数port：连接的mysql主机的端口，默认是3306
- 参数db：数据库的名称
- 参数user：连接的用户名
- 参数password：连接的密码
- 参数charset：通信采用的编码方式，默认是'gb2312'，要求与数据库创建时指定的编码一致，否则中文会乱码

#### 对象的方法

- close()关闭连接
- commit()事务，所以需要提交才会生效
- rollback()事务，放弃之前的操作
- cursor()返回Cursor对象，用于执行sql语句并获得结果



# 3、Cursor对象

- 执行sql语句
- 创建对象：调用Connection对象的cursor()方法

```
cursor1=conn.cursor()

```

#### 对象的方法

- close()关闭
- execute(operation [, parameters ])执行语句，返回受影响的行数
- fetchone()执行查询语句时，获取查询结果集的第一个行数据，返回一个元组
- next()执行查询语句时，获取当前行的下一行
- fetchall()执行查询时，获取结果集的所有行，一行构成一个元组，再将这些元组装入一个元组返回
- scroll(value[,mode])将行指针移动到某个位置
  - mode表示移动的方式
  - mode的默认值为relative，表示基于当前行移动到value，value为正则向下移动，value为负则向上移动
  - mode的值为absolute，表示基于第一条数据的位置，第一条数据的位置为0

#### 对象的属性

- rowcount只读属性，表示最近一次execute()执行后受影响的行数
- connection获得当前连接对象




# 4、封装

- 观察前面的文件发现，除了sql语句及参数不同，其它语句都是一样的
- 创建MysqlHelper.py文件，定义类

```python
#encoding=utf8
import MySQLdb

class MysqlHelper():
    def __init__(self,host,port,db,user,passwd,charset='utf8'):
        self.host=host
        self.port=port
        self.db=db
        self.user=user
        self.passwd=passwd
        self.charset=charset

    def connect(self):
        self.conn=MySQLdb.connect(host=self.host,port=self.port,db=self.db,user=self.user,passwd=self.passwd,charset=self.charset)
        self.cursor=self.conn.cursor()

    def close(self):
        self.cursor.close()
        self.conn.close()

    def get_one(self,sql,params=()):
        result=None
        try:
            self.connect()
            self.cursor.execute(sql, params)
            result = self.cursor.fetchone()
            self.close()
        except Exception, e:
            print e.message
        return result

    def get_all(self,sql,params=()):
        list=()
        try:
            self.connect()
            self.cursor.execute(sql,params)
            list=self.cursor.fetchall()
            self.close()
        except Exception,e:
            print e.message
        return list

    def insert(self,sql,params=()):
        return self.__edit(sql,params)

    def update(self, sql, params=()):
        return self.__edit(sql, params)

    def delete(self, sql, params=()):
        return self.__edit(sql, params)

    def __edit(self,sql,params):
        count=0
        try:
            self.connect()
            count=self.cursor.execute(sql,params)
            self.conn.commit()
            self.close()
        except Exception,e:
            print e.message
        return count

```

#### 添加

- 创建testInsertWrap.py文件，使用封装好的帮助类完成插入操作

```python
#encoding=utf8
from MysqlHelper import *

sql='insert into students(sname,gender) values(%s,%s)'
sname=raw_input("请输入用户名：")
gender=raw_input("请输入性别，1为男，0为女")
params=[sname,bool(gender)]

mysqlHelper=MysqlHelper('localhost',3306,'test1','root','mysql')
count=mysqlHelper.insert(sql,params)
if count==1:
    print 'ok'
else:
    print 'error'

```

#### 查询一个

- 创建testGetOneWrap.py文件，使用封装好的帮助类完成查询最新一行数据操作

```python
#encoding=utf8
from MysqlHelper import *

sql='select sname,gender from students order by id desc'

helper=MysqlHelper('localhost',3306,'test1','root','mysql')
one=helper.get_one(sql)
print one
```