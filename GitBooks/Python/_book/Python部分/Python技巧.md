# Python技巧

1、包含工作人员信息的列表如下：
infos= [
{"name":"lao_wang","age":45},
{"name":"xiao_li","age":25},
{"name":"niu_wa","age":17}
]
按照列表中字典的年龄给以下排工作人员排序。

```python
>>> infos.sort(key=lambda x: x[age])
```



2、数组列表 L = [1,-13,43,23,0,333,11.6] 

（1）得到递增排序列表L1（[L保持不变]()） 

```python
>>> import copy
>>> L = copy.deepcopy(L1)
>>> L.sort()
```

（2）得到递减排序列表L2（[L保持不变]()） 

```python
>>> import copy
>>> L = copy.deepcopy(L1)
>>> L.sort(reverse=True)
```



3、列表a=[11, 222, 30, -4, 0]，完成如下操作

（1）倒序输出： a = [0, -4, 30, 222, 11]

```python
>>> a[::-1]
```

（2）截取输出： a =[-4,  30,  222]

```python
>>> a[3:0:-1]
>>> a[-2:-5:-1]
```

（3）指定步长输出：  a = [30, 11]

```python
>>> a[-3::-2]
```



4、请尽可能列举python列表的成员方法，并给出一下列表操作的答案：

（1） a=[1, 2, 3, 4, 5], a[::2]=?, a[-2:] = ?

```python

```

（2） 一行代码实现对列表a中的偶数位置的元素进行加3后求和？

```python
from functools import reduce
L = reduce(lambda x, y: x + y, [j for i, j in enumerate([1, 2, 3, 4]) if i % 2 == 0])
```

（3） 将列表a的元素顺序打乱，再对a进行排序得到列表b，然后把a和b按元素顺序构造一个字典d。

 ```python
import random
origin_list = [1, 2, 3, 4]
shuffle_list = random.sample(origin_list, k=len(origin_list))

# 方法一
new_list1 = dict(zip(origin_list, shuffle_list))
print(new_list1)

# 方法二
new_list2 = dict(map(lambda x,y:[x,y], origin_list, shuffle_list))
print(new_list2)
 ```



5.将列表L = [11, 22, 33, 44, 55, 66, 77, 88]，中的33和44删除。

```python
# 方法一(特殊，因为33,44相邻)
for i in L:
    if i == 33 or i == 44:
        L.remove(i)
        
# 方法二
tmp = list()
for i in L:
    if i != 33 or i != 44:
        tmp.append(i)
L = tmp
```


6.[1,[2,3],[4,[5,6],7]]，把这个列表变成[1,2,3,4,5,6,7]