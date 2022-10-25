#pragma once

#include<queue>
#include<stack>
#include"lexer.h"
#include"profuction.h"
using namespace std;
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
struct print
{
	int indent;						//记录缩进值
	int linenum;					//记录缩进的行号
};
typedef int status;
extern int indent0;					//记录源代码的缩进值
extern queue<print> printList;		//存储各个打印格式单元
extern int w;						//获得gettoken函数的返回值即读入的单词种类编码
extern char kind[100];			    //存取类型关键字
extern char tokenText0[100];		//存取变量名或函数名

status program(FILE* fp, CTree& T); //语法单位<程序>的子程序
status ExtDefList(FILE* fp, CTree& T);//语法单位<外部定义序列>的子程序
status ExtDef(FILE* fp, CTree& T);//语法单位<外部定义>的子程序
status ExtVarDef(FILE* fp, CTree& T);//语法单位<外部变量定义>子程序
status VarList(FILE* fp, CTree& T);//语法单位<变量序列>子程序
status funcDef(FILE* fp, CTree& T);//语法单位<函数定义>子程序
status ParameList(FILE* fp, CTree& T);//语法单位<形参序列>子程序
status FormParDef(FILE* fp, CTree& T);//语法单位<形参定义>子程序
status CompStat(FILE* fp, CTree& T);//语法单位<复合语句>子程序
status LocVarList(FILE* fp, CTree& T);//语法单位<局部变量定义序列>子程序
status LocVarDef(FILE* fp, CTree& T);//语法单位<局部变量定义>子程序
status StatList(FILE* fp, CTree& T);//语法单位<语句序列>子程序
status Statement(FILE* fp, CTree& T);//语法单位<语句>子程序
status exp(FILE* fp, CTree& T, int endsym);//语法单位<表达式>子程序
char precede(char* a, char* b);//比较a与b的优先级
status PrintTree(char* data, int indent); //打印缩进
