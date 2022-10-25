#include"parser.h"

int w;  //���gettoken�����ķ���ֵ������ĵ����������
char kind[100]; //�������͹ؼ���
char tokenText0[100]; //�����һ�����������߱�����
int indent0 = 0;  //��ʼ������ֵ
queue<print> printList; //���ڷ����ӡ����

status program(FILE* fp, CTree& T)  //�﷨��λ<����>���ӳ���
{
	CTree c;
	struct print elem = { indent0,line_num };
	printList.push(elem);//������������ֵ
	w = gettoken(fp);
	if (!ExtDefList(fp, c)) return ERROR;  //�����ⲿ�������к���
	T.n = 1; T.r = 0;
	T.nodes[0].data = (char*)malloc((strlen("����") + 1) * sizeof(char)); //�����﷨���ĸ����nodes[0]
	strcpy(T.nodes[0].data, "����");
	T.nodes[0].indent = 0;
	T.nodes[0].firstchild = NULL;
	InsertChild(T, T.r, 1, c); //��c�������뵽�﷨����
	return OK;
}

status ExtDefList(FILE* fp, CTree& T) //�﷨��λ<�ⲿ��������>���ӳ���
{
	CTree c; 
	status flag;//�鿴�Ƿ����ڶ�������
	if (w == EOF) return INFEASIBLE;
	T.n = 1;  T.r = 0;
	T.nodes[0].data = (char*)malloc((strlen("�ⲿ��������") + 1) * sizeof(char)); //�����ⲿ����������
	strcpy(T.nodes[0].data, "�ⲿ��������");
	T.nodes[0].indent = 0;
	T.nodes[0].firstchild = NULL;
	if (!ExtDef(fp, c)) return ERROR;
	InsertChild(T, T.r, 1, c); //����һ���ⲿ���壬�õ�һ����������Ϊ���ĵ�һ������
	flag = ExtDefList(fp, c);
	if (flag == OK) InsertChild(T, T.r, 2, c); //�õ�����������Ϊ���ĵڶ�������
	if (flag==ERROR) return ERROR;
	return OK;
}

status ExtDef(FILE* fp, CTree& T)  //�﷨��λ<�ⲿ����>���ӳ���
{
	status flag;
	if (w != INT && w != LONG && w != SHORT && w != SIGNED && w != UNSIGNED &&
		w != FLOAT && w != DOUBLE && w != CHAR &&w!=VOID) return ERROR;
	strcpy(kind, token_text);			//�������͹ؼ���
	w = gettoken(fp);
	if (w != IDENT) return ERROR;
	strcpy(tokenText0, token_text);		//�����һ����������������tokenText0
	w = gettoken(fp);
	if (w != LS) flag = ExtVarDef(fp, T);	//�����ⲿ���������ӳ���
	else flag = funcDef(fp, T);			//���ú��������ӳ���
	if (!flag) return ERROR;
	return OK;
}

status ExtVarDef(FILE* fp, CTree& T)  //�﷨��λ<�ⲿ��������>�ӳ���
{
	CTree c; CTree p;
	T.n = 1; T.r = 0;
	T.nodes[0].data = (char*)malloc((strlen("�ⲿ�������壺") + 1) * sizeof(char)); //�����ⲿ����������
	strcpy(T.nodes[0].data, "�ⲿ�������壺");
	T.nodes[0].indent = 1;
	T.nodes[0].firstchild = NULL;
	c.n = 1; c.r = 0;   //�����ⲿ�������ͽ��
	c.nodes[0].data = (char*)malloc((strlen(kind) + strlen("���ͣ�") + 1) * sizeof(char));
	strcpy(c.nodes[0].data, "���ͣ�");
	strcat(c.nodes[0].data, kind);
	c.nodes[0].indent = 1;
	c.nodes[0].firstchild = NULL;
	if (!InsertChild(T, T.r, 1, c)) return ERROR;		//c��ΪT�ĵ�һ������
	if (!VarList(fp, p)) return ERROR;
	if (!InsertChild(T, T.r, 2, p)) return ERROR;		//p��ΪT�ĵڶ�������
	return OK;
}

