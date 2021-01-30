#include<iostream>
#include<stdio.h>
#include <fstream>
#include<string.h>
#include<stdlib.h>
#include<iomanip>
using namespace std;


/*中间代码部分*/
struct P_CODE
{
	int op_num;
	int lev;
	int x_int;
	float y_float;
	char z_char[100];
};
P_CODE p_list[10000];
int p_list_item = 0;//P_CODE代码索引数组下标
					/*符号表部分*/
union FU_HAO_BIAO_COMBINE
{
	int INT_num;
	float FLOAT_num;
	int ASCLL_num;
};
struct FU_HAO_BIAO
{
	char *name;
	int kind;////////0:变量...1:无返回值函数...2:有返回值函数...3:函数参数...4:常量...5:main函数...6:数组
	int type;
	int lev;
	int adr;
	FU_HAO_BIAO_COMBINE com_bine;
};
FU_HAO_BIAO FU_list[1000];//符号表结构数组大小为100
int FU_item = 1;//符号表的下表从1开始计数

int VOID_FLAG = 0;//标记是无返回值函数
int VOID_SYM = 0;//标记无返回值函数
int FLAG_add_sub = 0;//switch中标记+、-号

int getsym_count = 0;//词法分析标记，第二次不再次从文件中读取内容
int OPP[1000] = { 0 };//switch中存放每个分语句（case）当前需要跳出本层的位置数组
int op_item = 0;//OPP计数下标变量

int DEFAULT = 0;

int x = 0;//标记有返回值return(1)语句出现
int y = 0;//标记无返回值语句return;出现
int RET_FLAG;//标记是否从符号表返回值

int SBSB = 0;

int X_a[100];//最大调用层数为100（MKF\CAL存放a的数值只能是100个a）
int X_item = 0;//存放a的数组下标

int para_value;//他们共同标记参数的类型值，用在statement语句中
int para_value2;
int para_value_shuzu[100];//允许100个函数参数调用
int pv_item = 0;//数组下标

int hang_number = 1;//对行号进行计数
int symbol_array[10000] = { 0 };//存放symbol的标号数组
int symarr = 0;//该数组下标

int printf_flag = 11;//谁的printf_flag小，就转换成谁的类型
int printf_flag2 = 0;
int printf_flag3 = 0;
int printf_flag4 = 11;
int printf_flag_judge;//switch case judge
int FLAG_IN_CONST = 0;//标记常量说明中的+\-
int FLAG_IN_CONST2 = 0;
int FLAG_ADD_SUB = 0;//标记switch语句中的+\-号

int judge_if1 = 0;//judge the type(from the condition) is or not same(point to if)
int judge_if2 = 0;

int judge_while1 = 0;//judge the type(from the condition)is or not same(point to while)
int judge_while2 = 0;
int FLAG_JUMP_MAIN = 0;//标记main函数，针对main函数中的return情况
int printf_flag2_use = 0;
int printf_flag3_use = 0;
int temp_ARRAY_count = 0;
int error_count = 0;

int fu_he_flag = 0;//just like the key 

char switch_case[100];//存放switch-case的case行数
int sc_item = 0;

/*下面一些变量用在字符以及字符串中*/
char zi_fu;//目的记录字符
char zi_fu_two;
char zi_fu_three;
int ascll_num;//目的记录字符的ascll码
int ascll_num_two;
int ascll_num_three;

int SYMBOL_FLAG_W = 0;//记录被更新前的symbol值，用在处理语法冲突中
int OP[10000] = { 0 };//op_number记录数组

int printf_sc = 0;//switch-case语句标记，判断类型是否一致
int start;
int LEV_count_list[10];//最大层数存放数组，下标为层次，其实就两层，内容为该层对应的变量数
int op_number = 0;//指令数
int lev = 1;

/*以下涉及到一些字符以及字符串的操作*/
char print_f[100] = { 0 };//用在case语句以及printf语句中，需要进行单一字符读取的存取数组
int printf_item = 0;//printf_item
char Print_F[100] = { 0 };
int Print_f = 0;

int int_num = 0;//记录整数的数值
float real_num = 0;//记录实数的数值
int fu_int_num;//数组的时候需要一个int_num的复制版

double run[40000];//运行栈存放数组
int btab[100];//分程序索引数组，最多允许100个函数同时存在
int run_item = 0, btab_item = 0;

/*cattoken数组及下标*/
char number_ident[21] = { 0 };//标志符不准超过20个
static int number_ident_item = 0;

/*字符串传递*/
char CHUAN[30000][21] = { 0 };
static int CHUAN_item = 0;

/*一些跳出特定的程序的关键flag*/
//int flag, flag_flag, flag_flag_flag, flag_main, flag_main_two = 0;
int flag = 0;
int flag_flag = 0;
int flag_flag_flag = 0;
int flag_main = 0;
int flag_main_two = 0;
/*词法分析产生的结果，供语法分析使用*/
int symbol;

string filename;
//char ssym[14] = { '(',')','{','}',',',';',':','+','-','*','/','.','[',']' };

/*文件名存放数组*/
char token2[20] = { 0 };//文件名只允许长度为20个

						/*词法分析实现一个字符一个字符读取，用于字符和字符串的判断*/
char ch = '\0';

/*c0-文法关键字数组*/
string keyword[16] = { " ","const","int","float","char","void","main","if","else","switch","case","default","while",
"return","scanf","printf" };

/*c0-文法一些关键字和符号*/
string keyword_za[41] = { " ","const","int","float","char","void","main","if","else","switch","case","default","while",
"return","scanf","printf","(",")","{","}",",","=",";"," "," "," ","<",">","<=",">=","==","!=",
":","+","-","*","/",".","[","]"," " };

/*记忆符号*/
string keyword_za_sym[41] = { " ","const_sym","int_sym","float_sym","char_sym","void_sym","main_sym","if_sym","else_sym",
"switch_sym","case_sym","default_sym","while_sym","return_sym","scanf_sym","printf_sym","lp","rp",
"lb","rb","comma","equal","semi","ident","digit","string","small","big","small_equal","big_equal",
"equal_equal","not_equal","colon","add","sub","mult","div","spot","lff","rff","real" };


/*词法分析器的主要函数部分*/

/*查找保留字函数*/
int reserver(string str);

/*转换int类型函数*/
int transNum();

/*清空函数，每次词法分析都得clear()掉上一个词法附加产生的一些字符串*/
void clear(void);

/*词法分析关键函数*/
int getsym(int flag);


/*语法分析器的主要函数部分*/

/*错误处理函数*/
void error(int aa);



/*相关函数的声明部分*/
int interprent();
void get_out_of_FU_list();
void CLEAR_C();
void CLEAR();
void FUZHI();
int gen(int op_num, int lev, int x_int);
int gen(int op_num, int lev, float y_float);
int gen(int op_num, int lev, char *z_char);
int RE_find(int s_select);
void trans_catToken();
int find(int select_s);
void fuzhi();
int enter(int lev, int type, int kind, int whichcount, int num_num);
bool factor(int typ);
bool item(int typ);
bool expression(int typ);
bool wu_fu_hao_digital(int typ);
bool constant_description(int typ);
bool variable_description(int typ);
bool statement(int typ);

bool paralist(int typ);
bool return_no_return(int typ);
bool return_value_function_definition(int typ);
bool no_return_value_function_definition(int typ);
bool fu_he_statement(int typ);

bool return_no_void_tance(int typ);
bool no_return_void_tance(int typ);
bool main_function(int typ);
bool program(int typ);


/*词法分析器的主要判断bool类型函数*/
bool isSpace(char a) { return (a == ' ') ? true : false; }
bool isTab(char a) { return (a == '\t') ? true : false; }
bool isNewline(char a) { return (a == '\n') ? true : false; }
bool isExc(char a) { return (a == '\r') ? true : false; }
bool isLetter(char a) { return ((a >= 'a'&&a <= 'z') || (a >= 'A'&&a <= 'Z') || (a == '_')) ? true : false; }
bool isDigit(char a) { return (a >= '0'&&a <= '9') ? true : false; }
void catToken(char a) { number_ident[number_ident_item++] = a; }
bool isLss(char a) { return (a == '<') ? true : false; }
bool isEqu(char a) { return (a == '=') ? true : false; }
bool isGtr(char a) { return (a == '>') ? true : false; }
bool isEqual(char a) { return (a == '=') ? true : false; }
bool isEm(char a) { return (a == '!') ? true : false; }
bool isPoint(char a) { return (a == '.') ? true : false; }

void get_out_of_FU_list()
{
	int i;
	for (i = FU_item - 1; i > 0; i--)
	{
		LEV_count_list[lev] = 0;
		if (FU_list[i].kind == 3 || FU_list[i].kind == 1 || FU_list[i].kind == 2)
		{
			break;
		}
		if (FU_list[i].kind == 0 || FU_list[i].kind == 4)
		{
			FU_item = FU_item - 1;
		}
	}
}
void CLEAR_C()
{
	memset(print_f, 0, 100 * sizeof(char));
	printf_item = 0;
}
void CLEAR()
{
	memset(Print_F, 0, 100 * sizeof(char));
	Print_f = 0;
}
void FUZHI()
{
	int i = 0;
	for (i = 0; i < printf_item - 1; i++)
	{
		Print_F[i] = print_f[i];
	}
	Print_f = i;
}

int gen(int op_num, int lev, int x_int)//对于整数操作
{
	if (op_number < 10000)
	{
		p_list[op_number].op_num = op_num;
		p_list[op_number].lev = lev;
		p_list[op_number].x_int = x_int;
		op_number = op_number + 1;
	}
	else
	{
		cout << "gen out of range should be error" << endl;
		return 1;
	}
	return 0;
}

int gen(int op_num, int lev, float y_float)//对于实数操作
{
	if (op_number < 10000)
	{
		p_list[op_number].op_num = op_num;
		p_list[op_number].lev = lev;
		p_list[op_number].y_float = y_float;
		op_number = op_number + 1;
	}
	else
	{
		cout << "gen out of range should be error" << endl;
		return 1;
	}
	return 0;
}

int gen(int op_num, int lev, char *z_char)//对输出字符串的操作
{
	if (op_number < 10000)
	{
		p_list[op_number].op_num = op_num;
		p_list[op_number].lev = lev;
		strcpy(p_list[op_number].z_char, z_char);
		op_number = op_number + 1;
	}
	else
	{
		cout << "gen out of range should be error" << endl;
		return 1;
	}
	return 0;
}
int RE_find(int s_select)
{
	int iii = 0;
	int jjj = 1;
	if (FU_item == 1)
	{
		return 0;
	}

	for (iii = FU_item - 1; iii > 0; iii--)
	{
		if (FU_list[iii].lev < lev)
		{
			jjj = 0;
		}
		if (jjj)
		{
			if (!(strcmp(FU_list[iii].name, CHUAN[s_select])))
			{
				return iii;
			}
		}
	}
	return 0;
}



