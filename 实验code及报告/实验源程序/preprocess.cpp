#include"preprocess.h"
#include"lexer.h"
define_data data_Def[10];//���ڴ���define�궨������ݣ�ȫ��
include_data data_Inculd[10];//���ڴ���include�ļ����������ݣ�ȫ��
int data_Def_num;//�궨�����

status pre_process(FILE* fp) {
	int w; //����gettoken��ȡ�ķ���ֵ
	int i=0,j=0,m;//i�Ǻ궨�������j��include����
	int pre_line_num=1;//���ڼ�¼�������
	char container;//��ʱ�洢�ַ��жϽ�β���ķֺ�
	int a, b;//�Ƚ�����
	int flag=0;//�ж�������Ƿ����define�Ķ���
	FILE* mid_fp;
	char filename[50];
	strcpy(filename, "C_mid_file.txt"); //�м��ļ�
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

			if (w == IDENT) { //�Ǳ�ʶ��ʱ���ж��ǲ���define������
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