status VarList(FILE* fp, CTree& T)  //�﷨��λ<��������>�ӳ���
{
	CTree c; CTree t; //c�����������˱�����㣬t�������������ܴ��ڵ���һ�������
	T.n = 1; T.r = 0;//���ɱ������н��
	T.nodes[0].data = (char*)malloc((strlen("��������") + 1) * sizeof(char));
	strcpy(T.nodes[0].data, "��������");
	T.nodes[0].indent = 0;
	T.nodes[0].firstchild = NULL;
	if (w == LM)     //����������������Ǳ���
	{
		strcat(tokenText0, "[");  //ʶ�����飬ֻ֧��һά����
		if ((w = gettoken(fp)) == INT_CONST)
		{
			strcat(tokenText0, token_text);
			if ((w = gettoken(fp)) == RM)
			{
				strcat(tokenText0, "]");  //ʶ������ɹ�
				c.n = 1; c.r = 0;   //����������
				c.nodes[0].data = (char*)malloc((strlen(tokenText0) + strlen("Array: ") + 1) * sizeof(char));
				strcpy(c.nodes[0].data, "Array: ");
				strcat(c.nodes[0].data, tokenText0); // ��ʼʱ��tokenText0�����˵�һ��������
				c.nodes[0].indent = 1;
				c.nodes[0].firstchild = NULL;
				w = gettoken(fp);
			}
			else return ERROR;
		}
		else return ERROR;
	}
	else
	{
		c.n = 1; c.r = 0;  //�����������
		c.nodes[0].data = (char*)malloc((strlen(tokenText0) + strlen("ID: ") + 1) * sizeof(char));
		strcpy(c.nodes[0].data, "ID: ");
		strcat(c.nodes[0].data, tokenText0);
		c.nodes[0].indent = 1;
		c.nodes[0].firstchild = NULL;
	}
	if (!InsertChild(T, T.r, 1, c))	return ERROR;//ʶ��ı��������ΪT�ĵ�һ������
	if (w != COMMA && w != SEMI) return ERROR;
	if (w == SEMI)								//�����ʶ�����Ƿֺţ�ֱ�ӽ���
	{
		w = gettoken(fp);
		return OK;
	}
	w = gettoken(fp);
	if (w != IDENT) return ERROR;	//���w���Ǳ�ʶ���򱨴���֮���滹�еڶ�������
	strcpy(tokenText0, token_text);
	w = gettoken(fp);
	if (!VarList(fp, t)) return ERROR;
	if (!InsertChild(T, T.r, 2, t)) return ERROR;
	return OK;
}

status funcDef(FILE* fp, CTree& T)  //�﷨��λ<��������>�ӳ���
{
	CTree c; CTree p; //c���Ϊ��������ֵ���ͣ�p���Ϊ���������
	CTree q; CTree f; CTree s; //q���Ϊ�������н��,fΪ�������㣬sΪ���ܵĸ��������
	T.n = 1; T.r = 0;		//���ɺ���������T
	T.nodes[0].data = (char*)malloc((strlen("�������壺") + 1) * sizeof(char));
	strcpy(T.nodes[0].data, "�������壺");
	T.nodes[0].indent = 1;
	T.nodes[0].firstchild = NULL;
	c.n = 1; c.r = 0;  //���ɺ�������ֵ���ͽ��c
	c.nodes[0].data = (char*)malloc((strlen(kind) + strlen("���ͣ�") + 1) * sizeof(char));
	strcpy(c.nodes[0].data, "���ͣ�");
	strcat(c.nodes[0].data, kind);
	c.nodes[0].indent = 1;
	c.nodes[0].firstchild = NULL;
	if (!InsertChild(T, T.r, 1, c)) return ERROR;
	w = gettoken(fp);
	//���������ڿ����޲�����������void�������ǲ������У������������
	if (w != RS && w != VOID && w != INT && w != LONG && w != SHORT && w != SIGNED && w != UNSIGNED && w != FLOAT && w != DOUBLE && w != CHAR)
		return ERROR;
	p.n = 1; p.r = 0;		//���ɺ��������
	p.nodes[0].data = (char*)malloc((strlen(tokenText0) + strlen("��������") + 1) * sizeof(char));
	strcpy(p.nodes[0].data, "��������");
	strcat(p.nodes[0].data, tokenText0);
	p.nodes[0].indent = 1;
	p.nodes[0].firstchild = NULL;
	if (w == RS || w == VOID) //�ж�void���������������
	{
		if (w == VOID)
		{
			w = gettoken(fp);
			if (w != RS) return ERROR;
		}
	}
	else
	{
		if (!ParameList(fp, q)) return ERROR;
		if (!InsertChild(p, p.r, 1, q)) return ERROR;
	}
	if (!InsertChild(T, T.r, 2, p)) return ERROR;
	w = gettoken(fp);
	if (w != SEMI && w != LL) return ERROR;
	f.n = 1; f.r = 0;  //���ɺ�������
	f.nodes[0].data = (char*)malloc((strlen("�����壺") + 1) * sizeof(char));
	strcpy(f.nodes[0].data, "�����壺");
	f.nodes[0].indent = 1;
	f.nodes[0].firstchild = NULL;
	if (w == LL)  //���ں��������жϸ������
	{
		if (!CompStat(fp, s)) return ERROR;
		if (!InsertChild(f, f.r, 1, s)) return ERROR;
		if(!InsertChild(T, T.r, 3, f))return ERROR;
	}
//���Ǻ���������ֱ�ӷ���OK
	return OK;
}

