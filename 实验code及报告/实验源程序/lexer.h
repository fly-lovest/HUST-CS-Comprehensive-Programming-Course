#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

enum token_kind{AUTO,BREAK,CASE,CHAR,CONST,CONTINUE,DEFAULT,DO,
DOUBLE,ELSE,ENUM,EXTERN,FLOAT,FOR,GOTO,IF,
INT,LONG,REGISTER,RETURN,SHORT,SIGNED,SIZEOF,STATIC,
STRUCT,SWITCH,TYPEDEF,UNION,UNSIGNED,VOID,VOLATILE,WHILE, INCLUDE, DEFINE,      //����Ϊ�ؼ���
IDENT, //��ʶ��
INT_CONST, UNSIGNED_CONST, LONG_CONST, UNSIGNED_LONG_CONST, DOUBLE_CONST, FLOAT_CONST,
LONG_DOUBLE_CONST, CHAR_CONST, STRING_CONST,  //����
PLUS,MINUS, MULTIPLY, DIVIDE, MOD,//˫Ŀ�����
MORE,LESS,EQUAL,UNEQUAL,MORE_EQUAL,LESS_EQUAL, //��ϵ�����
AND,OR,
EQUAL_TO,  //��ֵ�����
LL,RL,LM,RM,LS,RS, //���ţ�L��M�У�SС��
LINENOTE,BLOCKNOTE,SEMI,COMMA,ERROR_TOKEN,POUND  //��������,����SEMIΪ�ֺţ�COMMAΪ����,POUNDΪ����,ERROR_TOKEN��ʶ����

};

typedef struct keyword { //����ؼ����������Ľṹ
	char key[10];
	int enum_key;
}keyword;

extern keyword n[IDENT]; //�������ؼ���
extern char token_text[100]; //�ݴ泣��
extern int line_num; //�����������
int gettoken(FILE* fp);
