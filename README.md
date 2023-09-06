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
### 其他语句模块的设计
除了上面提到的`因子`，`项`，`表达式`的设计之外，还有一些其他的待设计模块。
*	`常量说明(constant_description)`：比如：C0文法中`int`类型的赋值语句`int a=2;`的设计。
*	`变量说明(variable_description)`：比如：C0文法中`char`类型的定义`char a`的设计。
*	`语句(statement)`：符合C0文法的语句设计思路是跟C语言类似的，即包含一条完整的信息。
*	`有返回值的函数定义`：比如处理`return 0`这种的C0文法语言代码。
*	`无返回值的函数定义`：比如处理`return;`这种语言代码。
*	`主程序模块(program)`：在解析任何一份符合C0文法的代码时，都必须先经过这个`主程序模块`。该模块的主要作用是逐条的分析代码，并调用相关的函数（如语句函数）来一步步进行译码。
*	`解释执行模块(interprent)`：该模块实现对符号表中记录的信息进行译码。涉及到进栈和出栈操作，同时也涉及到对一些跳转指令的处理。
## 报错模块的设计
---
*	在报错模块的设计过程中，需要考虑多种不同的报错类型及其处理机制。下面的程序，列出了常见的报错类型：
```C
void error(int flag)
{
	error_count++;
	switch (flag)
	{
	case 0:cout << "program is error" << endl; break;
	case 1:cout << "constant_description is error" << endl; break;
	case 2:cout << "variable_description is error" << endl; break;
	case 3:cout << "return_value_function_definition is error" << endl; break;
	case 4:cout << "no_return_value_function_definition is error" << endl; break;
	case 5:cout << "common2_function is error" << endl; break;
	case 6:cout << "comma_function is error" << endl; break;
	case 7:cout << "fu_he_statement is error" << endl; break;
	case 8:cout << "statement is error" << endl; break;
	case 9:cout << "expression is error" << endl; break;
	case 10:cout << "wu_fu_hao_digital is error" << endl; break;
	case 11:cout << "item is error" << endl; break;
	case 12:cout << "factor is error" << endl; break;
	case 13:cout << "const not followed int|float|char " << endl; break;
	case 14:cout << "缺少标识符" << endl; break;
	case 15:cout << "缺少等于号" << endl; break;
	case 16:cout << "等于号右边不匹配" << endl; break;
	case 17:cout << "结尾处不是分号或者返回值用错地方" << endl; break;
	case 18:cout << "缺少单引号" << endl; break;
	case 19:cout << "不满足匹配字符条件" << endl; break;
	case 20:cout << "类型不匹配,极可能缺少参数" << endl; break;
	case 21:cout << "结尾处不是右圆括号" << endl; break;
	case 22:cout << "数组总大小出错" << endl; break;
	case 23:cout << "结尾处不是右方括号" << endl; break;
	case 24:cout << "缺少左圆括号" << endl; break;
	case 25:cout << "缺少双引号" << endl; break;
	case 26:cout << "不是加法运算符(+|-)" << endl; break;
	case 27:cout << "缺少左大括号" << endl; break;
	case 28:cout << "缺少case" << endl; break;
	case 29:cout << "缺少冒号" << endl; break;
	case 30:cout << "右大括号不匹配" << endl; break;
	case 31:cout << "符号表中没有找到需要用到的元素" << endl; break;
	case 32:cout << "有返回值函数不满足对应左圆括号条件" << endl; break;
	case 33:cout << "应为左方括号" << endl; break;
	case 34:cout << "无效标识符" << endl; break;
	case 35:cout << "0不允许作为数组的上限值" << endl; break;
	case 36:cout << "此处应为关系运算符" << endl; break;
	case 37:cout << "必须是int|float|char|void" << endl; break;
	case 38:cout << "类型不是ident|main" << endl; break;
	case 39:cout << "文件为空" << endl; break;
	case 40:cout << "赋值语句类型左右不匹配" << endl; break;
	case 41:cout << "switch - case类型不匹配" << endl; break;
	case 42:cout << "switch-case语句，条件类型不是int|char" << endl; break;
	case 43:cout << "case 语句条件元素重新定义" << endl; break;
	case 44:cout << "数组元素不是整型" << endl; break;
	case 45:cout << "符号表中出现重复元素，给予报错处理" << endl; break;
	case 46:cout << "条件表达式必须为整型" << endl; break;
	case 47:cout << "缺少return语句" << endl; break;
	case 48:cout << "有返回值函数缺少return（x）" << endl; break;
	case 49:cout << "无返回值函数缺少return；" << endl; break;
	case 50:cout << "数组超过上限值" << endl; break;
	case 51:cout << "条件判断不能是char类型" << endl; break;
	case 52:cout << "条件左右类型不匹配" << endl; break;
	case 53:cout << "缺少可枚举变量" << endl; break;
	case 100:cout << "文件已经读取完毕" << endl; break;
	}
	//return 1;
}
```
在调用报错模块时，通过如下代码来实现：
```C
error(20);
```
## 解释器的设计
---
### 加减乘除
```C
case 10://ADD	加减乘除指令通：需要栈顶与次栈顶元素运算，同时使用完毕后，需要把栈顶元素清空
    run[run_item - 2] = run[run_item - 1] + run[run_item - 2];
    run_item = run_item - 1;
    run[run_item] = 0;
    break;

case 11://SUB	
    run[run_item - 2] = run[run_item - 2] - run[run_item - 1];
    run_item = run_item - 1;
    run[run_item] = 0;
    break;

case 13://MUL
    run[run_item - 2] = run[run_item - 2] * run[run_item - 1];
    run_item = run_item - 1;
    run[run_item] = 0;
    break;

case 14://DIV
    run[run_item - 2] = run[run_item - 2] / run[run_item - 1];
    run_item = run_item - 1;
    run[run_item] = 0;
    break;
```
### 函数调用指令如：CAL
```C
case 23://CAL	函数调用指令
    a = run_item;
    X_a[X_item++] = a;//把运行栈的当前下标放进一个暂存数组中
    run[run_item + 1] = p_list[i].lev;
    run_item = run_item + 2;
    break;
```
通过访问符号表中的`lev`参数，来确定当前程序运行的在`第几层`，从而实现C0文法语言中关于函数`调用代码`的解析过程。

# 写在最后
在编译器的搭建过程中，遇到了不少的问题和debug，在`编译code.app`中给出了可运行的程序，里面包含阶段性的代码注释，感兴趣的读者可以参见！