status ParameList(FILE* fp, CTree& T)  //�﷨��λ<�β�����>�ӳ���
{
	CTree c; //c���������β�����
	CTree p; //p�������ɿ��ܳ��ֵ���һ���β�����
	T.n = 1; T.r = 0;  //�����β����н��
	T.nodes[0].data = (char*)malloc((strlen("�β�����") + 1) * sizeof(char));
	strcpy(T.nodes[0].data, "�β�����");
	T.nodes[0].indent = 0;
	T.nodes[0].firstchild = NULL;
	if (!FormParDef(fp, c)) return ERROR;
	if (!InsertChild(T, T.r, 1, c)) return ERROR;
	w = gettoken(fp);
	if (w != RS && w != COMMA) return ERROR;
	if (w == COMMA)
	{
		w = gettoken(fp);
		if (!ParameList(fp, p)) return ERROR;
		InsertChild(T, T.r, 2, p);
	}
	return OK;
}

status FormParDef(FILE* fp, CTree& T)  //�﷨��λ<�β�>�ӳ���
{
//w��ǰ�Ѷ�ȡ��һ���β�����
	CTree c; //���������β����ͽ��
	CTree p;  //���������βα������
	T.n = 1; T.r = 0;	//�����βν��
	T.nodes[0].data = (char*)malloc((strlen("�βΣ�") + 1) * sizeof(char));
	strcpy(T.nodes[0].data, "�βΣ�");
	T.nodes[0].indent = 1;
	T.nodes[0].firstchild = NULL;	
	if (w != INT && w != LONG && w != SHORT && w != SIGNED && w != UNSIGNED &&
		w != FLOAT && w != DOUBLE && w != CHAR) return ERROR;
	c.n = 1; c.r = 0;		//�����β����ͽ��
	c.nodes[0].data = (char*)malloc((strlen(token_text) + strlen("���ͣ�") + 1) * sizeof(char));
	strcpy(c.nodes[0].data, "���ͣ�");
	strcat(c.nodes[0].data, token_text);
	c.nodes[0].indent = 1;
	c.nodes[0].firstchild = NULL;
	InsertChild(T, T.r, 1, c);
	w = gettoken(fp);
	if (w != IDENT) return ERROR;
	p.n = 1; p.r = 0;   //�����βα������
	p.nodes[0].data = (char*)malloc((strlen(token_text) + strlen("ID: ") + 1) * sizeof(char));
	strcpy(p.nodes[0].data, "ID: ");
	strcat(p.nodes[0].data, token_text);
	p.nodes[0].indent = 1;
	p.nodes[0].firstchild = NULL;
	InsertChild(T, T.r, 2, p);
	return OK;
}

status CompStat(FILE* fp, CTree& T)  //�﷨��λ<�������>�ӳ���
{
	CTree c; CTree p; //c�������ɾֲ���������������p�������������������
	status flag;
	print elem;
	T.n = 1; T.r = 0;		//���ɸ��������
	T.nodes[0].data = (char*)malloc((strlen("������䣺") + 1) * sizeof(char));
	strcpy(T.nodes[0].data, "������䣺");
	T.nodes[0].indent = 1;
	T.nodes[0].firstchild = NULL;
	//ע�����оֲ�����˵����������ж�����Ϊ��
	w = gettoken(fp);
	elem = { ++indent0,line_num };
	printList.push(elem);  //�������ֵ
	if (w == INT || w == LONG || w == SHORT || w == SIGNED || w == UNSIGNED|| w == FLOAT || w == DOUBLE || w == CHAR)
	{
		if (!LocVarList(fp, c)) return ERROR;
		if (!InsertChild(T, T.r, 1, c)) return ERROR;
		flag = StatList(fp, p);
		if (!flag) return ERROR;
		if (flag == OK)
			if (!InsertChild(T, T.r, 2, p)) return ERROR;
	}
	else
	{
		flag = StatList(fp, p);
		if (!flag) return ERROR;
		if (flag == OK)
			if (!InsertChild(T, T.r, 1, p)) return ERROR;
	}
	elem = { --indent0,line_num };
	printList.push(elem);
	if (w != RL) return ERROR;
	w = gettoken(fp);
	return OK;
}

