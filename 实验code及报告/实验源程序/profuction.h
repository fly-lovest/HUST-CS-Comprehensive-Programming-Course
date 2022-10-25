#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stack>
#include<queue>
using namespace std;

#define OK 1
#define ERROR 0
#define INFEASIBLE -1
typedef int status; //函数返回值类型


/*树的相关说明：*/
typedef struct Child		//孩子结点
{
	int child;
	Child* next;
}*CNode;
typedef struct CTNode
{
	char* data;
	int indent;		//该结点的孩子要增加的缩进量
	CNode firstchild;	//孩子链表头结点
}CTNode;
typedef struct CTree
{
	CTNode nodes[1000];
	int n, r;				//结点数和根的位置
}CTree;

status InitTree(CTree& T);
status InsertChild(CTree& T, int p, int i, CTree c);
status GetParent(CTree T, int child, int& parent);
status TraverseTree(CTree T, status(*visit)(char*, int));



/*栈的相关说明*/
typedef CTree* SElemType;	//定义元素类型
typedef struct
{
	SElemType* base;			//在栈构造之前和销毁之后，base的值为NULL
	SElemType* top;			    //栈顶指针
	int stacksize;				//当前已分配的存储空间，以元素为单位
}SqStack;
status InitStack(SqStack& S);
status GetTop(SqStack S, SElemType& e);
status Push(SqStack& S, SElemType e);
status Pop(SqStack& S, SElemType& e);

