/*
 ????????е????????????????????????ɡ?
 ??д?????????????????д???????д?????????????�????????
???????????????#...#???
 */
#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
# define INFEASIBLE -1
# define OVERFLOW -2
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10
typedef int ElemType;
typedef int Status;
typedef struct Node
{
	ElemType data;
	struct Node *next;
}StackNode;
typedef struct Node *Stack; 
Status InitStack(Stack &top)
{
	top = (Stack)malloc(sizeof(StackNode));
	if (top == NULL)
		exit(OVERFLOW);
	else
	{
		top->next = NULL;
		return OK;
	}
}
Status DestroyStack(Stack &top)
{
	Stack p;
	if (!top)	
		return INFEASIBLE;
	else
	{
		while(top)
		{
			p = top;
			top = top->next;
			free(p);
		}
		return OK;
	}
}
Status ClearStack(Stack &top)
{
	Stack p = top->next, q;
	if (!top)
		return INFEASIBLE;
	else
	{
		while(p)
		{
			q = p->next;
			free(p);
			p = q;
		}
		top->next = NULL;
	}
}
Status StackEmpty(Stack top)
{
	if (!top)
		return INFEASIBLE;
	else if (top->next == NULL)
		return TRUE;
	else
		return FALSE;
}
Status StackLength(Stack top)
{
	Stack p;
	int i = 0; 
	if (!top)
		return INFEASIBLE;
	else 
	{
		p = top->next;
		while(p)
		{
			i++;
			p = p->next;
		}
	}
	return i;
}
Status GetTop(Stack top, ElemType &e)
{
	if (!top)
		return INFEASIBLE;
	else 
	{
		if(top->next != NULL)
		{
			e = top->next->data;
			return OK;
		}
		else
			return ERROR;
	}
}
Status Push(Stack &top, ElemType e)
{
	if (!top)
		return INFEASIBLE;
	else
	{
		Stack p = (Stack)malloc(sizeof(StackNode));
		p->data = e;
		p->next = top->next;
		top->next = p;
		return OK;
	}
}
Status Pop(Stack &top, ElemType &e)
{
	Stack p;
	if (!top||!top->next)
		return INFEASIBLE;
	else
	{
		p = top->next;
		top->next = p->next;
		e = p->data;
		free(p);
		return OK;
	}
}
void reverse(Stack &L)
{
	Stack p,q;
	p = L->next;L->next = NULL;
	while(p)
	{
		q = p->next;
		p->next = L->next;
		L->next = p;
		p = q;
	}
}
Status StackTraverse(Stack top)
{
	reverse(top);			//?????????? 
	Stack p = top->next; 
	while(p)
	{
		printf("%c ",p->data);
		p = p->next;
	}
}
Status IsOperator(char c)
{
	if(c == '#'||c == '('||c == '+'||c == '-'||c == '*'||c == '/'||c == ')')
		return TRUE;
	else
		return FALSE; 
}
Status Prior(char c1,char c2)
{
	char ch[] = "#(+-*/)";
	int i = 0,j = 0;
	while(ch[i] && ch[i] != c1) i++;
	if(i == 3) i--;	// ??????????????????????
	if(i == 5) i--;	// ??????????????????????
	while(ch[j] && ch[j] != c2) j++;
	if(j == 3) j--;
	if(j == 5) j--;
	if(i >= j) 
		return TRUE;
	else 
		return FALSE;
}
void InversePolandExpression(char Buffer[])
{
	Stack s1,s2;
	InitStack(s1);
	InitStack(s2);
	int i=0;
	ElemType e;
	while(Buffer[i] != '\0')
	{
		if(!IsOperator(Buffer[i]))// ???????
		{	
			Push(s1,Buffer[i]);
		}
		else// ???????
		{	
			if(Buffer[i] == '(' || StackEmpty(s2))
			{
				Push(s2,Buffer[i]);				
			}
			else
			{
				if(Buffer[i] == ')')
				{
					Pop(s2,e);
					while(e != '(')
					{
						Push(s1,e);
						Pop(s2,e);
					}
				}
				else
				{
					GetTop(s2,e);
					while(Prior(e,Buffer[i]))
					{
						Pop(s2,e);
						Push(s1,e);
						GetTop(s2,e);
						if(StackEmpty(s2))
							break;
					}
					Push(s2,Buffer[i]);
				}
			}
		}
		i++;
	}
	while(!StackEmpty(s2))
	{
		Pop(s2,e);
		Push(s1,e);
	}
	StackTraverse(s1);
}
void Operate(Stack &s1,ElemType e)
{
	int e1,e2;
	Pop(s1,e1);
	Pop(s1,e2);
	int x;
	switch(e)
	{
		case '+':Push(s1,e1+e2);break;
		case '-':Push(s1,e2-e1);break;
		case '*':Push(s1,e1*e2);break;
		case '/':Push(s1,e2/e1);break;
	}
}
void EvaluateExpression(char buff[])
{
	Stack s1,s2;	//s1������ջ��s2�����ջ
	InitStack(s1);
	InitStack(s2);
	int value = 0;
	int flag = 0;
	int i = 0;
	ElemType e;
	while(buff[i] != '\0')
	{
		if (!IsOperator(buff[i]))
		{	
			value = value * 10 + buff[i] - '0';
			flag = 1;
		}
		else
		{
			if(flag == 1)
			{
				Push(s1,value);
				flag = 0;
				value = 0;
			}
			if (buff[i] == '(' || StackEmpty(s2))
				Push(s2,buff[i]);
			else
			{
				if (buff[i] == ')')
				{
					Pop(s2,e);
					while(e != '(')
					{
						Operate(s1,e);
						Pop(s2,e);
					}
				}
				else
				{
					GetTop(s2,e);
				//	if (!Prior(e,Buffer[i]))		
					while(Prior(e,buff[i]))
					{
						Pop(s2,e);
						Operate(s1,e);
						GetTop(s2,e);
						if(StackEmpty(s2))
							break;
					}
					Push(s2,buff[i]);
				}
			}
		}
		i++;
	}
	if (flag)
        Push(s1,value);
    while (!StackEmpty(s2))
    {
        Pop(s2,e);
        Operate(s1,e);
        GetTop(s1,value); 
    }
    GetTop(s1,value); 
	printf("%s=%d\n",buff,value);
}
int main()
{
//	char buff[] = "#9+(3-1)*3+10/2#";
	char buff[255];
	gets(buff);
	InversePolandExpression(buff);
	printf("\n"); 
	EvaluateExpression(buff);
}