status LocVarList(FILE* fp, CTree& T)  //�﷨��λ<�ֲ�������������>�ӳ���
{
	CTree c; CTree p;//c���ɾֲ���������������p���ɿ��ܴ��ڵ���һ���ֲ�����������������
	status flag;
	if (w != INT && w != LONG && w != SHORT && w != SIGNED && w != UNSIGNED && w != FLOAT && w != DOUBLE && w != CHAR)
		return INFEASIBLE;
	//�����ĺ�̵��ʲ�Ϊ����˵����ʱ�������������н���
	T.n = 1;  T.r = 0;  //���ɾֲ������������н��
	T.nodes[0].data = (char*)malloc((strlen("�ֲ�������������") + 1) * sizeof(char));
	strcpy(T.nodes[0].data, "�ֲ�������������");
	T.nodes[0].indent = 0;
	T.nodes[0].firstchild = NULL;
	if (!LocVarDef(fp, c)) return ERROR;
	if (!InsertChild(T, T.r, 1, c)) return ERROR;
	flag = LocVarList(fp, p);
	if (flag == OK) InsertChild(T, T.r, 2, p);
	if (!flag) return ERROR;
	return OK;
}

status LocVarDef(FILE* fp, CTree& T)//�﷨��λ<�ֲ���������>�ӳ���
{
	CTree c;  CTree p; //c���ɾֲ��������ͽ��,p���ɱ�����������
	if (w != INT && w != LONG && w != SHORT && w != SIGNED && w != UNSIGNED && w != FLOAT && w != DOUBLE && w != CHAR) return ERROR;
	T.n = 1; T.r = 0;		//���ɾֲ�����������
	T.nodes[0].data = (char*)malloc((strlen("�ֲ��������壺") + 1) * sizeof(char));
	strcpy(T.nodes[0].data, "�ֲ��������壺");
	T.nodes[0].indent = 1;
	T.nodes[0].firstchild = NULL;	
	
	c.n = 1; c.r = 0;		//���ɾֲ��������ͽ��
	c.nodes[0].data = (char*)malloc((strlen(token_text) + strlen("���ͣ�") + 1) * sizeof(char));
	strcpy(c.nodes[0].data, "���ͣ�");
	strcat(c.nodes[0].data, token_text);
	c.nodes[0].indent = 1;
	c.nodes[0].firstchild = NULL;
	if (!InsertChild(T, T.r, 1, c)) return ERROR;
	w = gettoken(fp);
	if (w != IDENT) return ERROR;
	strcpy(tokenText0, token_text);
	w = gettoken(fp);
	if (!VarList(fp, p)) return ERROR;
	if (!InsertChild(T, T.r, 2, p)) return ERROR;
	return OK;
}

status StatList(FILE* fp, CTree& T)  //�﷨��λ<�������>�ӳ���
{
	CTree c; CTree p;  //c���������,p���ɿ��ܳ��ֵ����������
	status flag;
	flag = Statement(fp, c);
	if (flag == INFEASIBLE) return INFEASIBLE;
	if (!flag) return ERROR;
	else
	{
		T.n = 1; T.r = 0;		//����������н��
		T.nodes[0].data = (char*)malloc((strlen("�������") + 1) * sizeof(char));
		strcpy(T.nodes[0].data, "�������");
		T.nodes[0].indent = 0;
		T.nodes[0].firstchild = NULL;
		InsertChild(T, T.r, 1, c);
		flag = StatList(fp, p);
		if (!flag) return ERROR;
		if (flag == OK)
			if (!InsertChild(T, T.r, 2, p))
				return ERROR;
	}
	return OK;
}