/*查找符号表*/
int find(int select_s)
{
	int ii = 0;
	int jj = 1;
	for (ii = FU_item - 1; ii > 0; ii--)
	{
		if (FU_list[ii].lev < lev) { jj = 0; }
		if (FU_list[ii].kind != 3 || jj)//如果不是参数或者不是在上一层，就可以查看符号表
		{
			if (!strcmp(FU_list[ii].name, CHUAN[select_s])) { return ii; }
		}
	}
	return 0;
}

/*登录符号表*/
int enter(int lev, int type, int kind, int whichcount, int num_num)
{//这里需要加上符号表最大值得判断error情况
 //int temp_ARRAY_count = 0;
	if (FU_item < 1000)
	{

		FU_list[FU_item].lev = lev;
		FU_list[FU_item].type = type;
		FU_list[FU_item].kind = kind;
		FU_list[FU_item].name = CHUAN[whichcount];
		if (kind == 0 || kind == 3)//如果是变量或者参数
		{

			LEV_count_list[lev] = LEV_count_list[lev] + 1;
			FU_list[FU_item].adr = LEV_count_list[lev];//该层对应的变量数+1
		}
		if (kind == 6)//数组通用kind
		{
			if (LEV_count_list[lev] == 0)
			{
				temp_ARRAY_count = 0;
			}
			LEV_count_list[lev] = LEV_count_list[lev] + 1;
			temp_ARRAY_count = temp_ARRAY_count + 1;
			FU_list[FU_item].adr = temp_ARRAY_count * 200;
			//temp_ARRAY_count = LEV_count_list[lev]*100;
			//FU_list[FU_item].adr = temp_ARRAY_count;//记录数组的基础地址
			if (type == 11 || type == 12)//如果是整型或者字符型数组,这里登录的是数组的大小
			{
				FU_list[FU_item].com_bine.INT_num = num_num;//6,11,xx将数组的大小传入符号表对应区域
			}
		}
		if (kind == 1 || kind == 2 || kind == 5)
		{
			FU_list[FU_item].adr = op_number;//无返回值函数名，有返回值函数名，主函数名，ADR代表P_CODE的第几条指令，方便跳转
		}
		if (kind == 4)//如果为常亮
		{
			if (type == 11)//如果为int 类型
			{
				if (num_num)//如果是正数
				{
					FU_list[FU_item].com_bine.INT_num = int_num;
				}
				else//如果是负数
				{
					FU_list[FU_item].com_bine.INT_num = 0 - int_num;
				}
			}
			else if (type == 12)//如果为char 类型
			{

				FU_list[FU_item].com_bine.ASCLL_num = ascll_num;//需要把字符转换成ascii码形式，这个后面进行处理?????????????????????????????????????????
			}
			else if (type == 9)//如果为float 类型
			{

				if (num_num)//如果是正数
				{
					FU_list[FU_item].com_bine.FLOAT_num = real_num;
				}
				else//如果是负数
				{
					FU_list[FU_item].com_bine.FLOAT_num = 0 - real_num;
				}
			}
		}
		FU_item = FU_item + 1;
		//cout << "name:" << FU_list[FU_item - 1].name << "	" << "lev:" << FU_list[FU_item - 1].lev << "	" << "type:" << FU_list[FU_item - 1].type << "	" << "kind:" << FU_list[FU_item - 1].kind << "	" << "adr:" << FU_list[FU_item - 1].adr << endl;
	}
	else
	{
		cout << "符号表超上限" << endl;
	}
	return 0;
}

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
/*主要函数的实现部分(包括词法分析器和语法分析器)*/
int reserver(string str)
{
	int i = 0;
	for (i = 0; i < 16; i++)
	{
		if (str == keyword[i]) { return i; }
	}
	return 0;/////////////////////////待优化
}
/*字符函数转换成int类型*/
int transNum()
{
	return (atoi(number_ident));
}

/*需要将cattoken转换成全部小写字母*/
void trans_catToken()
{
	strlwr(number_ident);
}
void fuzhi()
{
	for (number_ident_item = 0; number_ident_item < 20; number_ident_item++)
	{
		CHUAN[CHUAN_item][number_ident_item] = number_ident[number_ident_item];
	}
	CHUAN_item = CHUAN_item + 1;
}
void clear(void)
{
	/*for (number_ident_item = 0; number_ident_item<20; number_ident_item++)
	{
	number_ident[number_ident_item] = '\0';
	}
	number_ident_item = 0;*/
	memset(number_ident, 0, 20 * sizeof(char));
	number_ident_item = 0;
}
int getsym(int flag)
{
	int_num = 0;
	real_num = 0;
	double t = 0.0;
	int tt = 1;
	static char data[300000] = { 0 };
	static int data_item = -1;/////////针对真指针的
	static int j = -1;/////////////////针对假指针的
					  //static int getsym_count = 0;
	symbol = 0;
	if (getsym_count == 0)
	{
		data_item = 0;
		FILE *fp;
		//fp = fopen("C:/Users/dalao/Documents/Visual Studio 2015/Projects/16182685_田兵弟_语法分析/16182685_test.txt", "r");
		fp = fopen(token2, "r");
		if (fp == NULL) { error(39); }
		do
		{
			ch = fgetc(fp);
			if (ch == EOF) { break; }
			data[data_item++] = ch;
		} while (1);
		fclose(fp);
		data_item = -1;
		getsym_count = getsym_count + 1;
	}
	if (flag == 1) { j = data_item; }//////////flag = 1 代表着要把真指针的值赋给假指针
	if (flag == 2)//////////flag = 2 代表着要让假指针进行往前探测，这是真指针的值保持不变，仅仅让假指针移动
	{
		clear();
		j = j + 1;
		ch = data[j];
		while (isSpace(ch) || isTab(ch) || isNewline(ch) || isExc(ch))
		{
			j = j + 1;
			ch = data[j];
		}
		if (ch == '\0')
		{
			cout << "finish" << endl;
			return 0;///////////如果假指针探测到文件末尾，那么应当结束探测，当然这种情况发生的概率很低，具体需要以后优化
		}
		if (isLetter(ch))////是字母 
		{
			while (isLetter(ch) || isDigit(ch))////是字母或者数字 
			{
				catToken(ch);//将字母和数字组成的字符串放入数组 
				j = j + 1;
				ch = data[j];
			}
			trans_catToken();
			j = j - 1;
			int resultValue;
			resultValue = reserver(number_ident);//判断关键字 
			if (resultValue == 0) { symbol = 23; }
			else { symbol = resultValue; }
		}
		else if (isDigit(ch))
		{
			//tt = 1;
			while (isDigit(ch))
			{
				catToken(ch);
				//int_num = int_num + (int)(ch - '0')*tt;
				//tt = tt * 10;
				j = j + 1;
				ch = data[j];
			}
			j = j - 1;

			j = j + 1;
			ch = data[j];
			int_num = transNum();
			if (isPoint(ch))
			{
				t = 0.1;
				real_num = int_num;
				catToken(ch);
				j = j + 1;
				ch = data[j];
				if (isDigit(ch))
				{
					while (isDigit(ch))
					{
						catToken(ch);
						real_num = real_num + (int)(ch - '0')*t;
						t = t / 10;
						j = j + 1;
						ch = data[j];
					}
					j = j - 1;
					symbol = 40;
				}
				else { j = j - 2; }
			}
			else
			{
				j = j - 1;
				symbol = 24;//是整数数字，不是实数
			}
		}
		else if (isLss(ch))
		{
			j = j + 1;
			ch = data[j];
			if (isEqu(ch)) { symbol = 28; }
			else
			{
				j = j - 1;
				symbol = 26;////小于号
			}
		}
		else if (isGtr(ch))
		{
			j = j + 1;
			ch = data[j];
			if (isEqu(ch)) { symbol = 29; }
			else
			{
				j = j - 1;
				symbol = 27;////大于号
			}
		}
		else if (isEqual(ch))
		{
			j = j + 1;
			ch = data[j];
			if (isEqual(ch)) { symbol = 30; }
			else
			{
				j = j - 1;
				symbol = 21;////等于号
			}
		}
		else if (isEm(ch))
		{
			j = j + 1;
			ch = data[j];
			if (isEqual(ch)) { symbol = 31; }
			else { j = j - 1; }
		}
		else if (ch == '+') { symbol = 33; }
		else if (ch == '-') { symbol = 34; }
		else if (ch == '*') { symbol = 35; }
		else if (ch == '/') { symbol = 36; }
		else if (ch == ',') { symbol = 20; }
		else if (ch == ';') { symbol = 22; }
		else if (ch == ':') { symbol = 32; }
		else if (ch == '(') { symbol = 16; }
		else if (ch == ')') { symbol = 17; }
		else if (ch == '{') { symbol = 18; }
		else if (ch == '}') { symbol = 19; }
		else if (ch == '[') { symbol = 38; }
		else if (ch == ']') { symbol = 39; }
		else if (ch == '.') { symbol = 37; }
		else if (ch == '"') { symbol = 200; }
		else if (ch == 39) { symbol = 100; }
		else { symbol = 0; }
	}
	if (flag == 3)///////////////flag = 3 代表着进行字符串分析的时候，需要读取单一的字符（跟探测指针无关）
	{
		data_item = data_item + 1;//
		ch = data[data_item];
		return ch;
	}
	if (flag == 4) { data_item = data_item - 1; }//语法分析中，关于字符和字符串分析，while 循环回退一个字符

	if (flag == 0)
	{
		clear();
		data_item = data_item + 1;//
		ch = data[data_item];

		while (isSpace(ch) || isTab(ch) || isNewline(ch) || isExc(ch))
		{
			if (isNewline(ch))
			{
				hang_number += 1;
				cout << hang_number << endl;

			}
			data_item = data_item + 1;
			ch = data[data_item];

		}
		if (ch == '\0')
		{
			cout << "finish" << endl;
			return 0;
		}
		if (isLetter(ch))////是字母 
		{
			while (isLetter(ch) || isDigit(ch))////是字母或者数字 
			{
				catToken(ch);//将字母和数字组成的字符串放入数组 
				data_item = data_item + 1;
				ch = data[data_item];
			}
			trans_catToken();
			fuzhi();
			data_item = data_item - 1;
			int resultValue;
			resultValue = reserver(number_ident);//判断关键字 
			if (resultValue == 0) { symbol = 23; }
			else { symbol = resultValue; }
		}
		else if (isDigit(ch))
		{
			//tt = 1;
			while (isDigit(ch))
			{

				catToken(ch);
				//int_num = int_num + (int)(ch - '0')*tt;
				//tt = tt * 10;
				data_item = data_item + 1;
				ch = data[data_item];
			}
			data_item = data_item - 1;

			data_item = data_item + 1;
			ch = data[data_item];
			int_num = transNum();
			if (isPoint(ch))
			{
				t = 0.1;
				real_num = int_num;
				catToken(ch);
				data_item = data_item + 1;
				ch = data[data_item];
				if (isDigit(ch))
				{
					while (isDigit(ch))
					{
						catToken(ch);
						real_num = real_num + (int)(ch - '0')*t;
						t = t / 10;
						data_item = data_item + 1;
						ch = data[data_item];
					}
					data_item = data_item - 1;
					symbol = 40;
				}
				else { data_item = data_item - 2; }
			}
			else
			{
				data_item = data_item - 1;
				symbol = 24;
			}
		}
		else if (isLss(ch))
		{
			data_item = data_item + 1;
			ch = data[data_item];
			if (isEqu(ch)) { symbol = 28; }
			else
			{
				data_item = data_item - 1;
				symbol = 26;////小于号
			}
		}
		else if (isGtr(ch))
		{
			data_item = data_item + 1;
			ch = data[data_item];
			if (isEqu(ch)) { symbol = 29; }
			else
			{
				data_item = data_item - 1;
				symbol = 27;////大于号
			}
		}
		else if (isEqual(ch))
		{
			data_item = data_item + 1;
			ch = data[data_item];
			if (isEqual(ch)) { symbol = 30; }
			else
			{
				data_item = data_item - 1;
				symbol = 21;////等于号
			}
		}
		else if (isEm(ch))
		{
			data_item = data_item + 1;
			ch = data[data_item];
			if (isEqual(ch)) { symbol = 31; }
			else { data_item = data_item - 1; }
		}
		else if (ch == '+') { symbol = 33; }
		else if (ch == '-') { symbol = 34; }
		else if (ch == '*') { symbol = 35; }
		else if (ch == '/') { symbol = 36; }
		else if (ch == ',') { symbol = 20; }
		else if (ch == ';') { symbol = 22; }
		else if (ch == ':') { symbol = 32; }
		else if (ch == '(') { symbol = 16; }
		else if (ch == ')') { symbol = 17; }
		else if (ch == '{') { symbol = 18; }
		else if (ch == '}') { symbol = 19; }
		else if (ch == '[') { symbol = 38; }
		else if (ch == ']') { symbol = 39; }
		else if (ch == '.') { symbol = 37; }
		else if (ch == '"') { symbol = 200; }
		else if (ch == 39) { symbol = 100; }
		else { symbol = 0; }
	}
	return 1;
}

