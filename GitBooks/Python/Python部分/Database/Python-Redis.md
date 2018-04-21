# 1、NoSQL简介

- NoSQL，全名为Not Only SQL，指的是非关系型的数据库
- 随着访问量的上升，网站的数据库性能出现了问题，于是nosql被设计出来

#### 优点/缺点

- 优点:
  - 高可扩展性
  - 分布式计算
  - 低成本
  - 架构的灵活性，半结构化数据
  - 没有复杂的关系
- 缺点:
  - 没有标准化
  - 有限的查询功能（到目前为止）
  - 最终一致是不直观的程序

#### 分类

| 类型          | 部分代表                                         | 特点                                                         |
| ------------- | ------------------------------------------------ | ------------------------------------------------------------ |
| 列存储        | HbaseCassandraHypertable                         | 顾名思义，是按列存储数据的。最大的特点是方便存储结构化和半结构化数据，方便做数据压缩，对针对某一列或者某几列的查询有非常大的IO优势。 |
| 文档存储      | MongoDBCouchDB                                   | 文档存储一般用类似json的格式存储，存储的内容是文档型的。这样也就有有机会对某些字段建立索引，实现关系数据库的某些功能。 |
| key-value存储 | Tokyo Cabinet / TyrantBerkeley DBMemcacheDBRedis | 可以通过key快速查询到其value。一般来说，存储不管value的格式，照单全收。（Redis包含了其他功能） |
| 图存储        | Neo4JFlockDB                                     | 图形关系的最佳存储。使用传统关系数据库来解决的话性能低下，而且设计使用不方便。 |
| 对象存储      | db4oVersant                                      | 通过类似面向对象语言的语法操作数据库，通过对象的方式存取数据。 |
| xml数据库     | Berkeley DB XMLBaseX                             | 高效的存储XML数据，并支持XML的内部查询语法，比如XQuery,Xpath。 |



# 2、安装