status Statement(FILE* fp, CTree& T)  //�﷨��λ<���>�ӳ���
{
	CTree c; CTree p; CTree q; CTree k; 
	print elem;
	
	if (w == IF) {//�����������,p�������ɱ��ʽ��,q��������ifģ���Ӿ�����k��������elseģ���Ӿ���
		w = gettoken(fp);
		if (w != LS) return ERROR;
		w = gettoken(fp);
		if (w == RS) return ERROR;
		if (!exp(fp, p, RS)) return ERROR;
		c.n = 1; c.r = 0;  //����if�������
		c.nodes[0].data = (char*)malloc((strlen("������") + 1) * sizeof(char));
		strcpy(c.nodes[0].data, "������");
		c.nodes[0].indent = 1;
		c.nodes[0].firstchild = NULL;
		InsertChild(c, c.r, 1, p);
		w = gettoken(fp);
		if (w == LL)
		{
			if (!CompStat(fp, q)) return ERROR;
		}
		else
		{
			elem = { ++indent0,line_num };
			printList.push(elem);
			if (!Statement(fp, q)) return ERROR;
			elem = { --indent0,line_num };
			printList.push(elem);
		}
		p.n = 1; p.r = 0;
		p.nodes[0].data = (char*)malloc((strlen("IF�Ӿ䣺") + 1) * sizeof(char));
		strcpy(p.nodes[0].data, "IF�Ӿ䣺");
		p.nodes[0].indent = 1;
		p.nodes[0].firstchild = NULL;
		InsertChild(p, p.r, 1, q);
		if (w == ELSE)
		{
			w = gettoken(fp);
			if (w == LL)
			{
				if (!CompStat(fp, k)) return ERROR;
			}
			else
			{
				elem = { ++indent0,line_num };
				printList.push(elem);
				if (!Statement(fp, k)) return ERROR;
				elem = { --indent0,line_num };
				printList.push(elem);
			}
			q.n = 1; q.r = 0;  //����else�������
			q.nodes[0].data = (char*)malloc((strlen("ELSE�Ӿ䣺") + 1) * sizeof(char));
			strcpy(q.nodes[0].data, "ELSE�Ӿ䣺");
			q.nodes[0].indent = 1;
			q.nodes[0].firstchild = NULL;
			InsertChild(q, q.r, 1, k);
			T.n = 1; T.r = 0;
			T.nodes[0].data = (char*)malloc((strlen("if-else��䣺") + 1) * sizeof(char));
			strcpy(T.nodes[0].data, "if-else��䣺");
			T.nodes[0].indent = 1;
			T.nodes[0].firstchild = NULL;
			InsertChild(T, T.r, 1, c);
			InsertChild(T, T.r, 2, p);
			InsertChild(T, T.r, 3, q);
		}
		else
		{
			T.n = 1; T.r = 0;
			T.nodes[0].data = (char*)malloc((strlen("if��䣺") + 1) * sizeof(char));
			strcpy(T.nodes[0].data, "if��䣺");
			T.nodes[0].indent = 1;
			T.nodes[0].firstchild = NULL;
			InsertChild(T, T.r, 1, c);
			InsertChild(T, T.r, 2, p);
		}
		return OK;
	}
	else if (w == LL) {
		if (!CompStat(fp, T)) return ERROR;
		return OK;
	}
	else if (w == FOR) {
		//����for���
		T.n = 1; T.r = 0;
		T.nodes[0].data = (char*)malloc((strlen("for��䣺") + 1) * sizeof(char));
		strcpy(T.nodes[0].data, "for��䣺");
		T.nodes[0].indent = 1;
		T.nodes[0].firstchild = NULL;
		c.n = 1; c.r = 0;
		c.nodes[0].data = (char*)malloc((strlen("��ʼ���ʽ��") + 1) * sizeof(char));
		strcpy(c.nodes[0].data, "��ʼ���ʽ��");
		c.nodes[0].indent = 1;
		c.nodes[0].firstchild = NULL;
		InsertChild(T, T.r, 1, c);
		c.n = 1; c.r = 0;
		c.nodes[0].data = (char*)malloc((strlen("��ֹ������") + 1) * sizeof(char));
		strcpy(c.nodes[0].data, "��ֹ������");
		c.nodes[0].indent = 1;
		c.nodes[0].firstchild = NULL;
		InsertChild(T, T.r, 2, c);
		c.n = 1; c.r = 0;
		c.nodes[0].data = (char*)malloc((strlen("ѭ�����ʽ��") + 1) * sizeof(char));
		strcpy(c.nodes[0].data, "ѭ�����ʽ��");
		c.nodes[0].indent = 1;
		c.nodes[0].firstchild = NULL;
		InsertChild(T, T.r, 3, c);
		c.n = 1; c.r = 0;
		c.nodes[0].data = (char*)malloc((strlen("for�Ӿ䣺") + 1) * sizeof(char));
		strcpy(c.nodes[0].data, "for�Ӿ䣺");
		c.nodes[0].indent = 1;
		c.nodes[0].firstchild = NULL;
		InsertChild(T, T.r, 4, c);
		w = gettoken(fp);
		if (w != LS) return ERROR;
		w = gettoken(fp);
		if (!exp(fp, c, SEMI)) return ERROR;
		InsertChild(T, T.nodes[0].firstchild->child, 1, c);
		w = gettoken(fp);
		if (w == SEMI) return ERROR;
		if (!exp(fp, c, SEMI)) return ERROR;
		InsertChild(T, T.nodes[0].firstchild->next->child, 1, c);
		w = gettoken(fp);
		if (!exp(fp, c, RS)) return ERROR;
		InsertChild(T, T.nodes[0].firstchild->next->next->child, 1, c);
		w = gettoken(fp);
		if (w == LL)
		{
			if (!CompStat(fp, c)) return ERROR;
		}
		else
		{
			elem = { ++indent0,line_num };
			printList.push(elem);
			if (!Statement(fp, c)) return ERROR;
			elem = { --indent0,line_num };
			printList.push(elem);
		}
		InsertChild(T, T.nodes[0].firstchild->next->next->next->child, 1, c);
		return OK;
	}
	else if (w == WHILE) {
		w = gettoken(fp);
		if (w != LS) return ERROR;
		w = gettoken(fp);
		if (w == RS) return ERROR;
		if (!exp(fp, c, RS)) return ERROR;
		w = gettoken(fp);
		if (w == LL)
		{
			if (!CompStat(fp, p)) return ERROR;
		}
		else
		{
			elem = { ++indent0,line_num };
			printList.push(elem);
			if (!Statement(fp, p)) return ERROR;
			elem = { --indent0,line_num };
			printList.push(elem);
		}
		T.n = 1; T.r = 0;
		T.nodes[0].data = (char*)malloc((strlen("while��䣺") + 1) * sizeof(char));
		strcpy(T.nodes[0].data, "while��䣺");
		T.nodes[0].indent = 1;
		T.nodes[0].firstchild = NULL;
		InsertChild(T, T.r, 1, c);
		InsertChild(T, T.r, 2, p);
		return OK;
	}
	else if (w == CONTINUE) {
		T.n = 1; T.r = 0;
		T.nodes[0].data = (char*)malloc((strlen("continue��䣺") + 1) * sizeof(char));
		strcpy(T.nodes[0].data, "continue��䣺");
		T.nodes[0].indent = 1;
		T.nodes[0].firstchild = NULL;
		w = gettoken(fp);
		if (w != SEMI) return ERROR;
		w = gettoken(fp);
		return OK;
	}
	else if (w == BREAK) {
		T.n = 1; T.r = 0;
		T.nodes[0].data = (char*)malloc((strlen("break��䣺") + 1) * sizeof(char));
		strcpy(T.nodes[0].data, "break��䣺");
		T.nodes[0].indent = 1;
		T.nodes[0].firstchild = NULL;
		w = gettoken(fp);
		if (w != SEMI) return ERROR;
		w = gettoken(fp);
		return OK;
	}
	else if (w == RETURN) {
		T.n = 1; T.r = 0;
		T.nodes[0].data = (char*)malloc((strlen("return��䣺") + 1) * sizeof(char));
		strcpy(T.nodes[0].data, "return��䣺");
		T.nodes[0].indent = 1;
		T.nodes[0].firstchild = NULL;
		w = gettoken(fp);
		if (w == SEMI) return ERROR;
		if (!exp(fp, c, SEMI)) return ERROR;
		w = gettoken(fp);
		InsertChild(T, T.r, 1, c);
		return OK;
	}
	else if (w == LS) {
		if (!exp(fp, T, RS)) return ERROR;
		w = gettoken(fp);
		return OK;
	}
	else if (w == IDENT || w == INT_CONST || w == UNSIGNED_CONST || w == LONG_CONST || w == UNSIGNED_LONG_CONST || w == DOUBLE_CONST || w == FLOAT_CONST || w == LONG_DOUBLE_CONST || w == CHAR_CONST) {
		if (!exp(fp, T, SEMI)) return ERROR;
		w = gettoken(fp);
		return OK;
	}
	else if (w == RL) {
		return INFEASIBLE;
	}
	else if (w == SEMI) {
		T.n = 1; T.r = 0;
		T.nodes[0].data = (char*)malloc((strlen("�����") + 1) * sizeof(char));
		strcpy(T.nodes[0].data, "�����");
		T.nodes[0].indent = 1;
		T.nodes[0].firstchild = NULL;
		w = gettoken(fp);
		return OK;
	}
	else return ERROR;

}

