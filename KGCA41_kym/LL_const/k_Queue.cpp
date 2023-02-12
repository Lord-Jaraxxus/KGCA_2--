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
		printf("큐 언더플로우.\n");
		return 0;
	}
	else
	{
		int temp = sacrificed->m_iAge;
		printf("%d out!\n", temp);
		k_LinkedList_que.death(sacrificed); //얘를 안하고 반복하면 출력가능
		return temp;
	}
}

k_Student* k_Queue::front() const
{
	k_Student* temp = k_LinkedList_que.begin();
	if (temp->m_pPrev == k_LinkedList_que.end())
	{
		printf("큐에 든 게 없습니다.\n");
		return nullptr;
	}
	else {
		printf("%d가 제일 앞에 있습니다.\n", temp->m_iAge);
		return temp;
	}
}

k_Student* k_Queue::back() const
{
	k_Student* temp = k_LinkedList_que.end();
	if (k_LinkedList_que.begin()->m_pPrev == temp)
	{
		printf("큐에 든 게 없습니다.\n");
		return nullptr;
	}
	else {
		printf("%d가 제일 뒤에 있습니다.\n", temp->m_iAge);
		return temp;
	}
}

void k_Queue::out_all()
{
	while (k_LinkedList_que.begin()->m_pPrev != k_LinkedList_que.end()) // 머리와 꼬리가 남을때까지
	{
		out();
	}
}