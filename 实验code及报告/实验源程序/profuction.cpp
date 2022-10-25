#include"profuction.h"

/*������غ���*/
status InitTree(CTree& T) //��ʼ����
{
	T.n = 0; T.r = -1;	//������λ����Ϊ-1��˵����ǰ��û�и�
	return OK;
}

status InsertChild(CTree& T, int p, int i, CTree c) {   //���뺢��,��p��㴦���뺢������c��i��ʾ��i������
	if (p < 0 || p >= T.n) return ERROR;
	if (!c.n) return ERROR;
	int k = 0, j = T.n;
	CNode prior = T.nodes[p].firstchild;
	CNode cur = prior;
	CNode t;
	while (k < c.n)  //�������
	{
		T.nodes[j] = c.nodes[k++];
		for (t = T.nodes[j].firstchild; t; t = t->next)
			t->child += T.n;
		j++;
	}
	t = (CNode)malloc(sizeof(CTNode));

	t->child = T.n + c.r;	//��������λ��
	t->next = NULL;
	T.n += c.n;
	if (i == 1)
	{
		if (prior)
		{
			t->next = prior;
			prior = t;
		}
		else prior = t;
		T.nodes[p].firstchild = prior;
	}
	else if (i < 1) return ERROR;					//���i����Ƿ�������ERROR
	else
	{
		k = 1;											//k��ʾ��ǰ�ǵڼ�������
		while (k < i && cur)
		{
			k++;
			prior = cur;
			cur = cur->next;
		}
		if (cur)
		{
			prior->next = t;
			t->next = cur;
		}
		else if (k == i)
		{
			prior->next = t;
			t->next = NULL;
		}
		else return ERROR;
	}
	return OK;
}

status GetParent(CTree T, int child, int& parent)
{
	CNode t; int i = 0;
	if (child >= T.n) return ERROR;
	for (; i < T.n; i++)
	{
		t = T.nodes[i].firstchild;
		while (t && t->child != child)
			t = t->next;
		if (t && t->child == child)
		{
			parent = i;
			return OK;
		}
	}
	return INFEASIBLE;
}

status TraverseTree(CTree T, status(*visit)(char*, int))
{
	stack<int> stack;
	int status = 0;							//������־�Ƿ����к��Ӷ������ʹ�
	int i; int j;
	int indent = 0;							//������
	int parent;
	CNode t;
	int visited[100] = { 0 };		//���ʱ�־�����ʼ��
	if (!T.n) return OK;					//��Ϊ��ֱ�ӽ���
	for (i = 0; i < T.n; i++)
	{
		indent = 0;
		if (!visited[i])					//����δ���ʵĽ����з���
		{
			stack.push(i);
			visited[i] = 1;
			if (GetParent(T, i, parent) == OK)
				indent += T.nodes[parent].indent;
			if (T.nodes[i].indent)
				visit(T.nodes[i].data, indent);
			t = T.nodes[i].firstchild;
			while (t && !status)									//����ý���к���
			{
				while (visited[t->child] && t->next)	//Ѱ��δ�����ʹ��ĺ���
					t = t->next;
				if (!visited[t->child])
				{
					visited[t->child] = 1;
					if (T.nodes[t->child].firstchild)
						stack.push(t->child);
					GetParent(T, t->child, parent);
					indent += T.nodes[parent].indent;
					if (T.nodes[t->child].indent)
						visit(T.nodes[t->child].data, indent);
					t = T.nodes[t->child].firstchild;
					status = 0;
				}
				else status = 1;
			}
		}
		while (!stack.empty())						//ѭ����ջΪ��
		{
			j = stack.top();
			indent -= T.nodes[j].indent;
			t = T.nodes[j].firstchild;
			status = 0;
			while (t && !status)							//����ý���к���
			{
				while (visited[t->child] && t->next)	//Ѱ��δ�����ʹ��ĺ���
					t = t->next;
				if (!visited[t->child])
				{
					visited[t->child] = 1;
					if (T.nodes[t->child].firstchild)
						stack.push(t->child);
					GetParent(T, t->child, parent);
					indent += T.nodes[parent].indent;
					if (T.nodes[t->child].indent)
						visit(T.nodes[t->child].data, indent);
					t = T.nodes[t->child].firstchild;
					status = 0;
				}
				else status = 1;
			}
			if (t) stack.pop();
		}
	}
	return OK;
}

/*ջ����غ���*/
status InitStack(SqStack& S)
{
	//����һ����ջS
	S.base = (SElemType*)malloc(100 * sizeof(SElemType));
	if (!S.base) exit(OVERFLOW);	//�洢����ʧ��
	S.top = S.base;
	S.stacksize = 100;
	return OK;
}

status GetTop(SqStack S, SElemType& e)
{
	//��ջ���գ�����e����S��ջ��Ԫ�أ�������OK�����򷵻�ERROR
	if (S.top == S.base) return ERROR;
	e = *(S.top - 1);
	return OK;
}

status Push(SqStack& S, SElemType e)
{//����Ԫ��eΪ�µ�ջ��Ԫ��
	if (S.top - S.base >= S.stacksize)
	{//ջ����׷�Ӵ洢�ռ�
		S.base = (SElemType*)realloc(S.base,(S.stacksize + 100) * sizeof(SElemType));
		if (!S.base) exit(OVERFLOW);	//�洢����ʧ��
		S.top = S.base + S.stacksize;
		S.stacksize += 100;
	}
	*S.top++ = e;
	return OK;
}

status Pop(SqStack& S, SElemType& e)
{//��ջ���գ���ɾ��S��ջ��Ԫ�أ���e������ֵ��������OK
	if (S.top == S.base) return ERROR;
	e = *(--S.top);
	return OK;
}