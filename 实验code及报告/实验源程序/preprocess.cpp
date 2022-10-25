#include"preprocess.h"
#include"lexer.h"
define_data data_Def[10];//用于储存define宏定义的内容，全局
include_data data_Inculd[10];//用于储存include文件包含的内容，全局
int data_Def_num;//宏定义个数

status pre_process(FILE* fp) {
	int w; //接受gettoken读取的返回值
	int i=0,j=0,m;//i是宏定义个数，j是include个数
	int pre_line_num=1;//用于记录换行情况
	char container;//暂时存储字符判断结尾处的分号
	int a, b;//比较行数
	int flag=0;//判断语句中是否出现define的定义
	FILE* mid_fp;
	char filename[50];
	strcpy(filename, "C_mid_file.txt"); //中间文件
	mid_fp = fopen(filename, "w");
	w = gettoken(fp);
	do {
		if (w == POUND) {
			w = gettoken(fp);
			if (w == DEFINE) {
				w = gettoken(fp);
				a = line_num;
				if (w == ERROR_TOKEN)return ERROR;
				else if (w == SEMI)return ERROR;
				else if (w == POUND)return ERROR;
				else {
					strcpy(data_Def[i].ident, token_text);
				}
				w = gettoken(fp);
				b = line_num;
				if (w == ERROR_TOKEN)return ERROR;
				else if (w == SEMI)return ERROR;
				else if (w == POUND)return ERROR;
				else {
					strcpy(data_Def[i++].string, token_text);
				}
				if (a != b)return ERROR;
				data_Def_num = i;
				fprintf(mid_fp, "\n");
				w = gettoken(fp); 
				pre_line_num = line_num;
				continue;
			}
			else if (w == INCLUDE) {
				w = gettoken(fp);
				if (w == ERROR_TOKEN) return ERROR;
				else if(w== STRING_CONST){ 
					strcpy(data_Inculd[j++].string, token_text); 
					if ((container = fgetc(fp)) != ';') { ungetc(container, fp); fprintf(mid_fp, "\n"); w = gettoken(fp); pre_line_num = line_num; continue; }
					else return ERROR;
				}
				else if (w == LESS) {
					w = gettoken(fp);
					a = line_num;
					if (w != IDENT)return ERROR;
					else {
						w = gettoken(fp);
						b = line_num;
						if (w != MORE)return ERROR;
						if (a != b)return ERROR;
						if ((container = fgetc(fp)) != ';') { ungetc(container, fp); fprintf(mid_fp, "\n"); w = gettoken(fp); pre_line_num = line_num; continue; }
						else return ERROR;
					}
				}
			}
			else return ERROR;
		}
		data_Def_num = i;

		if (w != POUND) {

			if (w == IDENT) { //是标识符时，判断是不是define的类型
				for (m = 0; m < data_Def_num; m++) {
					if (!strcmp(token_text, data_Def[m].ident)) {
						if (pre_line_num != line_num) {
							fprintf(mid_fp, "\n%s ", data_Def[m].string);
							flag = 1;
						}
						else { fprintf(mid_fp, "%s ", data_Def[m].string); flag = 1; }
					}
				}
				if (flag != 0) {
					flag = 0;
				}
				else {
					if (pre_line_num != line_num) {
						fprintf(mid_fp, "\n%s ", token_text);
					}
					else { fprintf(mid_fp, "%s ", token_text); }
				}
			}
			else if (w == LINENOTE) {
				pre_line_num = line_num;
				w = gettoken(fp); 
				continue;
			}
			else if (w == BLOCKNOTE) {
			
				for(m=0;m<line_num-pre_line_num;m++) fprintf(mid_fp, "\n");
				pre_line_num = line_num;
				w = gettoken(fp);
				continue;
			}
			else if (w == ERROR_TOKEN) {
				if (pre_line_num != line_num) {
					fprintf(mid_fp, "\n%s", token_text);
				}
				else { fprintf(mid_fp, "%s", token_text); }
			}
			else {
				if (pre_line_num != line_num) {
					fprintf(mid_fp, "\n%s ", token_text);
				}
				else { fprintf(mid_fp, "%s ", token_text); }
			}
		}
		pre_line_num = line_num;

		w = gettoken(fp);
	} while (w != EOF);
	fclose(mid_fp);
	return OK;
}