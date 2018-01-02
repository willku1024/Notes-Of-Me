- eval(string)，eval可将string转换为express（表达式）
---

- lambda表达式的特殊用法
```python
def test(a,b,func)
    return func(a,b)

func_new = input("请输入一个表达式") #resume func_new: lambda x,y:x+y 
func_new = eval(func_new) #python 3

result = test(3,4,func_new)
```
---
- num += num 和 num = num +num 不同

```python
#不的数据类型结果不同：
#如果是可变类型：字典、列表传引用；基本数据类型传值
a = [100]

def test(num):
    num += num
    num += num + num
    print(num)

test(a)
print(a)



```



