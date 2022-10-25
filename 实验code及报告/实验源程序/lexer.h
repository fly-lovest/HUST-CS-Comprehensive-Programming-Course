#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

enum token_kind{AUTO,BREAK,CASE,CHAR,CONST,CONTINUE,DEFAULT,DO,
DOUBLE,ELSE,ENUM,EXTERN,FLOAT,FOR,GOTO,IF,
INT,LONG,REGISTER,RETURN,SHORT,SIGNED,SIZEOF,STATIC,
STRUCT,SWITCH,TYPEDEF,UNION,UNSIGNED,VOID,VOLATILE,WHILE, INCLUDE, DEFINE,      //以上为关键字
IDENT, //标识符
INT_CONST, UNSIGNED_CONST, LONG_CONST, UNSIGNED_LONG_CONST, DOUBLE_CONST, FLOAT_CONST,
LONG_DOUBLE_CONST, CHAR_CONST, STRING_CONST,  //常量
PLUS,MINUS, MULTIPLY, DIVIDE, MOD,//双目运算符
MORE,LESS,EQUAL,UNEQUAL,MORE_EQUAL,LESS_EQUAL, //关系运算符
AND,OR,
EQUAL_TO,  //赋值运算符
LL,RL,LM,RM,LS,RS, //括号（L大，M中，S小）
LINENOTE,BLOCKNOTE,SEMI,COMMA,ERROR_TOKEN,POUND  //其他单词,其中SEMI为分号，COMMA为逗号,POUND为井号,ERROR_TOKEN标识错误

};

typedef struct keyword { //处理关键字所构建的结构
	char key[10];
	int enum_key;
}keyword;

extern keyword n[IDENT]; //储存各类关键字
extern char token_text[100]; //暂存常量
extern int line_num; //检测运行行数
int gettoken(FILE* fp);
