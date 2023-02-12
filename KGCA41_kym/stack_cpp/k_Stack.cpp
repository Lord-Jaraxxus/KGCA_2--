#include "k_Stack.h"

LinkedList k_LinkedList;

k_Stack::k_Stack() 
{

}

k_Stack::~k_Stack()
{

}

void push(int a) 
{
	k_Student* new_student = new k_Student;
	new_student->m_iAge = a;
	k_LinkedList.insertFront(new_student);
	printf("%d push!\n", new_student->m_iAge);
}

int pop() 
{
	k_Student* sacrificed =  k_LinkedList.begin();
	int temp = sacrificed->m_iAge;
	printf("%d pop!\n", temp);
	k_LinkedList.death(sacrificed);
	return temp;
}

k_Student* top() 
{
	return k_LinkedList.begin();
}

void pop_all() 
{
	while (k_LinkedList.begin()->m_pPrev == k_LinkedList.end()) // 머리와 꼬리가 남은 상황까지
	{
		pop();
	}
}