bool factor(int typ)//在因子中（表达式中），你只需要算出数组的值就可以了
{
	int iiii = 0;//查询符号表变量
	int ret_ret = 0;//返回值变量
	int op_number4;
	bool anti2 = false;
	if (symbol == 33 || symbol == 34)//如果是+、-
	{
		if (symbol == 34) { anti2 = true; }
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
	}
	if (symbol == 24)											//如果是整数
	{
		//if (para_value2 != 0 && para_value2 != 11) { cout << "参数类型不匹配" << endl; }
		gen(1, 11, int_num);									//加载立即数（整数）
		printf_flag = 11;										//整数的flag标记
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
	}
	else if (symbol == 40)										//如果是float类型
	{
		//if (para_value2 != 0 && para_value2 != 9) { cout << "参数类型不匹配" << endl; }
		gen(1, 9, real_num);									//加载float数				
		printf_flag = 9;										//float-flag标记
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
	}
	else if (symbol == 100)//字符
	{
		getsym(3);//读取单一字符
		if (ch == '_' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || (ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z') || (ch >= '0'&&ch <= '9'))
		{
			zi_fu_two = ch;											//记录当前读取字符
			ascll_num_two = (int)zi_fu_two;							//将字符转换成ascll码
			getsym(3);												//继续读取一个单一字符
			if (ch == '\'')											//如果是单引号，则结束
			{
				//if (para_value2 != 0 && para_value2 != 12) { cout << "参数类型不匹配" << endl; }
				gen(1, 12, zi_fu_two);								//加载立即数（字符的ascll码）
				printf_flag = 12;									//字符的flag标记
				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
			}
			else { error(18); }
		}
		else { error(19); }
	}
	else if (symbol == 23)//ident
	{
		ret_ret = find(CHUAN_item - 1);						//查询符号表，找到该标识符对应符号表位置，并返回。
		if (ret_ret == 0) { error(31); }
		if (FU_list[ret_ret].kind == 4)						//如果是常量
		{
			if (FU_list[ret_ret].type == 11)				//如果是整形
			{
				if (gen(1, FU_list[ret_ret].type, FU_list[ret_ret].com_bine.INT_num)) { return 1; }
				printf_flag = 11;							//整型flag标记
			}
			if (FU_list[ret_ret].type == 12)				//如果是char类型
			{
				if (gen(1, FU_list[ret_ret].type, FU_list[ret_ret].com_bine.ASCLL_num)) { return 1; }
				printf_flag = 12;							//字符型flag标记
			}
			if (FU_list[ret_ret].type == 9)					//如果是float类型
			{
				if (gen(1, FU_list[ret_ret].type, FU_list[ret_ret].com_bine.FLOAT_num)) { return 1; }
				printf_flag = 9;							//float类型flag标记
			}
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
		}
		else if (FU_list[ret_ret].kind == 3 || FU_list[ret_ret].kind == 0)//如果是参数
		{
			//if ((para_value != 0) && (para_value != FU_list[ret_ret].type)) { cout << "参数类型不匹配" << endl; }
			if (gen(0, FU_list[ret_ret].lev, FU_list[ret_ret].adr)) { return 1; }
			printf_flag = FU_list[ret_ret].type;
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
		}
		else if (FU_list[ret_ret].kind == 2)//走有返回值函数定义部分，也就是读到了函数名部分
		{

			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			if (symbol == 16)												//左边圆括号
			{
				//函数调用指令，type代表函数的类型
				if (gen(23, FU_list[ret_ret].type, 0)) { return 1; }
				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
				for (iiii = ret_ret + 1; iiii < 100; iiii++)//从当前元素开始继续查表，因为参数声明事先已经登录完毕
				{
					if (FU_list[iiii].kind != 3) { break; }//不是函数参数，直接跳出循环
					para_value2 = FU_list[iiii].type;

					if (expression(typ))//如果是函数参数，继续执行循环，进入表达式部分，检查参数类型
					{
						//表达式出来如果是逗号，说明参数还没有判断完毕，继续循环。
						SBSB = 0;
						if (para_value2 != 0 && para_value2 != printf_flag)
						{
							cout << "paralist type is error!" << endl;
						}


						if (symbol == 20)
						{
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
						}
						else { break; }
					}
					else { return false; }
				}
				para_value2 = 0;
				if ((FU_list[iiii].kind != 3 && symbol != 17)) //|| (FU_list[iiii + 1].kind == 3 && symbol == 17))
				{
					cout << "the paralist transmit is error!" << endl;
				}

				if (gen(24, 0, FU_list[ret_ret].adr)) { return 1; }
				if (symbol == 17)
				{
					printf_flag = FU_list[ret_ret].type;
					if (!getsym(0)) { return false; }
					symbol_array[symarr++] = symbol;
				}
				else { return false; }
			}
			else { error(32); }
		}
		else if (FU_list[ret_ret].kind == 6)
		{
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			if (symbol == 38)
			{
				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
				if (expression(typ))
				{
					//printf_flag = FU_list[ret_ret].type;
					SBSB = 0;
					if (printf_flag != 11) { error(44); }
					if (gen(33, 0, FU_list[ret_ret].com_bine.INT_num)) { return 1; }//第三个参数代表数组的大小
					if (gen(31, 0, 0)) { return 1; }//数组顶层元素与次栈顶比较，察看是否越界
					op_number4 = op_number;
					if (gen(39, 0, 0)) { return 1; }
					p_list[op_number4].x_int = op_number;
					//这里应该加一条指令，用以判断数组是否越界，如果数组越界，那么JUB指令的equ参数决定了
					//此处指令的执行情况。如果equ=1,说明可以通行，也就是跳转到报错的行的下一条继续执行。
					//而这条指令本身的目的是为了进行报错使用.
					if (gen(30, 0, 0)) { return 1; }//计算相对地址，对因子来讲是main_address，暂存相对偏移地址

					if (gen(35, FU_list[ret_ret].lev, FU_list[ret_ret].adr)) { return 1; }//LDQ,取值放栈顶

					if (symbol == 39)
					{
						printf_flag = FU_list[ret_ret].type;
						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
					}
					else { error(23); }
				}
				else { return false; }
			}
			else { error(33); }
		}
		//else if (FU_list[ret_ret].kind == 1) { cout << "FU_list[ret_ret].kind is:" << FU_list[ret_ret].kind << endl; }
		else { error(34); }
	}
	else if (symbol == 16)//(
	{
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
		if (expression(typ))
		{
			SBSB = 0;
			if (symbol == 17)
			{
				symbol_array[symarr++] = symbol;
				if (printf_flag == 12)//char
				{
					printf_flag = 11;
				}
				//printf_flag = 11;
				//cout << "printf_flag is"<<printf_flag << endl;
				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
			}
			else { error(21); }
		}
		else { return false; }
	}

	else { error(20); }
	if (anti2)
	{
		if (gen(15, 0, 0)) { return 1; }
	}
	//cout << "factor success" << endl;
	return true;
}
bool item(int typ)
{
	int f_f = 0;
	if (factor(typ))
	{
		if (printf_flag2_use == 0)
		{
			printf_flag2 = printf_flag;
			printf_flag2_use = 1;
		}
		while (symbol == 35 || symbol == 36)/*如果是乘除*/
		{

			if (symbol == 36)
			{

				/*printf_flag2 = 9;
				//printf_flag2 = printf_flag;
				printf_flag2_use = 1;*/


				SBSB = 9;
				
			}
			else
			{
				printf_flag2 = printf_flag;
				if (printf_flag2 == 12)
				{
					printf_flag2 == 11;
				}
				printf_flag2_use = 1;
			}
			//printf_flag2 = printf_flag;
			if (symbol == 35) { f_f = 13; }
			else if (symbol == 36) { f_f = 14; }
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			if (factor(typ))
			{
				if (printf_flag3_use == 0)
				{
					printf_flag3 = printf_flag;
					printf_flag3_use = 1;
				}
				//这里需要考虑输出表达式的float类型，不仅仅是int类型的
				if (printf_flag2 > printf_flag3)
				{
					printf_flag2_use = 0;
					printf_flag3_use = 0;
					printf_flag = printf_flag3;
					
				}
				else if (printf_flag2 == 12 && printf_flag3 == 12)//如果两者都是char类型，那么就应该按照int类型计算
				{
					printf_flag2_use = 0;
					printf_flag3_use = 0;
					printf_flag = 11;
					
				}
				
				//如果两者类型相同，并且都是整型则转成字符型
				else
				{
					printf_flag2_use = 0;
					printf_flag3_use = 0;
					printf_flag = printf_flag2;
					
				}
				if (SBSB == 9)
				{
					printf_flag = 9;
				}
				if (gen(f_f, 0, 0)) { return 1; }
				if (f_f == 13) { ; }
				else if (f_f == 14) { ; }
			}
			else { return false; }
		}
		printf_flag2_use = 0;
		printf_flag3_use = 0;
	}
	else { return false; }
	return true;
}
bool expression(int typ)
{
	int f_f_f = 0;
	bool anti = false;
	if (symbol == 33 || symbol == 34)
	{
		if (symbol == 34) { anti = true; }
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
	}
	if (item(typ))
	{
		if (printf_flag2_use == 0)
		{
			printf_flag2 = printf_flag;
			printf_flag2_use = 1;
		}
		//如果是减号
		if (anti)//如果是减号
		{
			//是负数
			if (gen(15, 0, 0)) { return 1; }//gen(11,11,0)

		}
		while (symbol == 33 || symbol == 34)
		{

			printf_flag2 = printf_flag;
			if (printf_flag2 == 12)
			{
				printf_flag2 = 11;
			}
			printf_flag2_use = 1;
			//如果是加号
			if (symbol == 33) { f_f_f = 10; }
			//如果是减号
			else if (symbol == 34) { f_f_f = 11; }
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			if (item(typ))
			{
				if (printf_flag3_use == 0)
				{
					printf_flag3 = printf_flag;
					printf_flag3_use = 1;
				}
				if (printf_flag2 > printf_flag3)
				{
					
					printf_flag2_use = 0;
					printf_flag3_use = 0;
					printf_flag = printf_flag3;
					
				}
				else if (printf_flag2 == 12 && printf_flag3 == 12)//如果两者都是char类型，那么就应该按照int类型计算
				{
					printf_flag2_use = 0;
					printf_flag3_use = 0;
					printf_flag = 11;
					
				}
				//如果两者类型相同，并且都是整型则转成字符型
				else
				{
					printf_flag2_use = 0;
					printf_flag3_use = 0;
					printf_flag = printf_flag2;
					
				}
				if (SBSB == 9)
				{
					printf_flag = 9;
				}
				//这里需要考虑输出表达式的float类型，不仅仅是int类型的
				if (gen(f_f_f, 0, 0)) { return 1; }
				//SB_flag = 1;
				//if (gen(40, 0, 0)) { return 1; }
				if (f_f_f == 10) { ; }
				else if (f_f_f == 11) { ; }
			}
			else { return false; }
		}
		printf_flag2_use = 0;
		printf_flag3_use = 0;
	}
	else { return false; }
	return true;
}
bool wu_fu_hao_digital(int typ)
{
	if (symbol == 24)
	{

		if (int_num == 0) { error(35); }
	}
	else { error(22); }
	return true;
}
bool constant_description(int typ)//常量说明
{
	int type = 0;
	if (symbol == 2 || symbol == 3 || symbol == 4)
	{
		if (symbol == 2)//int
		{
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			while (1)
			{
				if (symbol == 23)
				{
					if (!getsym(0)) { return false; }
					symbol_array[symarr++] = symbol;
					if (symbol == 21)//=
					{
						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
						if (symbol == 33 || symbol == 34)
						{
							FLAG_IN_CONST = symbol;
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
							if (symbol == 24)
							{
								type = 11;
								if (RE_find(CHUAN_item - 1) != 0) { error(45); }
								if (FLAG_IN_CONST == 33)
								{

									enter(lev, type, 4, CHUAN_item - 1, true);
								}
								else if (FLAG_IN_CONST == 34)
								{

									enter(lev, type, 4, CHUAN_item - 1, false);
								}

								if (!getsym(0)) { return false; }
								symbol_array[symarr++] = symbol;
								if (symbol == 20)
								{
									if (!getsym(0)) { return false; }
									symbol_array[symarr++] = symbol;
									continue;
								}
								else if (symbol == 22) { break; }
								else { error(17); break; }
							}
							else
							{
								cout << "not find the specific int_number" << endl;
							}
						}
						else if (symbol == 24)
						{
							type = 11;
							if (RE_find(CHUAN_item - 1) != 0) { error(45); }
							enter(lev, type, 4, CHUAN_item - 1, true);
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
							if (symbol == 20)
							{
								if (!getsym(0)) { return false; }
								symbol_array[symarr++] = symbol;
								continue;
							}
							else if (symbol == 22) { break; }
							else { error(17); break; }
						}
						else { error(16); break; }
					}
					else { error(15); break; }
				}
				else { error(14); break; }
			}
		}
		else if (symbol == 3)//float
		{
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			while (1)
			{
				if (symbol == 23)
				{
					if (!getsym(0)) { return false; }
					symbol_array[symarr++] = symbol;
					if (symbol == 21)
					{
						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
						if (symbol == 33 || symbol == 34)
						{
							FLAG_IN_CONST2 = symbol;
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
							if (symbol == 40)
							{
								type = 9;
								if (RE_find(CHUAN_item - 1) != 0) { error(45); }
								if (FLAG_IN_CONST2 == 33)
								{

									enter(lev, type, 4, CHUAN_item - 1, true);
								}
								else if (FLAG_IN_CONST2 == 34)
								{

									enter(lev, type, 4, CHUAN_item - 1, false);
								}

								if (!getsym(0)) { return false; }
								symbol_array[symarr++] = symbol;
								if (symbol == 20)
								{
									if (!getsym(0)) { return false; }
									symbol_array[symarr++] = symbol;
									continue;
								}
								else if (symbol == 22) { break; }
								else { error(17); break; }
							}
							else
							{
								cout << "not find the specific int_number" << endl;
							}
						}
						else if (symbol == 40)
						{
							type = 9;
							if (RE_find(CHUAN_item - 1) != 0) { error(45); }
							enter(lev, type, 4, CHUAN_item - 1, true);
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
							if (symbol == 20)
							{
								if (!getsym(0)) { return false; }
								symbol_array[symarr++] = symbol;
								continue;
							}
							else if (symbol == 22) { break; }
							else { error(17); break; }
						}
						else { error(16); break; }
					}
					else { error(15); break; }
				}
				else { error(14); break; }
			}
		}
		else
		{
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			while (1)
			{
				if (symbol == 23)
				{
					if (!getsym(0)) { return false; }
					symbol_array[symarr++] = symbol;
					if (symbol == 21)
					{
						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
						if (symbol == 100)
						{
							ch = getsym(3);
							if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch >= 'a'&&ch <= 'z' || ch >= 'A'&&ch <= 'Z' || ch >= '0'&&ch <= '9' || ch == '_')
							{
								zi_fu = ch;
								ascll_num = (int)zi_fu;
								getsym(3);
								if (ch == '\'')
								{
									type = 12;
									if (RE_find(CHUAN_item - 1) != 0) { error(45); }
									enter(lev, type, 4, CHUAN_item - 1, true);
									if (!getsym(0)) { return false; }
									symbol_array[symarr++] = symbol;
									if (symbol == 20)
									{
										if (!getsym(0)) { return false; }
										symbol_array[symarr++] = symbol;
										continue;
									}
									else if (symbol == 22) { break; }
									else { error(17); break; }
								}
								else { error(18); break; }
							}
							else { error(19); break; }
						}
						else { error(18); break; }
					}
					else { error(15); break; }
				}
				else { error(14); break; }
			}
		}
	}
	else { error(13); }
	return true;
}
bool variable_description(int typ)//变量说明
{
	int temp_var;
	if (symbol == 2 || symbol == 3 || symbol == 4)
	{
		if (symbol == 2) { temp_var = 11; }
		if (symbol == 3) { temp_var = 9; }
		if (symbol == 4) { temp_var = 12; }
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
		while (1)
		{
			if (symbol == 23)
			{
				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
				if (symbol == 38)
				{
					if (!getsym(0)) { return false; }
					symbol_array[symarr++] = symbol;
					if (wu_fu_hao_digital(typ))
					{
						fu_int_num = int_num;
						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
						if (symbol == 39)
						{
							if (RE_find(CHUAN_item - 1) != 0) { error(45); }
							enter(lev, temp_var, 6, CHUAN_item - 1, fu_int_num);//symbol==24
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
							if (symbol == 20)
							{
								if (!getsym(0)) { return false; }
								symbol_array[symarr++] = symbol;
								continue;
							}
							else if (symbol == 22) { break; }
							else { error(17); }
						}
						else { error(23); }
					}
					else { return false; }
				}
				else if (symbol == 22)//如果是分号
				{
					if (RE_find(CHUAN_item - 1) != 0) { error(45); }
					enter(lev, temp_var, 0, CHUAN_item - 1, true);
					break;
				}
				else if (symbol == 20)//如果是逗号
				{
					if (RE_find(CHUAN_item - 1) != 0) { error(45); }
					enter(lev, temp_var, 0, CHUAN_item - 1, true);
					if (!getsym(0)) { return false; }
					symbol_array[symarr++] = symbol;
					continue;
				}
				else { error(17); }
			}
			else { error(14);  break; }
		}
	}
	else { error(20); }
	return true;
}
bool statement(int typ)
{
	int f_f_f_f = 0;
	int op_number1, op_number22, op_number4, op_number5, op_number8, op_number2, op_number67;
	int ret_find;
	int i_5;
	int sc_i;
	if (symbol == 15)//printf
	{
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
		if (symbol == 16)
		{
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			if (symbol == 200)
			{
				getsym(3);
				CLEAR();
				print_f[printf_item] = ch;
				printf_item = printf_item + 1;
				while (ch == 32 || ch == 33 || (ch >= 35 && ch <= 126) || ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t')
				{
					getsym(3);//单个字符读取
					print_f[printf_item] = ch;
					printf_item = printf_item + 1;
				}
				getsym(4);//把多读取的内容给回退
				FUZHI();
				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
				if (symbol == 200)
				{
					if (gen(8, 5, Print_F)) { return 1; }

					if (!getsym(0)) { return false; }
					symbol_array[symarr++] = symbol;
					//CLEAR();
					//CLEAR_C();//可以考虑此将两者清理
					if (symbol == 17)
					{
						CLEAR();
						CLEAR_C();
						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
						if (symbol == 22)
						{
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
						}
						else { error(17); }
					}
					else if (symbol == 20)//逗号
					{
						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
						if (expression(typ))
						{
							SBSB = 0;
							if (gen(8, printf_flag, 0)) { return 1; }

							CLEAR();
							CLEAR_C();
							if (symbol == 17)
							{
								if (!getsym(0)) { return false; }
								symbol_array[symarr++] = symbol;
								if (symbol == 22)
								{

									if (!getsym(0)) { return false; }
									symbol_array[symarr++] = symbol;
								}
								else { error(17); }
							}
							else { error(21); }
						}
						else { return false; }
					}
					else { error(21); }
				}
				else { error(25); }
			}
			else
			{
				if (expression(typ))
				{
					SBSB = 0;
					if (gen(8, printf_flag, 0)) { return 1; }

					CLEAR();
					CLEAR_C();
					if (symbol == 17)
					{
						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
						if (symbol == 22)
						{
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
						}
						else { error(17); }
					}
					else { error(21); }
				}
				else { return false; }
			}
		}
		else { error(24); }
	}
	else if (symbol == 14)//scanf
	{
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
		if (symbol == 16)//(
		{
			while (1)
			{
				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
				if (symbol == 23)//ident
				{
					f_f_f_f = find(CHUAN_item - 1);
					if (f_f_f_f == 0) { cout << "not find the element which we will use" << endl; }
					if (FU_list[f_f_f_f].kind == 0 || FU_list[f_f_f_f].kind == 3)//如果是变量或者参数
					{
						if (gen(6, FU_list[f_f_f_f].type, 0)) { return 1; }
						if (gen(22, FU_list[f_f_f_f].lev, FU_list[f_f_f_f].adr)) { return 1; }
						//输入数据并传递到运行栈中
					}
					//如果不是变量或者参数，给予报错处理
					if (!getsym(0)) { return false; }
					symbol_array[symarr++] = symbol;
					if (symbol == 17) { break; }
					else if (symbol == 20) { continue; }
					else { error(21); break; }
				}
				else { error(14); break; }
			}
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			if (symbol == 22)
			{
				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
			}
			else { error(17); }
		}
		else { error(24); }
	}
	else if (symbol == 7)//if语句
	{
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
		if (symbol == 16)
		{
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			if (expression(typ))
			{
				SBSB = 0;
				if (printf_flag == 12) { error(51); }//不能是字符类型
				judge_if1 = printf_flag;
				//cout << "expression in if type is" << printf_flag << endl;
				//if (printf_flag != 11) { error(46); }
				/*进一步判断表达式0-1,not ending.................*/
				//如何判断表达式是否为零？
				//生成指令，帮助判断
				//在解析执行到此时的时候，表达式的值已经在栈顶就位，只需要一条指令，将值取出来，并与0比较就可以了
				//如果表达式的值为零，那么就跳转到else处执行，否则不跳转，继续往下面读取symbol
				if (symbol != 17)
				{
					if (symbol == 26 || symbol == 27 || symbol == 28 || symbol == 29 || symbol == 30 || symbol == 31)
					{
						if (symbol == 27) { f_f_f_f = 16; }
						else if (symbol == 29) { f_f_f_f = 20; }
						else if (symbol == 26) { f_f_f_f = 17; }
						else if (symbol == 28) { f_f_f_f = 21; }
						else if (symbol == 30) { f_f_f_f = 18; }
						else if (symbol == 31) { f_f_f_f = 19; }
						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
						if (expression(typ))
						{
							SBSB = 0;
							if (printf_flag == 12) { error(51); }
							judge_if2 = printf_flag;
							if (judge_if1 != judge_if2) { error(52); }

							//cout << "expression in if type is" << printf_flag << endl;
							//if (printf_flag != 11) { error(46); }

							if (gen(f_f_f_f, 0, 0)) { return 1; }
						}
						else { return false; }
					}
					else { error(36); }
				}
				else
				{
					//if(printf_flag==9){cout <<"do not allowed float type in this choice"<<endl;}
					if (gen(37, 0, 0)) { return 1; }//JET 判断条件是否为0，如果是0，直接走else部分，这里是跳转指令
				}
				//else{ cout << "不是关系运算符,但有可能是(a)这种形式，所以这里不需报错" << endl; }
				op_number1 = op_number;//做好跳转准备
				if (gen(3, 0, 0)) { return 1; }
				if (symbol == 17)
				{
					if (!getsym(0)) { return false; }
					symbol_array[symarr++] = symbol;
					if (statement(typ))//条件为真，需进入的语句部分
					{
						if (symbol == 8)//else判断条件
						{
							op_number2 = op_number;
							if (gen(4, 0, 0)) { return 1; }//此时走完条件为真部分，无条件转移到语句结束
							p_list[op_number1].x_int = op_number;//条件为假，跳转到这里，走完第二个语句部分结束
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
							if (statement(typ)) { p_list[op_number2].x_int = op_number; }//条件为假，需进入的语句部分
							else { return false; }
						}
						//没有else语句，将执行此部分
						else { p_list[op_number1].x_int = op_number; }
					}
					else { return false; }
				}
				else { error(21); }
			}
			else { return false; }
		}
		else { error(24); }
	}
	else if (symbol == 12)//while
	{
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;//将symbol的值放入对应的专门存symbol的数组中
		if (symbol == 16)
		{
			op_number22 = op_number;//记录while循环的开始位置
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			if (expression(typ))
			{
				SBSB = 0;
				if (printf_flag == 12) { error(51); }//如果表达式的类型是char类型，报错
				judge_while1 = printf_flag;//记录当前表达式的类型用于检查类型是否匹配使用
										   //cout << "expression in while type is" << printf_flag << endl;
										   //if (printf_flag != 11) { error(46); }
				if (symbol == 26 || symbol == 27 || symbol == 28 || symbol == 29 || symbol == 30 || symbol == 31)
				{
					if (symbol == 27) { f_f_f_f = 16; }
					else if (symbol == 29) { f_f_f_f = 20; }
					else if (symbol == 26) { f_f_f_f = 17; }
					else if (symbol == 28) { f_f_f_f = 21; }
					else if (symbol == 30) { f_f_f_f = 18; }
					else if (symbol == 31) { f_f_f_f = 19; }
					if (!getsym(0)) { return false; }
					symbol_array[symarr++] = symbol;
					if (expression(typ))
					{
						SBSB = 0;
						if (printf_flag == 12) { error(51); }//表达式的类型是char 报错
						judge_while2 = printf_flag;//记录此时表达式的类型
						if (judge_while1 != judge_while2) { error(52); }//while循环两边类型进行比较，如果不一致报错

						if (gen(f_f_f_f, 0, 0)) { return false; }
						op_number1 = op_number;//记录此时while循环的位置
						if (gen(3, 0, 0)) { return false; }//生成一条跳转指令

						if (symbol == 17)
						{
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
							if (statement(typ))
							{
								if (gen(2, 0, op_number22)) { return false; }//该指令跳转到while循环的开始位置

								p_list[op_number1].x_int = op_number;//记录while循环结束的P-CODE位置
							}
							else { return false; }
						}
						else { error(21); }
					}
					else { return false; }
				}
				else
				{
					//if(printf_flag==9){cout <<"not be allowed float type in this choice"<<endl;}
					if (gen(43, 0, 0)) { return 1; }//判断表达式的值是否为0
					op_number1 = op_number;
					if (gen(3, 0, 0)) { return 1; }//如果表达式的值为0，则条件转移到while循环外面。
					if (symbol == 17)//)
					{
						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
						if (statement(typ))
						{
							if (gen(2, 0, op_number22)) { return 1; }//再次进入while循环判断
							p_list[op_number1].x_int = op_number;
						}
						else { return false; }
					}
					else { error(21); }
				}
			}
			else { return false; }
		}
		else { error(24); }
	}
	else if (symbol == 23)
	{
		ret_find = find(CHUAN_item - 1);//查找符号表内容，看是否存在，存在则返回元素下标
		if (ret_find == 0) { cout << "not find the element which be token in the FU_list before" << endl; }
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
		if (FU_list[ret_find].kind == 0 || FU_list[ret_find].kind == 3)//说明是变量或者参数
		{
			
			if (symbol == 21)//=
			{
				
				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
				RET_FLAG = find(CHUAN_item - 1);
				if (RET_FLAG == 0) { cout << "not find the element which be token in the FU_list before" << endl; }
				if (FU_list[RET_FLAG].kind == 1) { cout << "无返回值函数不可以赋值给左边" << endl; }
				if (expression(typ))
				{

					SBSB = 0;
					if (FU_list[ret_find].type != printf_flag) { error(40); }//赋值语句左右类型不匹配
					//FU_list[ret_find].type = printf_flag;
					if (gen(22, FU_list[ret_find].lev, FU_list[ret_find].adr)) { return 1; }

					if (symbol == 22)
					{
						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
					}
					else { error(17); }
				}
				else { return false; }
			}
			else { error(15); }
		}
		else if (FU_list[ret_find].kind == 1 || FU_list[ret_find].kind == 2)//如果是有无返回值函数定义
		{
			if (symbol == 16)
			{
				if (gen(23, FU_list[ret_find].type, 0)) { return 1; }
				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
				for (i_5 = ret_find + 1; i_5 < 100; i_5++)
				{
					if (FU_list[i_5].kind != 3) { break; }
					para_value = FU_list[i_5].type;//查找符号表中参数的类型，并放入临时变量中
					if (expression(typ))
					{
						//add something
						SBSB = 0;
						if (para_value != 0 && para_value != printf_flag) { cout << "参数类型不匹配" << endl; }
						if (symbol == 20)
						{
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
						}
					}
					else { return false; }
				}
				para_value = 0;
				if ((FU_list[i_5].kind != 3 && symbol != 17)) { cout << "参数个数不匹配" << endl; }
				if (gen(24, 0, FU_list[ret_find].adr)) { return 1; }
				if (symbol == 17)
				{
					if (!getsym(0)) { return false; }
					symbol_array[symarr++] = symbol;
					if (symbol == 22)
					{
						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
					}
					else { error(17); }
				}
				else { return false; }
			}
			else { error(24); }
		}
		else if (FU_list[ret_find].kind == 6)//是数组
		{
			if (symbol == 38)//[
			{
				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
				if (expression(typ))
				{
					//cout << "expression in array type is" << printf_flag << endl;
					SBSB = 0;
					if (printf_flag != 11) { error(44); }//判断数组的内含参数是不是整型的，如果不是则会报错
														 //PUF,将符号表数组大小放入次栈顶，以便比较，检查数组大小是否越界
					if (gen(33, 0, FU_list[ret_find].com_bine.INT_num)) { return 1; }

					//新指令：JUB    判断指令，判断表达式的值是否满足数组的上限（栈顶与次栈顶值进行比较）
					if (gen(31, 0, 0)) { return 1; }


					op_number4 = op_number;
					if (gen(39, 0, 0)) { return 1; }

					//cout << "not pi pei" << endl;
					p_list[op_number4].x_int = op_number;

					//JSW,计算地址，这里语句部分为了防止跟因子部分冲突，特意新添了一个变量main2_address
					if (gen(36, 0, 0)) { return 1; }

					if (symbol == 39)//]
					{
						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
						if (symbol == 21)//=
						{
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
							if (expression(typ))
							{

								SBSB = 0;
								if (FU_list[ret_find].type != printf_flag) { error(40); }
								//FU_list[ret_find].type = printf_flag;
								//STW 不同于sto的是，多了一个参数main2_address,用于进行数组偏移地址计算
								if (gen(34, FU_list[ret_find].lev, FU_list[ret_find].adr)) { return 1; }

								if (symbol == 22)
								{
									if (!getsym(0)) { return false; }
									symbol_array[symarr++] = symbol;
								}
								else { error(29); }
							}
							else { return false; }
						}
						else { error(15); }
					}
					else { error(23); }
				}
				else { return false; }
			}
			else { error(32); }
		}
		else { cout << "不属于标识符范畴，被错误引用" << endl; }
	}/////////////////getsym(0);
	else if (symbol == 9)
	{
		int op_number20 = 0, op_number21 = 0, k;
		sc_item = 0;
		op_number5 = 0;

		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
		if (symbol == 16)//(
		{
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			if (expression(typ))
			{
				SBSB = 0;
				printf_flag_judge = printf_flag;
				if (printf_flag == 11 || printf_flag == 12)
				{
					if (symbol == 17)
					{
						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
						if (symbol == 18)//{
						{
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
							while (1)
							{
								if (symbol == 10)//case
								{
									if (!getsym(0)) { return false; }
									symbol_array[symarr++] = symbol;
									if (symbol == 33 || symbol == 34)
									{
										FLAG_ADD_SUB = symbol;
										FLAG_add_sub = symbol;
										if (!getsym(0)) { return false; }
										symbol_array[symarr++] = symbol;
										if (symbol == 24)//int
										{
											printf_sc = 11;
											if (printf_sc == printf_flag_judge)
											{
												if (sc_item == 0)
												{
													if (FLAG_ADD_SUB == 33)
													{
														FLAG_ADD_SUB = 0;
														switch_case[sc_item++] = int_num;
													}
													else if (FLAG_ADD_SUB == 34)
													{
														FLAG_ADD_SUB = 0;
														switch_case[sc_item++] = -int_num;
													}
												}
												else
												{
													if (FLAG_ADD_SUB == 33)
													{
														for (sc_i = 0; sc_i < sc_item; sc_i++)
														{
															if (switch_case[sc_i] == int_num)
															{
																error(43);
																break;
															}
														}
													}
													if (FLAG_ADD_SUB == 34)
													{
														for (sc_i = 0; sc_i < sc_item; sc_i++)
														{
															if (switch_case[sc_i] == -int_num)
															{
																error(43);
																break;
															}
														}
													}
													if (FLAG_ADD_SUB == 33)
													{
														FLAG_ADD_SUB = 0;
														switch_case[sc_item++] = int_num;
													}
													else if (FLAG_ADD_SUB == 34)
													{
														FLAG_ADD_SUB = 0;
														switch_case[sc_item++] = -int_num;
													}
												}
											}
											else { error(42); }
											if (FLAG_add_sub == 33)
											{
												if (gen(1, 11, int_num)) { return 1; }
											}
											else if (FLAG_add_sub == 34)
											{
												if (gen(1, 11, -int_num)) { return 1; }
											}

											printf_flag = 11;
											if (gen(26, 0, 0)) { return 1; }//compare

											op_number20 = op_number;
											if (gen(28, 0, 0)) { return 1; }//有条件跳转

											if (!getsym(0)) { return false; }
											symbol_array[symarr++] = symbol;
											if (symbol == 32)//:
											{
												if (!getsym(0)) { return false; }
												symbol_array[symarr++] = symbol;
												if (statement(typ))
												{

													op_number21 = op_number;
													OPP[op_item++] = op_number21;
													if (gen(42, 0, op_number21)) { return 1; }//有条件跳转
													p_list[op_number20].x_int = op_number;//有条件跳转
													if (symbol == 11) 
													{
														DEFAULT = 1;
														break; 
													}
													if (symbol == 19) { break; }
													continue;
												}
												else { return false; }
											}
											else { error(29); break; }
										}
										else { error(53); }

									}

									else if (symbol == 24)//int
									{
										printf_sc = 11;
										if (printf_sc == printf_flag_judge)
										{
											if (sc_item == 0)

											{
												switch_case[sc_item++] = int_num;
											}
											else
											{
												for (sc_i = 0; sc_i < sc_item; sc_i++)
												{
													if (switch_case[sc_i] == int_num)
													{
														error(43);
														break;
													}
												}
												switch_case[sc_item++] = int_num;
											}
										}
										else { error(42); }
										if (gen(1, 11, int_num)) { return 1; }
										printf_flag = 11;

										if (gen(26, 0, 0)) { return 1; }


										op_number20 = op_number;
										if (gen(28, 0, 0)) { return 1; }

										if (!getsym(0)) { return false; }
										symbol_array[symarr++] = symbol;
										if (symbol == 32)//:
										{
											if (!getsym(0)) { return false; }
											symbol_array[symarr++] = symbol;
											if (statement(typ))
											{

												op_number21 = op_number;
												OPP[op_item++] = op_number21;
												if (gen(42, 0, op_number21)) { return 1; }
												p_list[op_number20].x_int = op_number;


												if (symbol == 11) 
												{
													DEFAULT = 1;
													break; 
												}
												if (symbol == 19) { break; }
												continue;
											}
											else { return false; }
										}
										else { error(29); break; }
									}
									else if (symbol == 100)
									{////////这里有一个小bug，就是没有分析'  a'情况，即是有一个空格的情况
										getsym(3);
										if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '_' || (ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z') || (ch >= '0'&&ch <= '9'))
										{
											zi_fu_three = ch;
											ascll_num_three = (int)zi_fu_three;
											getsym(3);
											if (ch == '\'')
											{
												printf_sc = 12;
												if (printf_sc == printf_flag_judge)
												{
													if (sc_item == 0)
													{
														switch_case[sc_item++] = int_num;
													}
													else
													{
														for (sc_i = 0; sc_i < sc_item; sc_i++)
														{
															if (switch_case[sc_i] == zi_fu_three)
															{
																error(43);
																break;
															}
														}
														switch_case[sc_item++] = zi_fu_three;
													}
												}
												else { error(42); }

												//symbol = 25;//说明是字符类型的
												if (gen(1, 12, zi_fu_three)) { return 1; }

												printf_flag = 12;
												if (gen(26, 0, 0)) { return 1; }


												op_number20 = op_number;
												if (gen(28, 0, 0)) { return 1; }

												if (!getsym(0)) { return false; }
												symbol_array[symarr++] = symbol;
												if (symbol == 32)//:
												{
													if (!getsym(0)) { return false; }
													symbol_array[symarr++] = symbol;
													if (statement(typ))
													{

														op_number21 = op_number;
														OPP[op_item++] = op_number21;
														if (gen(42, 0, op_number21)) { return 1; }
														p_list[op_number20].x_int = op_number;


														if (symbol == 11) 
														{ 
															DEFAULT = 1;
															break; 
														}
														if (symbol == 19) { break; }//add++++++++
														continue;
													}
													else { return false; }
												}
												else { error(29); break; }
											}
											else { error(18); break; }
										}
										else
										{/*注意指针的回退*/
											error(19);
											break;
										}
									}

									else { error(20); break; }
								}
								else
								{

									error(28);
									break;
								}
							}

							if (symbol == 19)//}
							{

								for (k = 0; k < op_item; k++)
								{
									p_list[OPP[k]].x_int = op_number;
								}
								op_item = 0;
								//p_list[op_number21].x_int = op_number;
								if (!getsym(0)) { return false; }
								symbol_array[symarr++] = symbol;
								return true;
							}
							else
							{
								if (!getsym(0)) { return false; }
								symbol_array[symarr++] = symbol;
								if (symbol == 32)//:
								{


									if (!getsym(0)) { return false; }
									symbol_array[symarr++] = symbol;
									if (statement(typ))
									{

										if (symbol == 19)
										{
											for (k = 0; k < op_item; k++)
											{
												p_list[OPP[k]].x_int = op_number;
											}
											op_item = 0;
											//p_list[op_number21].x_int = op_number;
											if (!getsym(0)) { return false; }
											symbol_array[symarr++] = symbol;
											return true;
										}
										else { error(30); }

									}
									else { return false; }

								}
								else { error(29); }
							}
							//bug//

						}
						//add

						else { error(27); }

					}
					else { error(21); }
				}
				else { error(41); }
			}
			else { return false; }
		}
		else { error(24); }
	}
	else if (symbol == 18)
	{
		//lev=lev+1;
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
		if (symbol == 19)//}
		{
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			/*if (DEFAULT == 1)
			{
				DEFAULT = 0;
				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
			}*/
			return true;
		}
		while (1)
		{
			if (statement(typ))
			{
				if (symbol == 19)
				{
					//lev=lev-1;
					if (!getsym(0)) { return false; }
					symbol_array[symarr++] = symbol;
					break;
				}
			}
			else { return false; }
		}
	}/////////////////getsym(0);
	else if (symbol == 22)
	{
		//y = 0;
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
	}
	else
	{

		if (symbol == 13)
		{
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			if (symbol == 16)//(
			{

				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
				if (expression(typ))
				{
					SBSB = 0;
					if (gen(7, 11, 0)) { return 1; }

					if (symbol == 17)
					{

						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
						if (symbol == 22)
						{
							x = 1;
							y = 1;
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
						}
						else { error(17); }
					}
					else { error(21); }
				}
				else { return false; }
			}
			else if (symbol == 22)
			{
				//y = 0;
				//y = 0;
				if (FLAG_JUMP_MAIN == 1)
				{
					FLAG_JUMP_MAIN = 0;
					//JUMP
					op_number67 = op_number;
					if (gen(44, 0, 0)) { return 1; }
					// jump to here!
					p_list[op_number67].x_int = op_number;
				}
				else
				{
					if (gen(1, 11, 0)) { return 1; }
					if (gen(7, 11, 0)) { return 1; }
				}
				//jump to here!(error!)

				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
			}
			else { error(17); }
		}

		else
		{
			error(47);
		}
	}
	return true;
}

bool paralist(int typ)
{
	int temp;
	while (1)
	{
		if (symbol == 2 || symbol == 3 || symbol == 4)
		{
			if (symbol == 2) { temp = 11; }
			if (symbol == 3) { temp = 9; }
			if (symbol == 4) { temp = 12; }
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			if (symbol == 23)//此时肯定知道他是参数，因此登表
			{
				if (RE_find(CHUAN_item - 1) != 0) { error(45); }
				enter(lev, temp, 3, CHUAN_item - 1, true);
				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
				if (symbol == 20)
				{
					if (!getsym(0)) { return false; }
					symbol_array[symarr++] = symbol;
					continue;
				}
				else { break; }
			}
			else { error(14); break; }
		}
		else if (symbol == 17) { break; }
		else { error(13); break; }
	}
	return true;
}
bool return_no_return(int typ)
{
	if (symbol == 23)
	{
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
		if (symbol == 16)
		{
			if (RE_find(CHUAN_item - 1) != 0) { error(45); }
			if (VOID_FLAG == 1)
			{
				VOID_FLAG = 0;
				enter(lev, typ, 1, CHUAN_item - 1, true);
			}
			else
			{
				enter(lev, typ, 2, CHUAN_item - 1, true);
			}
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			lev = lev + 1;
			if (paralist(typ))
			{
				if (symbol == 17)
				{
					if (!getsym(0)) { return false; }
					symbol_array[symarr++] = symbol;
					if (symbol == 18)
					{
						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
						if (fu_he_statement(typ))
						{
							if (flag == 0)
							{

								if (!getsym(0)) { return false; }
								symbol_array[symarr++] = symbol;
							}
							flag = 0;
							if (symbol == 19) { ; }
							else { error(30); }
						}
						else { return false; }
					}
					else { error(27); }
				}
				else { error(21); }
			}
			else { return false; }
		}
		else { error(24); }
	}
	else { error(14); }
	return true;

}
bool return_value_function_definition(int typ)//有返回值函数定义
{
	if (symbol == 2 || symbol == 3 || symbol == 4)
	{
		if (symbol == 2) { typ = 11; }
		if (symbol == 3) { typ = 9; }
		if (symbol == 4) { typ = 12; }
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
		if (return_no_return(typ))
		{
			y = 0;
			//if (!getsym(0)) { return false; }/////////////////////新加上的指令
			if (x == 0) { error(48); }
			else { x = 0; }
		}
		else { return false; }
	}
	else { error(13); }
	return true;
}
bool no_return_value_function_definition(int typ)//无返回值函数定义
{
	if (symbol == 5)
	{
		typ = 12;
		VOID_FLAG = 1;
		VOID_SYM = 1;
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
		if (return_no_return(typ))
		{
			if (y == 1 && VOID_SYM == 1)
			{
				VOID_SYM = 0;
				y = 0;
				error(49);
			}

			//if (!getsym(0)) { return false; }//////////////新加上的指令
			/*if (y == 1) { error(49); }
			else { y = 1; }*/
		}
		else { return false; }
	}
	else { error(20); }
	return true;
}
bool fu_he_statement(int typ)
{

	if (symbol == 1)
	{
		while (1)
		{
			if (symbol == 1)
			{
				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
				if (symbol == 2 || symbol == 3 || symbol == 4)
				{
					if (constant_description(typ))
					{
						getsym(1);
						if (!getsym(2)) { return false; }
						if (symbol == 1)
						{
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
							continue;
						}
						else if (symbol == 19)
						{
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
							flag = 1;
							return true;
						}
						else
						{
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
							//fu_he_flag = 1; 
							break;
						}
					}
					else { return false; }
				}
				else { error(13); }
			}
			else { break; }
		}
	}
	if (symbol == 2 || symbol == 3 || symbol == 4)
	{
		while (1)
		{
			if (symbol == 2 || symbol == 3 || symbol == 4)
			{
				if (symbol == 2) { typ = 11; }
				if (symbol == 3) { typ = 9; }
				if (symbol == 4) { typ = 12; }
				if (variable_description(typ))
				{
					getsym(1);
					if (!getsym(2)) { return false; }
					if (symbol == 2 || symbol == 3 || symbol == 4)
					{
						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
						continue;
					}
					else if (symbol == 19)
					{
						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
						flag = 1;
						return true;
					}
					else
					{
						if (!getsym(0)) { return false; }//=+++++++++++++++++++++++
						symbol_array[symarr++] = symbol;
						//key_to_19 = 1;
						break;
					}
				}
				else { return false; }
			}
			else { error(13); break; }
		}
	}
	if (symbol == 18 || symbol == 7 || symbol == 12 || symbol == 23 || symbol == 14 || symbol == 15 || symbol == 9
		|| symbol == 13 || symbol == 22)
	{
		if (gen(9, 0, LEV_count_list[lev])) { return 1; }

		while (1)
		{
			if (symbol == 18 || symbol == 7 || symbol == 12 || symbol == 23 || symbol == 14 || symbol == 15 || symbol == 9
				|| symbol == 13 || symbol == 22)
			{
				if (statement(typ))
				{
					if (symbol == 18 || symbol == 7 || symbol == 12 || symbol == 23 || symbol == 14 || symbol == 15 || symbol == 9
						|| symbol == 13 || symbol == 22)
					{
						continue;
					}
					else if (symbol == 19)
					{
						getsym(1);
						//if (!getsym(2)) { return false; }
						if (!getsym(2)) { ; }
						if (symbol == 19)
						{
							if (!getsym(0)) { return false; }
							symbol_array[symarr++] = symbol;
							flag = 1;
							return true;
						}
						else if (symbol == 18 || symbol == 22)
						{
							if (!getsym(0)) { return false; }
							continue;
						}
						else
						{
							flag = 1;
							symbol = symbol_array[symarr - 1];
							return true;
						}
					}
					else { error(30); break; }
				}
				else { return false; }
			}
			else { break; }
		}
	}

	error(20);
	return true;
}///////success stop getsym();


bool return_no_void_tance(int typ)
{
	while (1)
	{
		while (1)
		{
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			if (return_value_function_definition(typ))
			{
				get_out_of_FU_list();
				lev = lev - 1;
				if (gen(7, 0, 0)) { return 1; }

				getsym(1);
				if (!getsym(2)) { return false; }
				if (symbol == 2 || symbol == 3 || symbol == 4) { continue; }
				else if (symbol == 5) { break; }
				else { error(37); break; }
			}
			else { return false; }
		}
		while (1)
		{
			if (!getsym(2)) { return false; }
			if (symbol == 23)//ident
			{
				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
				if (no_return_value_function_definition(typ))
				{
					get_out_of_FU_list();
					lev = lev - 1;
					if (gen(25, 0, 0)) { return 1; }

					getsym(1);
					if (!getsym(2)) { return false; }
					if (symbol == 5) { continue; }
					else if (symbol == 2 || symbol == 3 || symbol == 4)
					{
						flag_flag = 1;
						break;
					}
				}
				else { return false; }
			}
			else if (symbol == 6)
			{
				flag_main = 1;
				break;
			}
			else if (symbol == 0) { return false; }
			else { error(14); break; }
		}
		if (flag_flag == 1)
		{
			flag_flag = 0;
			continue;
		}
		else if (flag_main == 1) { break; }
		else { break; }
	}
	if (flag_main == 1)
	{
		flag_main = 0;
		if (main_function(typ))
		{
			;
		}
		else { return false; }
	}
	return true;
}
bool no_return_void_tance(int typ)
{
	while (1)
	{
		while (1)
		{
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			if (no_return_value_function_definition(typ))
			{
				get_out_of_FU_list();
				lev = lev - 1;
				if (gen(25, 0, 0)) { return 1; }

				getsym(1);
				if (!getsym(2)) { return false; }
				if (symbol == 5)
				{
					if (!getsym(2)) { return false; }
					if (symbol == 23) { continue; }
					else if (symbol == 6)
					{
						flag_main_two = 1;
						break;
					}
					else { error(20); }
				}
				else if (symbol == 2 || symbol == 3 || symbol == 4) { break; }
				else { error(37); break; }
			}
			else { return false; }
		}
		while (1)
		{
			if (flag_main_two == 1) { break; }
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			if (return_value_function_definition(typ))
			{
				get_out_of_FU_list();
				lev = lev - 1;
				if (gen(7, 0, 0)) { return 1; }

				getsym(1);
				if (!getsym(2)) { return false; }
				if (symbol == 2 || symbol == 3 || symbol == 4) { continue; }
				else if (symbol == 5)
				{
					SYMBOL_FLAG_W = symbol;
					getsym(2);
					if (symbol == 6)
					{
						flag_main_two = 1;
						break;
					}
					else
					{
						symbol = SYMBOL_FLAG_W;
						SYMBOL_FLAG_W = 0;
						flag_flag_flag = 1;
						break;
					}
				}
			}
			else if (symbol == 0) { return false; }
			else { return false; }
		}
		if (flag_flag_flag == 1)
		{
			flag_flag_flag = 0;
			continue;
		}
		else if (flag_main_two == 1) { break; }
		else { break; }
	}
	if (flag_main_two == 1)
	{
		flag_main_two = 0;
		if (main_function(typ)) { ; }
		else { return false; }
	}
	return true;
}
bool main_function(int typ)
{
	FLAG_JUMP_MAIN = 1;
	start = op_number;
	if (!getsym(0)) { return false; }
	symbol_array[symarr++] = symbol;
	if (!getsym(0)) { return false; }
	symbol_array[symarr++] = symbol;
	enter(lev, 0, 5, CHUAN_item - 1, true);//the second number shoulb be 2 from count 1 number
										   //FU_list[FU_item-1].adr=op_number;
	if (!getsym(0)) { return false; }
	symbol_array[symarr++] = symbol;
	if (symbol == 16)
	{
		if (!getsym(0)) { return false; }
		symbol_array[symarr++] = symbol;
		if (symbol == 17)
		{
			if (!getsym(0)) { return false; }
			symbol_array[symarr++] = symbol;
			if (symbol == 18)
			{
				lev = lev + 1;//lev++
				if (!getsym(0)) { return false; }
				symbol_array[symarr++] = symbol;
				if (fu_he_statement(typ))
				{
					if (flag == 0)
					{

						if (!getsym(0)) { return false; }
						symbol_array[symarr++] = symbol;
					}

					flag = 0;
					if (symbol == 19)
					{
						get_out_of_FU_list();
						lev = lev - 1;
						//if (gen(25, 0, 0)) { return 1; }
						//cout << "EXP" << "	" << "0" << "	" << "0" << endl;
						//cout << "main_function is success" << endl;
					}
					else { error(30); }
				}
				else { return false; }
			}
			else { error(27); }
		}
		else { error(21); }
	}
	else { error(24); }
	return true;
}
bool program(int typ)//主程序
{
	int temp = 0;
	while (1)
	{
		if (symbol == 1)//const
		{
			if (!getsym(0)) { return false; }//const
			symbol_array[symarr++] = symbol;
			if (!getsym(0)) { return false; }//int|float|char|
			symbol_array[symarr++] = symbol;
			if (constant_description(typ))
			{
				getsym(1);
				if (!getsym(2)) { return false; }
				if (symbol == 1) { continue; }
			}
			else { return false; }
		}
		break;
	}
	while (1)
	{
		if (symbol == 2 || symbol == 3 || symbol == 4)//int|float|char	(future1)
		{
			//if (!getsym(0)) { return false; }						//pass  future1
			if (symbol == 2) { temp = 11; }
			if (symbol == 3) { temp = 9; }
			if (symbol == 4) { temp = 12; }
			if (!getsym(2)) { return false; }
			if (symbol == 23)										//(future2)
			{
				if (!getsym(2)) { return false; }					//(future3)
				if (symbol == 16)
				{
					if (return_no_void_tance(temp)) { ; }
					else { return false; }
				}
				else
				{
					if (!getsym(0)) { return false; }
					symbol_array[symarr++] = symbol;
					if (variable_description(temp))
					{
						getsym(1);
						if (!getsym(2)) { return false; }
						if (symbol == 2 || symbol == 3 || symbol == 4) { continue; }
					}
					else { return false; }
				}
			}
			else { error(14); break; }
		}
		break;
	}
	if (symbol == 5)
	{
		if (!getsym(2)) { return false; }
		if (symbol == 23)
		{
			if (no_return_void_tance(typ)) { ; }
			else { return false; }
		}
		else if (symbol == 6)
		{
			if (main_function(typ))
			{
				;

			}
			else { return false; }
		}
		else { error(38); }
	}
	return true;
}

int main()
{
	int typ = 0;
	int i = 0;
	//scanf("%s\n", &filename);
	cout << "请输入文件名:";
	cin >> token2;
	for (i = 0; i < 10; i++) { LEV_count_list[i] = 0; }
	//do
	//{
		cout << "1" << endl;
		if (!getsym(2)) { return -1; }
		if (!program(typ)) { return -1; }
		/*if (error_count >= 3)//错误个数太多，程序不解释执行
		{
		cout << "这个错误大于等于3，程序无法处理" << endl;
		break;
		}
		cout << "interprent output" << endl;
		if (interprent())
		{
		cout << "interprent is error" << endl;
		return -1;

		}
		else
		{
		if (ch == '\0')
		{
		break;
		}
		}*/
	//} while (ch != '\0');
	cout << "本程序暂不支持纠错能力，即如果错误个数大于1，本程序将不再解释执行" << endl;

	/*if (error_count == 0)
	{
		cout << "interprent output" << endl;
		interprent();
		cout << "program over!" << endl;
	}
	else
	{
		cout << "目前有错误，无法解释执行" << endl;
	}*/
	cout << "interprent output" << endl;
	interprent();
	
	return 0;
}
int interprent()
{
	int i;
	int a;
	int equ = 1;
	int main_address = 0, main2_address = 0;
	int fang = -1;
	int ZHAN = 0;
	float swap;
	int swap1;
	char swap2;
	float swap3;
	btab[btab_item++] = 0;
	run[run_item++] = 0;
	run[run_item++] = 0;
	//对于数组来讲，咱们这里的解释执行程序的btab[]部分，就已经包括了数组的基地址部分，因此，咱们只要根据
	//数组对应下标的值，进行适当的增添，就可以达到访问数组对应位置变量的目的
	for (i = 1; i < 1000; i++)//对于函数内部也就是不是顶层定义的变量来讲，该处变量的地址会重新计数
	{
		if (FU_list[i].kind == 0)//变量run_item+1，只适应于对顶层的变量计数
		{
			run_item = run_item + 1;
		}
		else if (FU_list[i].kind == 6)//数组run_item+1，数组这里只是在顶层加了，如果在函数内部出现，依然可以加1，没有关系。
		{

			run_item = run_item + FU_list[i].com_bine.INT_num;//这里加上了数组上界的最大值
		}
		else if (FU_list[i].kind != 0 && FU_list[i].kind != 4)//函数名或者函数参数,跳出循环，他们不参与顶层变量计数
		{
			break;
		}
	}//因此，这里for循环的目的在于对顶层的各种变量（包括数组变量、普通变量）计数，常量不参与计数
	btab[btab_item++] = run_item;// the number is due to the size of array
	run[run_item++] = 0;
	run[run_item++] = 0;
	for (i = start; i < op_number; i++)
	{
		//cout << p_list[i].op_num << "	" << p_list[i].lev << "	" << p_list[i].x_int << endl;
		switch (p_list[i].op_num)
		{
		case 0://LDA	从地址中读取值
			if (p_list[i].lev == 1)//如果是第一层，地址等于：第一层的变量总数+1
			{
				run[run_item++] = run[btab[0] + p_list[i].x_int + 1];
			}
			else                   //如果不是第一层，地址等于：上一层的
			{
				run[run_item++] = run[btab[btab_item - 1] + p_list[i].x_int + 1];
			}
			break;

		case 1://LDI	加载立即数，放到运行栈栈顶
			if (p_list[i].lev == 11 || p_list[i].lev == 12)
			{
				run[run_item++] = p_list[i].x_int;//这里的x_int域对应立即数的值，如果是char类型，则对应ascll码
			}
			else if (p_list[i].lev == 9)
			{
				run[run_item++] = p_list[i].y_float;
			}
			break;

		case 2://JMD	用在while循环中，为了进行相应的循环跳转,跳转到开头
			i = p_list[i].x_int - 1;
			break;

		case 3://JMC	不满足跳转，用在if,switch-case语句中
			if (!equ) { i = p_list[i].x_int - 1; }
			break;

		case 4://JMP	if、while、中的无条件跳转
			i = p_list[i].x_int - 1;
			break;

		case 6://RED
			if (p_list[i].lev == 11)
			{
				cin >> swap1;
				run[run_item++] = swap1;
			}
			else if (p_list[i].lev == 12)
			{
				cin >> swap2;
				run[run_item++] = swap2;
			}
			else if (p_list[i].lev == 9)
			{
				cin >> swap3;
				run[run_item++] = swap3;
			}
			break;

		case 7://EXF	函数返回指令
			i = run[btab[btab_item - 1]] - 1;
			run[btab[btab_item - 1]] = run[--run_item];
			run[run_item] = 0;
			run_item = btab[--btab_item] + 1;
			break;

		case 8://WRR	函数返回指令
			if (p_list[i].lev == 5) { cout << p_list[i].z_char << endl; }
			else if (p_list[i].lev == 12) { cout << (char)run[--run_item] << endl; }
			else { cout << run[--run_item] << endl; }
			/*else 
			{ 
				cout << setprecision(9) << run[--run_item] << endl; 
			}*/
			break;

		case 9://INF	分配空间指令
			run_item = btab[btab_item - 1] + p_list[i].x_int + 2;// + 100;
			break;

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

		case 15://MUS
			run[run_item - 1] = -run[run_item - 1];
			break;

		case 16://>
			if (run[run_item - 2] > run[run_item - 1])//关系运算符比较运算，比较完成后需要将相应内容清空
			{
				run[run_item - 2] = 0;
				run[run_item - 1] = 0;
				equ = 1;
			}
			else
			{
				run[run_item - 2] = 0;
				run[run_item - 1] = 0;
				equ = 0;
			}
			break;

		case 17://<
			if (run[run_item - 2] < run[run_item - 1])
			{
				run[run_item - 2] = 0;
				run[run_item - 1] = 0;
				equ = 1;
			}
			else
			{
				run[run_item - 2] = 0;
				run[run_item - 1] = 0;
				equ = 0;
			}
			break;

		case 18://==
			if (run[run_item - 2] == run[run_item - 1])
			{
				run[run_item - 2] = 0;
				run[run_item - 1] = 0;
				equ = 1;
			}
			else
			{
				run[run_item - 2] = 0;
				run[run_item - 1] = 0;
				equ = 0;
			}
			break;

		case 19://!=
			if (run[run_item - 2] != run[run_item - 1])
			{
				run[run_item - 2] = 0;
				run[run_item - 1] = 0;
				equ = 1;
			}
			else
			{
				run[run_item - 2] = 0;
				run[run_item - 1] = 0;
				equ = 0;
			}
			break;

		case 20://>=
			if (run[run_item - 2] >= run[run_item - 1])
			{
				run[run_item - 2] = 0;
				run[run_item - 1] = 0;
				equ = 1;
			}
			else
			{
				run[run_item - 2] = 0;
				run[run_item - 1] = 0;
				equ = 0;
			}
			break;

		case 21://<=
			if (run[run_item - 2] <= run[run_item - 1])
			{
				run[run_item - 2] = 0;
				run[run_item - 1] = 0;
				equ = 1;
			}
			else
			{
				run[run_item - 2] = 0;
				run[run_item - 1] = 0;
				equ = 0;
			}
			break;

		case 22://STO	将栈顶结果存入相应位置,是LDA指令的反过程
			if (p_list[i].lev == 1) { run[btab[0] + p_list[i].x_int + 1] = run[--run_item]; }//1 level
			else { run[btab[btab_item - 1] + p_list[i].x_int + 1] = run[--run_item]; }//2 level
			break;

		case 23://CAL	函数调用指令
			a = run_item;
			X_a[X_item++] = a;//把运行栈的当前下标放进一个暂存数组中
			run[run_item + 1] = p_list[i].lev;
			run_item = run_item + 2;
			break;

		case 24://MKF
			btab[btab_item++] = X_a[--X_item];//需要将刚才存入的运行栈下标值放到一个分程序索引btab中
			run[X_a[X_item]] = i + 1;//把当前指令位置的下一个指令位置放到运行栈中，此时运行栈的下标是CAL里面之前记录的下标
			i = p_list[i].x_int - 1;//jump to the aim address
			break;

		case 25://EXP
			i = run[btab[btab_item - 1]] - 1;//需要从运行栈中取出地址的位置
			run[run_item] = 0;
			run_item = btab[--btab_item];//把原来运行栈的下标值取出来，当前运行栈的下标值就是函数调用前的下标值
			break;

		case 26://JUD	判断栈顶和次栈顶的元素是否一致，用在条件判断中
			if (run[run_item - 2] == run[run_item - 1]) { equ = 1; }
			else { equ = 0; }
			break;

		case 28://JMCC
			if (!equ)
			{
				i = p_list[i].x_int - 1;
				run_item = run_item - 1;
			}
			break;

		case 30://JSQ	主要将表达式的值从运行栈栈顶取出来，进行地址的偏移计算。
			if (equ)
			{
				main_address = run[--run_item];
				run[run_item] = 0;
			}
			break;

		case 31://JUB  判断指令，判断表达式的值是否满足数组的上限（栈顶与次栈顶值进行比较）
			if (ZHAN >= run[run_item - 1])
			{
				equ = 1;
				ZHAN = 0;
			}
			else
			{
				equ = 0;
				ZHAN = 0;
			}
			break;

		case 32://PUT	将从符号表中取出来的数组的上限值放入运行栈栈顶
			run[run_item++] = p_list[i].x_int;
			break;

		case 33://PUF	将符号表取出来的数组的上限值放到次栈顶中去
			ZHAN = p_list[i].x_int - 1;
			break;

		case 34://STW	将运行栈栈顶结果存到之前的预留的位置，这里是数组的位置
			if (p_list[i].lev == 1)
			{
				run[main2_address + btab[0] + p_list[i].x_int + 20000] = run[--run_item];
				//main_address = 0;
			}
			else
			{
				run[main2_address + btab[btab_item - 1] + p_list[i].x_int + 20000] = run[--run_item];
				//main_address = 0;
			}
			break;

		case 35://LDQ	读取数组的地址（这里地址计算需要数组的基地址以及数组的相对偏移地址）
			if (p_list[i].lev == 1) { run[run_item++] = run[btab[0] + p_list[i].x_int + main_address + 20000]; }
			else { run[run_item++] = run[btab[btab_item - 1] + p_list[i].x_int + main_address + 20000]; }
			break;

		case 36://JSW	主要将表达式的值从运行栈栈顶取出来，进行地址的偏移计算。
			if (equ)
			{
				main2_address = run[--run_item];
				run[run_item] = 0;
			}
			break;

		case 37://JET	判断表达式的值是否为零，如果为零，就跳转到else处执行
			if (run[run_item - 1] == 0) 
			{ 
				run[run_item - 1] = 0;//addaddaddadd 2019.1.7
				equ = 0; 
			}
			//if (run[--run_item] == 0) { equ = 0; }//决定跳转到else处执行
			else 
			{ 
				run[run_item - 1] = 0;//addaddaddadd 2019.1.7
				equ = 1; 
			}//不跳转，继续顺序执行
			break;

		case 38://JTE	如果为真，跳转到else处执行，否则就顺序执行
			if (equ) { i = p_list[i].x_int - 1; }
			break;

		case 39://JSZ	和JUB指令共同执行。根据JUB判断的条件，决定是否跳过数组越界错误处理部分
			if (equ) { i = p_list[i].x_int - 1; }
			else { error(50); }
			break;//如果条件为真，那么就说明表达式的值“没有”超过数组的上界值，那么就可以跳过错误处理部分，

		case 42://WU_JUMP,无条件跳转指令,用在switch_case中，如果其中一个分支执行，为了跳出整个switch_case循环
			i = p_list[i].x_int - 1;
			break;

		case 43://JUDGE_ZERO
			if (run[run_item - 1] > 0)
			{
				run[run_item - 1] = 0;
				equ = 1;
			}
			else
			{
				run[run_item - 1] = 0;
				equ = 0;
			}
			break;

		case 44://NO reason for JUMP
			i = p_list[i].x_int - 1;
			break;
		}
	}
	return 0;
}