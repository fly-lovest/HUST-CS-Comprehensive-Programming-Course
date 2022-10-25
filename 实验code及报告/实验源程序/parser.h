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
	int indent;						//��¼����ֵ
	int linenum;					//��¼�������к�
};
typedef int status;
extern int indent0;					//��¼Դ���������ֵ
extern queue<print> printList;		//�洢������ӡ��ʽ��Ԫ
extern int w;						//���gettoken�����ķ���ֵ������ĵ����������
extern char kind[100];			    //��ȡ���͹ؼ���
extern char tokenText0[100];		//��ȡ������������

status program(FILE* fp, CTree& T); //�﷨��λ<����>���ӳ���
status ExtDefList(FILE* fp, CTree& T);//�﷨��λ<�ⲿ��������>���ӳ���
status ExtDef(FILE* fp, CTree& T);//�﷨��λ<�ⲿ����>���ӳ���
status ExtVarDef(FILE* fp, CTree& T);//�﷨��λ<�ⲿ��������>�ӳ���
status VarList(FILE* fp, CTree& T);//�﷨��λ<��������>�ӳ���
status funcDef(FILE* fp, CTree& T);//�﷨��λ<��������>�ӳ���
status ParameList(FILE* fp, CTree& T);//�﷨��λ<�β�����>�ӳ���
status FormParDef(FILE* fp, CTree& T);//�﷨��λ<�βζ���>�ӳ���
status CompStat(FILE* fp, CTree& T);//�﷨��λ<�������>�ӳ���
status LocVarList(FILE* fp, CTree& T);//�﷨��λ<�ֲ�������������>�ӳ���
status LocVarDef(FILE* fp, CTree& T);//�﷨��λ<�ֲ���������>�ӳ���
status StatList(FILE* fp, CTree& T);//�﷨��λ<�������>�ӳ���
status Statement(FILE* fp, CTree& T);//�﷨��λ<���>�ӳ���
status exp(FILE* fp, CTree& T, int endsym);//�﷨��λ<���ʽ>�ӳ���
char precede(char* a, char* b);//�Ƚ�a��b�����ȼ�
status PrintTree(char* data, int indent); //��ӡ����
