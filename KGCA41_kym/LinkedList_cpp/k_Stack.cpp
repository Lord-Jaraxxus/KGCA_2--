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
		printf("���� ����÷ο�.\n");
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
		printf("���ÿ� �� �� �����ϴ�.\n");
		return nullptr;
	}
	else {
		printf("%d�� ���� ���� �ֽ��ϴ�.\n", temp->m_iAge);
		return temp;
	}
}

void k_Stack::pop_all()
{
	while (k_LinkedList.begin()->m_pPrev != k_LinkedList.end()) // �Ӹ��� ������ ����������
	{
		pop();
	}
}