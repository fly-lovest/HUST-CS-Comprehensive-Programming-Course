#pragma once
//Ԥ����
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef int status;
#define OK 1
#define ERROR 0
typedef struct define_data {  //���ڴ���define�궨������
	char ident[50];
	char string[50];
}define_data;

typedef struct include_data {  //���ڴ���includeͷ�ļ���������
	char string[50];
}include_data;

extern define_data data_Def[10];//���ڴ���define�궨�������
extern include_data data_Inculd[10];//���ڴ���include�ļ�����������
extern int data_Def_num;
status pre_process(FILE* fp);