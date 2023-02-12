#include "Linked_List.h"

void push(int a);
int pop();
void destroy();
void print_stack();

void main() 
{
	Init();
	push(2);
	push(3);
	push(4);
	push(5);
	push(6);
	print_stack();

	pop();
	pop();
	print_stack();
	pop();
	pop();
	pop();
	pop();

	destroy();
}


void push(int a) 
{
	k_Student* temp = NewStudent();
	temp->m_iAge = a;
	insertFront(temp);
	printf("%d push!\n", temp->m_iAge);
}

int pop() 
{
	k_Student* temp = g_pHead->m_pNext;

	if (temp == g_pTail) 
	{ 
		printf("스택 언더플로우\n"); 
		return 0; 
	}

	else 
	{
		int popcorn = temp->m_iAge;
		printf("%d pop!\n", temp->m_iAge);
		death(temp);
		return popcorn;
	}
}

void destroy() 
{
	ReleaseAll();
}

void print_stack() 
{
	k_Student* temp = g_pHead->m_pNext;
	while (temp != g_pTail) 
	{ 
		printf("%d ", temp->m_iAge); 
		temp = temp->m_pNext;
	}
	printf("\n");
}