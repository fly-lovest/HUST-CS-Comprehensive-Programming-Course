#pragma once
//预处理
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef int status;
#define OK 1
#define ERROR 0
typedef struct define_data {  //用于储存define宏定义数据
	char ident[50];
	char string[50];
}define_data;

typedef struct include_data {  //用于储存include头文件定义数据
	char string[50];
}include_data;

extern define_data data_Def[10];//用于储存define宏定义的内容
extern include_data data_Inculd[10];//用于储存include文件包含的内容
extern int data_Def_num;
status pre_process(FILE* fp);