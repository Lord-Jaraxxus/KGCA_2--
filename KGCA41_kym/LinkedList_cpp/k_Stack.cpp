#include "k_Stack.h"

LinkedList k_LinkedList;

k_Stack::k_Stack() 
{

}

k_Stack::~k_Stack()
{

}

void k_Stack::push(int a)
{
	k_Student* new_student = new k_Student;
	new_student->m_iAge = a;
	k_LinkedList.insertFront(new_student);
	printf("%d push!\n", new_student->m_iAge);
}

int k_Stack::pop()
{
	k_Student* sacrificed =  k_LinkedList.begin();
	if (sacrificed->m_pPrev == k_LinkedList.end()) 
	{
		printf("스택 언더플로우.\n");
		return 0;
	}
	else 
	{
		int temp = sacrificed->m_iAge;
		printf("%d pop!\n", temp);
		k_LinkedList.death(sacrificed);
		return temp;
	}
}

k_Student* k_Stack::top()
{
	k_Student* temp = k_LinkedList.begin();
	if (temp->m_pPrev == k_LinkedList.end()) 
	{
		printf("스택에 든 게 없습니다.\n");
		return nullptr;
	}
	else {
		printf("%d가 제일 위에 있습니다.\n", temp->m_iAge);
		return temp;
	}
}

void k_Stack::pop_all()
{
	while (k_LinkedList.begin()->m_pPrev != k_LinkedList.end()) // 머리와 꼬리가 남을때까지
	{
		pop();
	}
}