status exp(FILE* fp, CTree& T, int endsym)//�﷨��λ<���ʽ>�ӳ���
{
	//�Ѿ������˵�һ��������w��
	SqStack op;		//�����ջ
	SqStack opn;	//������ջ
	CTree* node = (CTree*)malloc(sizeof(CTree));
	CTree* child1 = (CTree*)malloc(sizeof(CTree));
	CTree* child2 = (CTree*)malloc(sizeof(CTree));
	T.n = 1; T.r = 0;  //���ɱ��ʽ���
	T.nodes[0].data = (char*)malloc((strlen("���ʽ��䣺") + 1) * sizeof(char));
	strcpy(T.nodes[0].data, "���ʽ��䣺");
	T.nodes[0].indent = 1;
	T.nodes[0].firstchild = NULL;
	int error = 0;
	node->n = 1; node->r = 0; //������ֹ����
	node->nodes[0].data = (char*)malloc((strlen("#") + 1) * sizeof(char));
	strcpy(node->nodes[0].data, "#");
	node->nodes[0].firstchild = NULL;
	InitStack(opn);  InitStack(op);  Push(op, node);		//��ʼ��������ֹ��#��ջ
	while ((w != POUND || strcmp(node->nodes[0].data, "#")) && !error)  //�������ջջ��������ֹ���ţ���û�д���ʱ
	{
		if (w == IDENT || w == INT_CONST || w == UNSIGNED_CONST || w == LONG_CONST
			|| w == UNSIGNED_LONG_CONST || w == DOUBLE_CONST || w == FLOAT_CONST
			|| w == LONG_DOUBLE_CONST || w == CHAR_CONST)
		{
			node = (CTree*)malloc(sizeof(CTree));
			node->n = 1; node->r = 0;
			node->nodes[0].data = (char*)malloc((strlen(token_text) + strlen("ID: ") + 1) * sizeof(char));
			strcpy(node->nodes[0].data, "ID: ");
			strcat(node->nodes[0].data, token_text);
			node->nodes[0].indent = 1;
			node->nodes[0].firstchild = NULL;
			Push(opn, node);			//����w����һ����㣬���ָ���ջopn
			w = gettoken(fp);
		}
		else if (w == PLUS || w == MINUS || w == MULTIPLY || w == DIVIDE ||
			w == MOD || w == LS || w == RS || ((w>=MORE)&&(w<= LESS_EQUAL)) || w == EQUAL_TO ||
			w == AND || w == OR || w == POUND)
		{
			node = (CTree*)malloc(sizeof(CTree));
			GetTop(op, node);
			if (w == POUND) strcpy(token_text, "#");
			switch (precede(node->nodes[0].data, token_text))
			{
			case '<':
				node = (CTree*)malloc(sizeof(CTree));
				node->nodes[0].data = (char*)malloc((strlen(token_text) + 1) * sizeof(char));
				strcpy(node->nodes[0].data, token_text);
				node->nodes[0].indent = 1;
				node->nodes[0].firstchild = NULL;
				node->n = 1; node->r = 0;
				Push(op, node);		//����w����һ����㣬���ָ���ջopn
				w = gettoken(fp);
				break;
			case '=':
				if (!Pop(op, node)) error++;
				w = gettoken(fp);
				break;   //ȥ����
			case '>':
				if (!Pop(opn, child2)) error++;
				if (!Pop(opn, child1)) error++;
				if (!Pop(op, node)) error++;
				//���������ջ��ջ�õ��������node�Ͳ������Ľ��ָ��child1��child2��
				//��ɽ�������һ��������Ľ�㣬���ָ���ջopn
				InsertChild(*node, node->r, 1, *child1);
				InsertChild(*node, node->r, 2, *child2);
				Push(opn, node);
				break;
			default:
				if (w == endsym) w = POUND; //����������ǣ���w���滻��#
				else error++;
			}
		}
		else if (w == endsym) w = POUND;//����������Ƿֺţ�w���滻��#
		else error = 1;
		GetTop(op, node);
	}
	if (error) return ERROR;
	GetTop(opn, node);
	InsertChild(T, T.r, 1, *node);
	return OK;
}

