# 编译器的搭建思路
---
## 写在前面
---
*    本项目采用C语言，搭建了支持C0文法的语言编译器。编译后的结果，能够以PASCAL语言的形式解释执行。编译器搭建过程中使用到了`符号表`，`解释器`等核心模块，代码工程量达到三千以上。本项目可以支持多种符合C0文法的语言程序，并且这些语言程序的篇幅量也可以很多（比如一千行代码量等）。
*    本项目在构建过程中参考了两本《编译原理》书籍：龙书和虎书。其中，虎书在PASCAL编译原理的解释方面个人感觉更为全面。因此，在按照符合C0文法的语言模式构建编译器的时候，大都参考着虎书中的原理去设计的。
*    本项目所构建的解释器可以边输出C0文法的运行结果，同时也打印出符合PASCAL语言(`P-CODE`)的语句条目。本项目所构建的解释器可能相比于PASCAL语言编译器有出入，但编译器核心内容依然是不变的。
*    下面将从以下四个部分来对本项目进行介绍。
## 符号表的设计
---
*    符号表的设计是基于一个链表的，由于`符号表`中的内容可以动态的`增加`和`删除`，因此符号表的数组长度设置了一个上限`1000`。
*    考虑到C0文法语言的特点，在符号表中的`kind`类中定义了6种不同的处理类型，它们分别是：`无返回值函数`，`有返回值函数`，`函数参数`，`常量`，`main函数`，`数组`。
*    符号表中也需要记录C0文法语言中不同的变量类型，比如：`int`，`char`，`float`等。这就会使用符号表中的`type`类来进行记录。
*    符号表中同时需要记录符合C0文法语言程序的层数，这是因为我们在撰写C语言程序的时候会考虑`for`循环内部的程序和循环外部的程序的差异性。那么类C文法(`C0文法`)也需要这样做。
*    符号表中最后一个关键的设置参数叫`adr`。设计它的目的在于记录数组的基础地址，同时也能够定位出P-CODE中第几条指令，方便跳转使用。
*    另外在符号表设计过程中还需要一些`数据输入`，`数据查找`接口函数。设计这些函数的目的在于完成符号表动态更新和查找的功能机制。

## 各种编译模块的设计
---
### 因子表达式(factor)的设计
*    因子表达式是编译原理中最底层的设计模块，其中包含`+/-`符号，`整形`，`float`类型，`字符`，`括号`的设计。
*    下面的代码给出了`因子表达式`中有关`整形`的设计程序，其中包含立即数加载，登入符号表等模块的设计。
```C
if (symbol == 24)											//如果是整数
	{
		gen(1, 11, int_num);									//加载立即数（整数）
		printf_flag = 11;										//整数的flag标记
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
	}
```

### 项(item)的设计
*	`项`是基于`因子`来设计的，在项的设计过程中涉及到`乘法(*)`和`除法(/)`的运算。
*	在`项`的设计中，设计思路是在`因子`的基础之上，通过联系运算表达式`+`，`-`，`*`，`/`来构成一个更大的模块。
### 表达式(expression)的设计
`表达式`是包含`项`和`因子`的，表达式构成了一条完整的语句。因而，在表达式设计的过程中应当充分考虑所有`项`和`因子`等情况。

## 报错模块的设计
---
*	在报错模块的设计过程中，需要考虑多种不同的报错类型及其处理机制。下面的程序，列出了常见的报错类型：

## 解释器的设计
---

