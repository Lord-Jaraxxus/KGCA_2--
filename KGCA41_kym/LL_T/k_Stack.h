#include "LinkedList.h"

template <class T>
class k_Stack : public LinkedList<T>
{
public:
	void push(T data);
	int pop();
	Box<T>* top();
	void pop_all();

public:
	k_Stack();
	~k_Stack();
};





LinkedList<int> k_LinkedList;

template <class T>
k_Stack<T>::k_Stack()
{

}

template <class T>
k_Stack<T>::~k_Stack()
{

}

template <class T>
void k_Stack<T>::push(T data)
{
	k_LinkedList.insertFront(data);
	printf("%d push!\n", data); // �̷��� ������?
}

template <class T>
int k_Stack<T>::pop()
{
	if (k_LinkedList.begin()->m_pPrev == k_LinkedList.end()) { printf("���ÿ� �� �� �����ϴ�! - pop\n"); return 0; }
	Box<T>* sacrificed = k_LinkedList.begin();
	int temp = sacrificed->m_Data;
	printf("%d pop!\n", temp);
	k_LinkedList.death(sacrificed);
	return temp;
}

template <class T>
Box<T>* k_Stack<T>::top()
{
	if (k_LinkedList.begin()->m_pPrev == k_LinkedList.end()) { printf("���ÿ� �� �� �����ϴ�! - top\n"); return nullptr; }
	printf("%d is t.o.p.\n", k_LinkedList.begin()->m_Data);
	return k_LinkedList.begin();
	
}

template <class T>
void k_Stack<T>::pop_all()
{
	while (k_LinkedList.begin()->m_pPrev != k_LinkedList.end()) // �Ӹ��� ������ ���� ��Ȳ����
	{
		pop();
	}
}