- 下载：打开[redis官方网站](https://redis.io/)，推荐下载稳定版本(stable)
- 解压

```
tar zxvf redis-3.2.5.tar.gz

```

- 复制：推荐放到usr/local目录下

```
sudo mv -r redis-3.2.3/* /usr/local/redis/
进入redis目录
cd /usr/local/redis/

```

- 生成

```
sudo make

```

- 测试

```
sudo make test
这段运行时间会较长

```

- 安装：将redis的命令安装到/usr/bin/目录

```
sudo make install

```



# 3、运行

- 启动服务器：在桌面目录

```
redis-server
按ctrl+c停止

```

- 启动客户端：在新终端中运行如下代码

```
redis-cli

```

- 运行命令

```
ping
set 'a' '123'

```

- 当添加键值后，发现在当前运行的目录下，创建了一个文件：dump.rdb，这个文件用于将数据持久化存储



# 3、基本配置

- 在源文件/usr/local/redis目录下，文件redis.conf为配置文件
- 绑定地址：如果需要远程访问，可将此行注释

```
bind 127.0.0.1

```

- 端口，默认为6379

```
port 6379

```

- 是否以守护进程运行
  - 如果以守护进程运行，则不会在命令行阻塞，类似于服务
  - 如果以非守护进程运行，则当前终端被阻塞，无法使用
  - 推荐改为yes，以守护进程运行

```
daemonize no|yes

```

- 数据文件

```
dbfilename dump.rdb

```

- 数据文件存储路径

```
dir的默认值为./，表示当前目录
推荐改为：dir /var/lib/redis

```

#### 使用配置文件方式启动

- 直接运行redis-server会直接运行，阻塞当前终端
- 一般配置文件都放在/etc/目录下

```
sudo cp /usr/local/redis/redis.conf /etc/redis/

```

- 推荐指定配置文件启动

```
sudo redis-server /etc/redis/redis.conf

```

- 停止redis服务

```
ps ajx|grep redis
sudo kill -9 redis的进程id
```



# 4、String

#### （1）string

- string是redis最基本的类型
- 最大能存储512MB数据
- string类型是二进制安全的，即可以为任何数据，比如数字、图片、序列化对象等

#### （2）命令

##### 设置

- 设置键值

```
set key value
```

- 设置键值及过期时间，以秒为单位

```
SETEX key seconds value
```

- 设置多个键值

```
MSET key value [key value ...]
```

##### 获取

- 根据键获取值，如果不存在此键则返回nil

```
GET key
```

- 根据多个键获取多个值

```
MGET key [key ...]
```

##### 运算

- 要求：值是数字
- 将key对应的value加1

```
INCR key

```

- 将key对应的value加整数

```
INCRBY key increment
```

- 将key对应的value减1

```
DECR key
```

- 将key对应的value减整数

```
DECRBY key decrement
```

##### 其它

- 追加值

```
APPEND key value
```

- 获取值长度

```
STRLEN key
```


# 5、键的命令

- 查找键，参数支持正则

```
KEYS pattern

```

- 判断键是否存在，如果存在返回1，不存在返回0

```
EXISTS key [key ...]

```

- 查看键对应的value的类型

```
TYPE key

```

- 删除键及对应的值

```
DEL key [key ...]

```

- 设置过期时间，以秒为单位
- 创建时没有设置过期时间则一直存在，直到使用使用DEL移除

```
EXPIRE key seconds

```

- 查看有效时间，以秒为单位

```
TTL key
```





# 6、hash

- hash用于存储对象，对象的格式为键值对

#### 设置

- 设置单个属性

```
HSET key field value

```

- 设置多个属性

```
HMSET key field value [field value ...]

```

#### 获取

- 获取一个属性的值

```
HGET key field

```

- 获取多个属性的值

```
HMGET key field [field ...]

```

- 获取所有属性和值

```
HGETALL key

```

- 获取所有的属性

```
HKEYS key

```

- 返回包含属性的个数

```
HLEN key

```

- 获取所有值

```
HVALS key

```

#### 其它

- 判断属性是否存在

```
HEXISTS key field

```

- 删除属性及值

```
HDEL key field [field ...]

```

- 返回值的字符串长度

```
HSTRLEN key field
```



# 7、list

- 列表的元素类型为string
- 按照插入顺序排序
- 在列表的头部或者尾部添加元素

#### 设置

- 在头部插入数据

```
LPUSH key value [value ...]

```

- 在尾部插入数据

```
RPUSH key value [value ...]

```

- 在一个元素的前|后插入新元素

```
LINSERT key BEFORE|AFTER pivot value

```

- 设置指定索引的元素值
- 索引是基于0的下标
- 索引可以是负数，表示偏移量是从list尾部开始计数，如-1表示列表的最后一个元素

```
LSET key index value

```

#### 获取

- 移除并且返回 key 对应的 list 的第一个元素

```
LPOP key

```

- 移除并返回存于 key 的 list 的最后一个元素

```
RPOP key

```

- 返回存储在 key 的列表里指定范围内的元素
- start 和 end 偏移量都是基于0的下标
- 偏移量也可以是负数，表示偏移量是从list尾部开始计数，如-1表示列表的最后一个元素

```
LRANGE key start stop

```

#### 其它

- 裁剪列表，改为原集合的一个子集
- start 和 end 偏移量都是基于0的下标
- 偏移量也可以是负数，表示偏移量是从list尾部开始计数，如-1表示列表的最后一个元素

```
LTRIM key start stop

```

- 返回存储在 key 里的list的长度

```
LLEN key

```

- 返回列表里索引对应的元素

```
LINDEX key index
```



# 8、set

- 无序集合
- 元素为string类型
- 元素具有唯一性，不重复

#### 设置

- 添加元素

```
SADD key member [member ...]

```

#### 获取

- 返回key集合所有的元素

```
SMEMBERS key

```

- 返回集合元素个数

```
SCARD key

```

#### 其它

- 求多个集合的交集

```
SINTER key [key ...]

```

- 求某集合与其它集合的差集

```
SDIFF key [key ...]

```

- 求多个集合的合集

```
SUNION key [key ...]

```

- 判断元素是否在集合中

```
SISMEMBER key member
```



9、zset

- sorted set，有序集合
- 元素为string类型
- 元素具有唯一性，不重复
- 每个元素都会关联一个double类型的score，表示权重，通过权重将元素从小到大排序
- 元素的score可以相同

#### 设置

- 添加

```
ZADD key score member [score member ...]

```

#### 获取

- 返回指定范围内的元素

```
ZRANGE key start stop

```

- 返回元素个数

```
ZCARD key

```

- 返回有序集key中，score值在min和max之间的成员

```
ZCOUNT key min max

```

- 返回有序集key中，成员member的score值

```
ZSCORE key member
```





# 9、高级

#### （1）发布订阅

- 发布者不是计划发送消息给特定的接收者（订阅者），而是发布的消息分到不同的频道，不需要知道什么样的订阅者订阅
- 订阅者对一个或多个频道感兴趣，只需接收感兴趣的消息，不需要知道什么样的发布者发布的
- 发布者和订阅者的解耦合可以带来更大的扩展性和更加动态的网络拓扑
- 客户端发到频道的消息，将会被推送到所有订阅此频道的客户端
- 客户端不需要主动去获取消息，只需要订阅频道，这个频道的内容就会被推送过来

##### 消息的格式

- 推送消息的格式包含三部分
- part1:消息类型，包含三种类型
  - subscribe，表示订阅成功
  - unsubscribe，表示取消订阅成功
  - message，表示其它终端发布消息
- 如果第一部分的值为subscribe，则第二部分是频道，第三部分是现在订阅的频道的数量
- 如果第一部分的值为unsubscribe，则第二部分是频道，第三部分是现在订阅的频道的数量，如果为0则表示当前没有订阅任何频道，当在Pub/Sub以外状态，客户端可以发出任何redis命令
- 如果第一部分的值为message，则第二部分是来源频道的名称，第三部分是消息的内容

##### 命令

- 订阅

```
SUBSCRIBE 频道名称 [频道名称 ...]

```

- 取消订阅
- 如果不写参数，表示取消所有订阅

```
UNSUBSCRIBE 频道名称 [频道名称 ...]

```

- 发布

```
PUBLISH 频道 消息
```

#### （2）主从配置

- 一个master可以拥有多个slave，一个slave又可以拥有多个slave，如此下去，形成了强大的多级服务器集群架构
- 比如，将ip为192.168.1.10的机器作为主服务器，将ip为192.168.1.11的机器作为从服务器
- 设置主服务器的配置

```
bind 192.168.1.10

```

- 设置从服务器的配置
- 注意：在slaveof后面写主机ip，再写端口，而且端口必须写

```
bind 192.168.1.11
slaveof 192.168.1.10 6379

```

- 在master和slave分别执行info命令，查看输出信息
- 在master上写数据

```
set hello world

```

- 在slave上读数据

```
get hello
```



# 10、与Python交互

#### （1）安装包

- 到[中文官网](http://redis.cn/clients.html)查找客户端代码
- 联网安装

```
sudo pip install redis

```

- 使用源码安装

```
unzip redis-py-master.zip
cd redis-py-master
sudo python setup.py install

```

#### （2）交互代码

- 引入模块

```
import redis

```

- 连接

```
try:
    r=redis.StrictRedis(host='localhost',port=6379)
except Exception,e:
    print e.message

```

- 方式一：根据数据类型的不同，调用相应的方法，完成读写
- 更多方法同前面学的命令

```
r.set('name','hello')
r.get('name')
```

- 方式二：pipline
- 缓冲多条命令，然后一次性执行，减少服务器-客户端之间TCP数据库包，从而提高效率

```
pipe = r.pipeline()
pipe.set('name', 'world')
pipe.get('name')
pipe.execute()

```

#### （3）封装

- 连接redis服务器部分是一致的
- 这里将string类型的读写进行封装

```
import redis
class RedisHelper():
    def __init__(self,host='localhost',port=6379):
        self.__redis = redis.StrictRedis(host, port)
    def get(self,key):
        if self.__redis.exists(key):
            return self.__redis.get(key)
        else:
            return ""
    def set(self,key,value):
        self.__redis.set(key,value)
```