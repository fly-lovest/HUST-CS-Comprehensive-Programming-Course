#include"lexer.h"
#include"parser.h"
#include"profuction.h"
#include"preprocess.h"
#include"printfile.h"

int main() {
	char filename[30];
	int ch = 1;
	int w;
	CTree T;
	FILE* fp,*mid_fp;
	extern define_data data_Def[10];//用于储存define宏定义的内容
	extern int data_Def_num;
	extern int line_num;
	extern char token_text[100];
	int error_line[100];//记录错误行数
	int error_line_num=0;//记录错误总个数
	int i;//用于循环
Choosefile:
	system("cls");
	printf("输入文件名：");
	scanf("%s", filename);
	if (!(fp = fopen(filename, "r")))
	{
		printf("...选择文件错误，请重新选择...\n");
		getchar(); getchar();
		goto Choosefile;
	}
	else fclose(fp);
	
	while (ch)
	{
		system("cls");
		printf("基于高级语言源程序格式处理工具菜单\n1. 词法分析\n2. 语法分析\n3. 缩进编排\n4. 选择文件\n0. 退出程序\n请选择：");
		scanf("%d", &ch);
		system("cls");
		switch (ch)
		{
		case 1:
			error_line_num = 1;
			line_num = 1;  //行数初始化为1
			if (!(fp = fopen(filename, "r"))) 
			{ printf("...选择文件错误，请重新选择...\n"); 
			getchar(); getchar(); 
			break; 
			}
			printf("...正在预编译文件...\n");
			if (pre_process(fp))
			{
				printf("预编译成功！按任意键继续...\n");
				getchar(); getchar();
			}
			else
			{
				printf("预编译失败！退出系统！请检查错误！\n");
				getchar(); getchar();
				return 0;
			}
			mid_fp = fopen("C_mid_file.txt", "r");
			line_num = 1;  //行数初始化为1
			system("cls");
			printf("\n");
			printf("	单词类别				单词值\n");
			while (!feof(mid_fp))
			{
				w = gettoken(mid_fp);
				if (w >= AUTO && w <= DEFINE)
				{
					printf("	关键字					%s\n", token_text);
				}
				switch (w)
				{
				case POUND:
					printf("	井号					%s\n", token_text);
					break;
				case IDENT:
					printf("	标识符					%s\n", token_text);
					break;
				case INT_CONST:
					printf("	整型常量				%s\n", token_text);
					break;
				case UNSIGNED_CONST:
					printf("	无符号整型常量			         %s\n", token_text);
					break;
				case LONG_CONST:
					printf("	长整型常量				%s\n", token_text);
					break;
				case UNSIGNED_LONG_CONST:
					printf("	无符号长整型常量		         %s\n", token_text);
					break;
				case DOUBLE_CONST:
					printf("	双精度浮点型常量	    	        %s\n", token_text);
					break;
				case FLOAT_CONST:
					printf("	浮点型常量				%s\n", token_text);
					break;
				case LONG_DOUBLE_CONST:
					printf("	长双精度浮点型常量		         %s\n", token_text);
					break;
				case CHAR_CONST:
					printf("	字符型常量				%s\n", token_text);
					break;
				case STRING_CONST:
					printf("	字符串常量				%s\n", token_text);
					break;
				case EQUAL_TO:
					printf("	赋值运算符				=\n");
					break;
				case PLUS:
					printf("	加法运算符				+\n");
					break;
				case MINUS:
					printf("	减法运算符				-\n");
					break;
				case MULTIPLY:
					printf("	乘法运算符				*\n");
					break;
				case DIVIDE:
					printf("	除法运算符				/\n");
					break;
				case MOD:
					printf("	取模运算符				%%\n");
					break;
				case MORE:
					printf("	关系运算符				>\n");
					break;
				case LESS:
					printf("	关系运算符				<\n");
					break;
				case EQUAL:
					printf("	关系运算符				==\n");
					break;
				case UNEQUAL:
					printf("	关系运算符				!=\n");
					break;
				case MORE_EQUAL:
					printf("	关系运算符				>=\n");
					break;
				case LESS_EQUAL:
					printf("	关系运算符				<=\n");
					break;
				case AND:
					printf("	逻辑与					&&\n");
					break;
				case OR:
					printf("	逻辑或					||\n");
					break;
				case LS:
					printf("	左小括号				(\n");
					break;
				case RS:
					printf("	右小括号				)\n");
					break;
				case LM:
					printf("	左中括号				[\n");
					break;
				case RM:
					printf("	右中括号				]\n");
					break;
				case LL:
					printf("	左大括号				{\n");
					break;
				case RL:
					printf("	右大括号				}\n");
					break;
				case SEMI:
					printf("	分号					;\n");
					break;
				case COMMA:
					printf("	逗号					,\n");
					break;
				case ERROR_TOKEN:
					error_line[error_line_num] = line_num;
					error_line_num++;
					break;
				}
			}
			printf("\n");
			printf("错误列表 错误总数%d\n", error_line_num);
			if (error_line_num)
			{
				printf("\n");
				printf("序号	行数\n");
				for (i = 0; i < error_line_num; i++) {
					printf("   %d       %d\n", i+1, error_line[i]);
				}
			}
			fclose(fp);
			fclose(mid_fp);
			printf("\n按任意键继续...");
			getchar();
			break;
		case 2:
			printf("...正在预编译文件...\n");
			line_num = 1;  //行数初始化为1
			fp = fopen(filename, "r");
			if (pre_process(fp))
			{
				printf("预编译成功！按任意键继续...\n");
				getchar(); getchar();
			}
			else
			{
				printf("预编译失败！退出系统！请检查错误！\n");
				getchar(); getchar();
				return 0;
			}
			mid_fp = fopen("C_mid_file.txt", "r");
			line_num = 1;  //行数初始化为1
			system("cls");
			if (!program(mid_fp, T))
			{
				printf("发生生成错误！错误行号：%d\n", line_num);
				printf("按任意键继续...\n");
				getchar(); 
				break;
			}
			TraverseTree(T, PrintTree);
			printf("\n按任意键继续...\n");
			fclose(fp);
			fclose(mid_fp);
			getchar();
			break;
		case 3:
			line_num = 1;  //行数初始化为1
			fp = fopen(filename, "r");
			if (!pre_process(fp))
			{
				printf("预编译失败！退出系统！请检查错误！\n");
				getchar(); getchar();
				return 0;
			}
			line_num = 1;
			mid_fp = fopen("C_mid_file.txt", "r");
			if (!program(mid_fp, T))
			{
				printf("程序存在语法错误！无法缩进打印！\n");
				getchar(); getchar();
				break;
			}
			else { printf("程序语法正确！\n"); }
			fclose(fp);
			fp = fopen(filename, "r");
			PrintFile(fp);
			printf("缩进编排文件生成成功！\n");
			getchar(); getchar();
			fclose(fp);
			fclose(mid_fp);
			break;
		case 4:
			goto Choosefile;

		default:
			return 0;
		}
	}
	return 1;

}