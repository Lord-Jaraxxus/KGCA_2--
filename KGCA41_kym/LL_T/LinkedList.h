#include <crtdbg.h>

template <class T>
class Box 
{
public:
	Box() 
	{
		m_pNext = m_pPrev = nullptr; 
		HT = 0;
	}
	~Box() {}; // {}�����༭ ū�ϳ����ڳ�

public:
	T m_Data;
	Box* m_pNext;
	Box* m_pPrev;

	int HT; // ������Ҷ� �Ӹ����� ���п�, �Ӹ�:1 ����:2 else:0
};

template <class T>
class LinkedList
{
public:
	LinkedList(); //������
	~LinkedList(); //�Ҹ��� , ��� �� ���� �̷��� �Ƚ���� �ڵ����� ����

//protected:
public: //�ϴ� �ۺ��س��� ��������
	Box<T>* m_pHead = nullptr;
	Box<T>* m_pTail = nullptr;

public:
	static int m_iCount; // Ŭ���� ���ο� �ִ� static int�� �ܺο��� �ʱ�ȭ�������

public:
	Box<T>* boxing(T data);
	Box<T>* begin() const;
	Box<T>* end() const;

public:
	void insertFront(T new_data);
	void insertTargetFront(T new_data, Box<T>* target_node);
	void insertBack(T new_data);
	void insertTargetBack(T new_data, Box<T>* target_node);

	void death(Box<T>* target_node);
	void raft(Box<T>* target_node);
	void H_T_link();

public:
	Box<T>* operator[](int index);
};









template<class T>
int LinkedList<T>::m_iCount = 0;  // Ŭ���� ���� ����ƽ ������ �ݵ�� Ŭ���� �ܺο��� �ʱ�ȭ

template<class T>
LinkedList<T>::LinkedList() // ������, �Ӹ��� ���� �ʱ�ȭ
{
	m_pHead = new Box<T>;
	m_pTail = new Box<T>;

	m_pHead->m_pPrev = nullptr;
	m_pHead->m_pNext = m_pTail;
	m_pTail->m_pPrev = m_pHead;
	m_pTail->m_pNext = nullptr;

	m_pHead->HT = 1;
	m_pTail->HT = 2;
}

template<class T>
LinkedList<T>::~LinkedList() // �Ҹ���, �Ӹ��� ���� �޸� �Ҵ� ����
{
	delete m_pHead;
	delete m_pTail;
}

template<class T>
Box<T>* LinkedList<T>::boxing(T data)
{
	//_ASSERT(data);
	Box<T>* new_box = new Box<T>;
	new_box->m_Data = data;
	return new_box;
}

template<class T>
void LinkedList<T>::H_T_link() // Student_manager���� ReleaseData�� �ϴ� ���� �Ŀ� �Ӹ��� ���� �ٽ� ����, ����ֳ�?
{
	m_pHead->m_pNext = m_pTail;
	m_pTail->m_pPrev = m_pHead;
}

template<class T>
Box<T>* LinkedList<T>::begin() const // head �ٷ� ���� ��带 ����
{
	Box<T>* temp = m_pHead->m_pNext;
	return temp;
}

template<class T>
Box<T>* LinkedList<T>::end() const // Tail �ٷ� �� ��带 ����
{
	Box<T>* temp = m_pTail->m_pPrev;
	return temp;
}

template<class T>
void LinkedList<T>::insertFront(T new_data)
{
	Box<T>* pNewbox = boxing(new_data);
	Box<T>* pTemp = m_pHead->m_pNext; // head �ٷ� ���� ��带 ����

	pNewbox->m_pNext = pTemp;
	pNewbox->m_pPrev = m_pHead;

	m_pHead->m_pNext = pNewbox;
	pTemp->m_pPrev = pNewbox;

	//new_student->m_pNext = m_pHead->m_pNext;
	//new_student->m_pPrev = m_pHead;
	//m_pHead->m_pNext->m_pPrev = new_student;
	//m_pHead->m_pNext = new_student;                  //- �Ƹ� �̷����ص� ���ƴ� ���ٵ� �ʹ� �����ؼ� Temp�� ���°ǰ�?
}

template<class T>
void LinkedList<T>::insertTargetFront(T new_data, Box         <T>* target_node)
{
	_ASSERT(target_node != m_pHead); // head�տ� ������ ������� �����ϱ� ����ó��

	Box<T>* pNewbox = boxing(new_data);
	Box<T>* pTemp = target_node->m_pPrev; // Ÿ�ٳ�� �ٷ� ���� ��带 ����
	pNewbox->m_pNext = target_node;
	pNewbox->m_pPrev = pTemp;

	target_node->m_pPrev = pNewbox;
	pTemp->m_pNext = pNewbox;
}

template<class T>
void LinkedList<T>::insertBack(T new_data)
{
	Box<T>* pNewbox = boxing(new_data);
	Box<T>* pTemp = m_pTail->m_pPrev; // Tail �ٷ� ���� ��带 ����

	pNewbox->m_pNext = m_pTail;
	pNewbox->m_pPrev = pTemp;

	pTemp->m_pNext = pNewbox;
	m_pTail->m_pPrev = pNewbox;

	//new_student->m_pNext = m_pTail;
	//new_student->m_pPrev = m_pTail->m_pPrev;
	//m_pTail->m_pPrev->m_pNext = new_student;
	//m_pTail->m_pPrev = new_student; // �̷��� �ɰͰ�����2
}

template<class T>
void LinkedList<T>::insertTargetBack(T new_data, Box<T>* target_node)
{
	_ASSERT(target_node != m_pTail); // Tail �ڿ� ������ ������� �����ϱ� ����ó��

	Box<T>* pNewbox = boxing(new_data);
	Box<T>* pTemp = target_node->m_pNext; // Ÿ�ٳ�� �ٷ� ���� ��带 ����
	pNewbox->m_pNext = pTemp;
	pNewbox->m_pPrev = target_node;

	target_node->m_pNext = pNewbox;
	pTemp->m_pPrev = pNewbox;
}

template<class T>
void LinkedList<T>::raft(Box<T>* target_node) // ���Ŀ� ���� ���ؼ� ���Ḹ ����� free�� ���� ���� 
{
	target_node->m_pPrev->m_pNext = target_node->m_pNext;
	target_node->m_pNext->m_pPrev = target_node->m_pPrev;    // �̷��� �� ���ư���������
}

template<class T>
void LinkedList<T>::death(Box<T>* target_node)
{
	target_node->m_pPrev->m_pNext = target_node->m_pNext;
	target_node->m_pNext->m_pPrev = target_node->m_pPrev;    // �̷��� �� ���ư���������
	delete(target_node);
}

template<class T>
Box<T>* LinkedList<T>::operator[](int index)
{
	// if�ɾ ���� �������� ū �� ������ �ɷ����� �ؾ��ϴµ� ���� ���߿�
	Box<T>* current = begin();
	for (int i = 0; i < index; i++)
	{
		current = current->m_pNext;
	}
	return current;
}


