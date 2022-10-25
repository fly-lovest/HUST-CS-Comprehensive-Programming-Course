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
	extern define_data data_Def[10];//���ڴ���define�궨�������
	extern int data_Def_num;
	extern int line_num;
	extern char token_text[100];
	int error_line[100];//��¼��������
	int error_line_num=0;//��¼�����ܸ���
	int i;//����ѭ��
Choosefile:
	system("cls");
	printf("�����ļ�����");
	scanf("%s", filename);
	if (!(fp = fopen(filename, "r")))
	{
		printf("...ѡ���ļ�����������ѡ��...\n");
		getchar(); getchar();
		goto Choosefile;
	}
	else fclose(fp);
	
	while (ch)
	{
		system("cls");
		printf("���ڸ߼�����Դ�����ʽ�����߲˵�\n1. �ʷ�����\n2. �﷨����\n3. ��������\n4. ѡ���ļ�\n0. �˳�����\n��ѡ��");
		scanf("%d", &ch);
		system("cls");
		switch (ch)
		{
		case 1:
			error_line_num = 1;
			line_num = 1;  //������ʼ��Ϊ1
			if (!(fp = fopen(filename, "r"))) 
			{ printf("...ѡ���ļ�����������ѡ��...\n"); 
			getchar(); getchar(); 
			break; 
			}
			printf("...����Ԥ�����ļ�...\n");
			if (pre_process(fp))
			{
				printf("Ԥ����ɹ��������������...\n");
				getchar(); getchar();
			}
			else
			{
				printf("Ԥ����ʧ�ܣ��˳�ϵͳ���������\n");
				getchar(); getchar();
				return 0;
			}
			mid_fp = fopen("C_mid_file.txt", "r");
			line_num = 1;  //������ʼ��Ϊ1
			system("cls");
			printf("\n");
			printf("	�������				����ֵ\n");
			while (!feof(mid_fp))
			{
				w = gettoken(mid_fp);
				if (w >= AUTO && w <= DEFINE)
				{
					printf("	�ؼ���					%s\n", token_text);
				}
				switch (w)
				{
				case POUND:
					printf("	����					%s\n", token_text);
					break;
				case IDENT:
					printf("	��ʶ��					%s\n", token_text);
					break;
				case INT_CONST:
					printf("	���ͳ���				%s\n", token_text);
					break;
				case UNSIGNED_CONST:
					printf("	�޷������ͳ���			         %s\n", token_text);
					break;
				case LONG_CONST:
					printf("	�����ͳ���				%s\n", token_text);
					break;
				case UNSIGNED_LONG_CONST:
					printf("	�޷��ų����ͳ���		         %s\n", token_text);
					break;
				case DOUBLE_CONST:
					printf("	˫���ȸ����ͳ���	    	        %s\n", token_text);
					break;
				case FLOAT_CONST:
					printf("	�����ͳ���				%s\n", token_text);
					break;
				case LONG_DOUBLE_CONST:
					printf("	��˫���ȸ����ͳ���		         %s\n", token_text);
					break;
				case CHAR_CONST:
					printf("	�ַ��ͳ���				%s\n", token_text);
					break;
				case STRING_CONST:
					printf("	�ַ�������				%s\n", token_text);
					break;
				case EQUAL_TO:
					printf("	��ֵ�����				=\n");
					break;
				case PLUS:
					printf("	�ӷ������				+\n");
					break;
				case MINUS:
					printf("	���������				-\n");
					break;
				case MULTIPLY:
					printf("	�˷������				*\n");
					break;
				case DIVIDE:
					printf("	���������				/\n");
					break;
				case MOD:
					printf("	ȡģ�����				%%\n");
					break;
				case MORE:
					printf("	��ϵ�����				>\n");
					break;
				case LESS:
					printf("	��ϵ�����				<\n");
					break;
				case EQUAL:
					printf("	��ϵ�����				==\n");
					break;
				case UNEQUAL:
					printf("	��ϵ�����				!=\n");
					break;
				case MORE_EQUAL:
					printf("	��ϵ�����				>=\n");
					break;
				case LESS_EQUAL:
					printf("	��ϵ�����				<=\n");
					break;
				case AND:
					printf("	�߼���					&&\n");
					break;
				case OR:
					printf("	�߼���					||\n");
					break;
				case LS:
					printf("	��С����				(\n");
					break;
				case RS:
					printf("	��С����				)\n");
					break;
				case LM:
					printf("	��������				[\n");
					break;
				case RM:
					printf("	��������				]\n");
					break;
				case LL:
					printf("	�������				{\n");
					break;
				case RL:
					printf("	�Ҵ�����				}\n");
					break;
				case SEMI:
					printf("	�ֺ�					;\n");
					break;
				case COMMA:
					printf("	����					,\n");
					break;
				case ERROR_TOKEN:
					error_line[error_line_num] = line_num;
					error_line_num++;
					break;
				}
			}
			printf("\n");
			printf("�����б� ��������%d\n", error_line_num);
			if (error_line_num)
			{
				printf("\n");
				printf("���	����\n");
				for (i = 0; i < error_line_num; i++) {
					printf("   %d       %d\n", i+1, error_line[i]);
				}
			}
			fclose(fp);
			fclose(mid_fp);
			printf("\n�����������...");
			getchar();
			break;
		case 2:
			printf("...����Ԥ�����ļ�...\n");
			line_num = 1;  //������ʼ��Ϊ1
			fp = fopen(filename, "r");
			if (pre_process(fp))
			{
				printf("Ԥ����ɹ��������������...\n");
				getchar(); getchar();
			}
			else
			{
				printf("Ԥ����ʧ�ܣ��˳�ϵͳ���������\n");
				getchar(); getchar();
				return 0;
			}
			mid_fp = fopen("C_mid_file.txt", "r");
			line_num = 1;  //������ʼ��Ϊ1
			system("cls");
			if (!program(mid_fp, T))
			{
				printf("�������ɴ��󣡴����кţ�%d\n", line_num);
				printf("�����������...\n");
				getchar(); 
				break;
			}
			TraverseTree(T, PrintTree);
			printf("\n�����������...\n");
			fclose(fp);
			fclose(mid_fp);
			getchar();
			break;
		case 3:
			line_num = 1;  //������ʼ��Ϊ1
			fp = fopen(filename, "r");
			if (!pre_process(fp))
			{
				printf("Ԥ����ʧ�ܣ��˳�ϵͳ���������\n");
				getchar(); getchar();
				return 0;
			}
			line_num = 1;
			mid_fp = fopen("C_mid_file.txt", "r");
			if (!program(mid_fp, T))
			{
				printf("��������﷨�����޷�������ӡ��\n");
				getchar(); getchar();
				break;
			}
			else { printf("�����﷨��ȷ��\n"); }
			fclose(fp);
			fp = fopen(filename, "r");
			PrintFile(fp);
			printf("���������ļ����ɳɹ���\n");
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