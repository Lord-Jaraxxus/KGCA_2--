#include "Linked_List.h"

void in(int a);
int out();
void destroy();
void print_queue();

void main()
{
	Init();
	in(2);
	in(3);
	in(4);
	in(5);
	in(6);
	in(7);
	print_queue();

	out();
	out();
	out();
	print_queue();
	out();
	out();
	out();
	out();

	destroy();
}


void in(int a)
{
	k_Student* temp = NewStudent();
	temp->m_iAge = a;
	insertBack(temp);
	printf("%d in!\n", temp->m_iAge);
}

int out()
{
	k_Student* temp = g_pHead->m_pNext;

	if (temp == g_pTail)
	{
		printf("큐 언더플로우\n");
		return 0;
	}

	else
	{
		int popcorn = temp->m_iAge;
		printf("%d out!\n", temp->m_iAge);
		death(temp);
		return popcorn;
	}
}

void destroy()
{
	ReleaseAll();
}

void print_queue()
{
	k_Student* temp = g_pHead->m_pNext;
	while (temp != g_pTail)
	{
		printf("%d ", temp->m_iAge);
		temp = temp->m_pNext;
	}
	printf("\n");
}