char precede(char* a, char* b)
{
	int i, j;		//ָʾ�������Ӧ�ı��
	//����һ����ά���飬���ڴ�����ȼ�
	char precede[13][13] =
	{   //				 +		 -		 *		 /		 %		��		 ��	  	 =		>��<	  ==��!=		 #		&&		||
		/* + */			'>',	'>',	'<',	'<',	'<',	'<',	'>',	'?',	'>',		'>',		'>',	'>',	'>',
		/* - */			'>',	'>',	'<',	'<',	'<',	'<',	'>',	'?',	'>',		'>',		'>',	'>',	'>',
		/* * */			'>',	'>',	'>',	'>',	'>',	'<',	'>',	'?',	'>',		'>',		'>',	'>',	'>',
		/* / */			'>',	'>',	'>',	'>',	'>',	'<',	'>',	'?',	'>',		'>',		'>',	'>',	'>',
		/* % */			'>',	'>',	'<',	'<',	'<',	'<',	'>',	'?',	'>',		'>',		'>',	'>',	'>',
		/* ( */			'<',	'<',	'<',	'<',	'<',	'<',	'=',	'?',	'<',		'<',		'>',	'<',	'<',
		/* ) */			'>',	'>',	'>',	'>',	'>',	'>',	'?',	'?',	'>',		'>',		'>',	'>',	'>',
		/* = */			'<',	'<',	'<',	'<',	'<',	'<',	'?',	'<',	'<',		'<',		'>',	'<',	'<',
		/* >��< */		'<',	'<',	'<',	'<',	'<',	'<',	'>',	'?',	'>',		'>',		'>',	'>',	'>',
		/* ==��!= */	'<',	'<',	'<',	'<',	'<',	'<',	'>',	'?',	'<',		'>',		'>',	'>',	'>',
		/* # */			'<',	'<',	'<',	'<',	'<',	'<',	'?',	'<',	'<',		'<',		'=',	'<',	'<',
		/* && */		'<',	'<',	'<',	'<',	'<',	'<',	'>',	'>',	'<',		'<',		'>',	'>',	'>',
		/* || */		'<',	'<',	'<',	'<',	'<',	'<',	'>',	'>',	'<',		'<',		'>',	'>',	'>'
	};
	switch (a[0])
	{
	case '+':
		i = 0; break;
	case '-':
		i = 1; break;
	case '*':
		i = 2; break;
	case '/':
		i = 3; break;
	case '%':
		i = 4; break;
	case '(':
		i = 5; break;
	case ')':
		i = 6; break;
	case '=':
		if (a[1] == '=') i = 9;
		else i = 7;
		break;
	case '>':
	case '<':
		i = 8; break;
	case '!':
		if (a[1] == '=') i = 9;
		else return '?';
		break;
	case '#':
		i = 10;
		break;
	case '&':
		if (a[1] == '&') i = 11;
		else return '?';
		break;
	case '|':
		if (a[1] == '|') i = 12;
		else return '?';
		break;
	default:
		return '?';
	}
	switch (b[0])
	{
	case '+':
		j = 0; break;
	case '-':
		j = 1; break;
	case '*':
		j = 2; break;
	case '/':
		j = 3; break;
	case '%':
		j = 4; break;
	case '(':
		j = 5; break;
	case ')':
		j = 6; break;
	case '=':
		if (b[1] == '=') j = 9;
		else j = 7;
		break;
	case '>':
	case '<':
		j = 8; break;
	case '!':
		if (b[1] == '=') j = 9;
		else return '?';
		break;
	case '#':
		j = 10;
		break;
	case '&':
		if (b[1] == '&') j = 11;
		else return '?';
		break;
	case '|':
		if (b[1] == '|') j = 12;
		else return '?';
		break;
	default:
		return '?';
	}
	return precede[i][j];
}

status PrintTree(char* data, int indent) //��ӡ����
{
	int i;
	for (i = 0; i < indent; i++)
		printf("\t");
	printf("%s\n", data);
	return OK;
}