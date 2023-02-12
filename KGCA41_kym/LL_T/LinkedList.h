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
	~Box() {}; // {}안해줘서 큰일날뻔핸내

public:
	T m_Data;
	Box* m_pNext;
	Box* m_pPrev;

	int HT; // 디버깅할때 머리꼬리 구분용, 머리:1 꼬리:2 else:0
};

template <class T>
class LinkedList
{
public:
	LinkedList(); //생성자
	~LinkedList(); //소멸자 , 사실 얘 둘은 이러케 안써놔도 자동으로 생김

//protected:
public: //일단 퍼블릭해놓고 돌려보자
	Box<T>* m_pHead = nullptr;
	Box<T>* m_pTail = nullptr;

public:
	static int m_iCount; // 클래스 내부에 있는 static int는 외부에서 초기화해줘야함

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
int LinkedList<T>::m_iCount = 0;  // 클래스 안의 스태틱 변수는 반드시 클래스 외부에서 초기화

template<class T>
LinkedList<T>::LinkedList() // 생성자, 머리와 꼬리 초기화
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
LinkedList<T>::~LinkedList() // 소멸자, 머리와 꼬리 메모리 할당 해제
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
void LinkedList<T>::H_T_link() // Student_manager에서 ReleaseData로 싹다 날린 후에 머리랑 꼬리 다시 연결, 방법있나?
{
	m_pHead->m_pNext = m_pTail;
	m_pTail->m_pPrev = m_pHead;
}

template<class T>
Box<T>* LinkedList<T>::begin() const // head 바로 다음 노드를 리턴
{
	Box<T>* temp = m_pHead->m_pNext;
	return temp;
}

template<class T>
Box<T>* LinkedList<T>::end() const // Tail 바로 앞 노드를 리턴
{
	Box<T>* temp = m_pTail->m_pPrev;
	return temp;
}

template<class T>
void LinkedList<T>::insertFront(T new_data)
{
	Box<T>* pNewbox = boxing(new_data);
	Box<T>* pTemp = m_pHead->m_pNext; // head 바로 뒤의 노드를 저장

	pNewbox->m_pNext = pTemp;
	pNewbox->m_pPrev = m_pHead;

	m_pHead->m_pNext = pNewbox;
	pTemp->m_pPrev = pNewbox;

	//new_student->m_pNext = m_pHead->m_pNext;
	//new_student->m_pPrev = m_pHead;
	//m_pHead->m_pNext->m_pPrev = new_student;
	//m_pHead->m_pNext = new_student;                  //- 아마 이러케해도 돌아는 갈텐데 너무 복잡해서 Temp를 쓰는건가?
}

template<class T>
void LinkedList<T>::insertTargetFront(T new_data, Box         <T>* target_node)
{
	_ASSERT(target_node != m_pHead); // head앞에 뭔가를 낑길수는 없으니까 예외처리

	Box<T>* pNewbox = boxing(new_data);
	Box<T>* pTemp = target_node->m_pPrev; // 타겟노드 바로 앞의 노드를 저장
	pNewbox->m_pNext = target_node;
	pNewbox->m_pPrev = pTemp;

	target_node->m_pPrev = pNewbox;
	pTemp->m_pNext = pNewbox;
}

template<class T>
void LinkedList<T>::insertBack(T new_data)
{
	Box<T>* pNewbox = boxing(new_data);
	Box<T>* pTemp = m_pTail->m_pPrev; // Tail 바로 앞의 노드를 저장

	pNewbox->m_pNext = m_pTail;
	pNewbox->m_pPrev = pTemp;

	pTemp->m_pNext = pNewbox;
	m_pTail->m_pPrev = pNewbox;

	//new_student->m_pNext = m_pTail;
	//new_student->m_pPrev = m_pTail->m_pPrev;
	//m_pTail->m_pPrev->m_pNext = new_student;
	//m_pTail->m_pPrev = new_student; // 이래도 될것같은디2
}

template<class T>
void LinkedList<T>::insertTargetBack(T new_data, Box<T>* target_node)
{
	_ASSERT(target_node != m_pTail); // Tail 뒤에 뭔가를 낑길수는 없으니까 예외처리

	Box<T>* pNewbox = boxing(new_data);
	Box<T>* pTemp = target_node->m_pNext; // 타겟노드 바로 뒤의 노드를 저장
	pNewbox->m_pNext = pTemp;
	pNewbox->m_pPrev = target_node;

	target_node->m_pNext = pNewbox;
	pTemp->m_pPrev = pNewbox;
}

template<class T>
void LinkedList<T>::raft(Box<T>* target_node) // 정렬에 쓰기 위해서 연결만 끊어내고 free는 하지 않음 
{
	target_node->m_pPrev->m_pNext = target_node->m_pNext;
	target_node->m_pNext->m_pPrev = target_node->m_pPrev;    // 이래도 잘 돌아갈성싶은디
}

template<class T>
void LinkedList<T>::death(Box<T>* target_node)
{
	target_node->m_pPrev->m_pNext = target_node->m_pNext;
	target_node->m_pNext->m_pPrev = target_node->m_pPrev;    // 이래도 잘 돌아갈성싶은디
	delete(target_node);
}

template<class T>
Box<T>* LinkedList<T>::operator[](int index)
{
	// if걸어서 현재 링리보다 큰 수 들어오면 걸러내게 해야하는데 대충 나중에
	Box<T>* current = begin();
	for (int i = 0; i < index; i++)
	{
		current = current->m_pNext;
	}
	return current;
}


