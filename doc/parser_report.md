### parser



程序接受的输入格式是```.lp```文件，这种格式并没有办法很容易地给单纯形法作爲输入，因此需要一个转换工具。

首先需要定义一些类表示输入信息

异常，如果发先未知的语句会抛出异常，以及原因。

```C++
class ParseException
```

变量的上下界

```C++
class Bounds {
public:
	// lower <= x && x <= upper
	int upper, lower;
};
```

在约束式中的变量，有变量的索引、係数

```C++
class Variable {
public:
	int coefficient;
	size_t index;
};
```

约束，可以分$\leq, \geq, =$三种情况，然后包含一个容器装约束中的变量，还有一个常数。
左右顺序是: $C_1, C_2, ..., C_N \leq \text{Constant}$

```C++
class Condition {
public:
	enum Type { eq, leq, geq };
	Type type;
	std::vector<Variable> variables;
	int constant;
};
```

```Data```包含所有的数据:

- 约束: ```conditions```
- 变量的上下界: ```bounds```
- 变量的索引: ```indices```
- 目标函数: ```function```

```C++
class Data {
private:
	std::vector<Condition> conditions;
	std::vector< std::pair<size_t, Bounds> > bounds;
	std::vector<size_t> indices;
	std::vector<Variable> function;
};
```



虽然输入类似```C```的风格，是个上下文无关文法，但是爲了方便就简化为正则语言(应该不会出现非正则的情况)。



#### 预处理

- 移除注释 ```/\\*(.|\n)*?\\*/```
- 移除多馀空白```^\\s*$```
- 移除换行，用空白取代```[\n\r]```

预处理之后，整个输入就可以当作一行，然后用```;```符号当作真正的换行重新分行，一行一行处理

对于每一行，可以分成几种类型

- ```int``` 定义变量
- ```max```, ```min``` 
- 约束

#### 优化

对于约束如果变量只有1个的情况，可以当作该变量的上下界，因爲我们实现的是分支定界法，所以这些信息可以对算法效率有帮助。

值得注意的事情是如果变量的係数是负数，大于、小于要交换。


#### 工具函数



```C
std::vector<std::string> Data::Split(const std::string & input, char delim);
std::string Data::Join(const std::vector<std::string> & input);
```



```a;b; c```或```a, b,\nc,d```这类代表多个元素合在一个字符串上的形式，因爲比较複杂，需要合并、分割这两种功能来实现分开。



