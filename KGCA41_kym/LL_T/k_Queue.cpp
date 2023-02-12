#include "k_Queue.h"

LinkedList k_LinkedList_que;

k_Queue::k_Queue()
{

}

k_Queue::~k_Queue()
{

}

void k_Queue::in(int a)
{
	k_Student* new_student = new k_Student;
	new_student->m_iAge = a;
	k_LinkedList_que.insertBack(new_student);
	printf("%d in!\n", new_student->m_iAge);
}

int k_Queue::out()
{
	k_Student* sacrificed = k_LinkedList_que.begin();
	if (sacrificed->m_pPrev == k_LinkedList_que.end())
	{
		printf("ť ����÷ο�.\n");
		return 0;
	}
	else
	{
		int temp = sacrificed->m_iAge;
		printf("%d out!\n", temp);
		k_LinkedList_que.death(sacrificed); //�긦 ���ϰ� �ݺ��ϸ� ��°���
		return temp;
	}
}

k_Student* k_Queue::front() const
{
	k_Student* temp = k_LinkedList_que.begin();
	if (temp->m_pPrev == k_LinkedList_que.end())
	{
		printf("ť�� �� �� �����ϴ�.\n");
		return nullptr;
	}
	else {
		printf("%d�� ���� �տ� �ֽ��ϴ�.\n", temp->m_iAge);
		return temp;
	}
}

k_Student* k_Queue::back() const
{
	k_Student* temp = k_LinkedList_que.end();
	if (k_LinkedList_que.begin()->m_pPrev == temp)
	{
		printf("ť�� �� �� �����ϴ�.\n");
		return nullptr;
	}
	else {
		printf("%d�� ���� �ڿ� �ֽ��ϴ�.\n", temp->m_iAge);
		return temp;
	}
}

void k_Queue::out_all()
{
	while (k_LinkedList_que.begin()->m_pPrev != k_LinkedList_que.end()) // �Ӹ��� ������ ����������
	{
		out();
	}
}