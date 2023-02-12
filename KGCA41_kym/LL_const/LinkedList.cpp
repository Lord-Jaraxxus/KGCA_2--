#include "LinkedList.h"

int LinkedList::m_iCount = 0;  // 클래스 안의 스태틱 변수는 반드시 클래스 외부에서 초기화
k_Student* LinkedList::m_pCurr = nullptr;

LinkedList::LinkedList() // 생성자, 머리와 꼬리 초기화
{
	g_pHead = new k_Student;
	g_pTail = new k_Student;

	g_pHead->m_pPrev = nullptr;
	g_pHead->m_pNext = g_pTail;
	g_pTail->m_pPrev = g_pHead;
	g_pTail->m_pNext = nullptr;

	g_pHead->m_sName[0] = 'H';
	g_pHead->m_sName[1] = 'D';
	g_pHead->m_sName[2] = ' ';
	g_pHead->m_sName[3] = '\0';
	g_pTail->m_sName[0] = 'T';
	g_pTail->m_sName[1] = 'L';
	g_pTail->m_sName[2] = ' ';
	g_pTail->m_sName[3] = '\0';  // 디버깅할때 좀 편하려고
}

LinkedList::~LinkedList() // 소멸자, 머리와 꼬리 메모리 할당 해제
{
	delete g_pHead;
	delete g_pTail;
}

void LinkedList::H_T_link() // Student_manager에서 ReleaseData로 싹다 날린 후에 머리랑 꼬리 다시 연결, 방법있나?
{
	g_pHead->m_pNext = g_pTail;
	g_pTail->m_pPrev = g_pHead;
}

k_Student* LinkedList::begin() const // head 바로 다음 노드를 리턴
{
	k_Student* temp = g_pHead->m_pNext;
	return temp;
}

k_Student* LinkedList::end() const // Tail 바로 앞 노드를 리턴
{
	k_Student* temp = g_pTail->m_pPrev;
	return temp;
}

void LinkedList::insertFront(k_Student* new_student)
{
	k_Student* pTemp = g_pHead->m_pNext; // head 바로 뒤의 노드를 저장
	new_student->m_pNext = pTemp;
	new_student->m_pPrev = g_pHead;

	g_pHead->m_pNext = new_student;
	pTemp->m_pPrev = new_student;

	//new_student->m_pNext = g_pHead->m_pNext;
	//new_student->m_pPrev = g_pHead;
	//g_pHead->m_pNext->m_pPrev = new_student;
	//g_pHead->m_pNext = new_student;                  //- 아마 이러케해도 돌아는 갈텐데 너무 복잡해서 Temp를 쓰는건가?
}

void LinkedList::insertTargetFront(k_Student* new_student, k_Student* target_node)
{
	_ASSERT(target_node != g_pHead); // head앞에 뭔가를 낑길수는 없으니까 예외처리

	if (target_node == NULL) { insertFront(new_student); } // 타겟 노드가 NULL로 들어올때

	k_Student* pTemp = target_node->m_pPrev; // 타겟노드 바로 앞의 노드를 저장
	new_student->m_pNext = target_node;
	new_student->m_pPrev = pTemp;

	target_node->m_pPrev = new_student;
	pTemp->m_pNext = new_student;
}

void LinkedList::insertBack(k_Student* new_student)
{
	k_Student* pTemp = g_pTail->m_pPrev; // Tail 바로 앞의 노드를 저장
	new_student->m_pNext = g_pTail;
	new_student->m_pPrev = pTemp;

	pTemp->m_pNext = new_student;
	g_pTail->m_pPrev = new_student;

	//new_student->m_pNext = g_pTail;
	//new_student->m_pPrev = g_pTail->m_pPrev;
	//g_pTail->m_pPrev->m_pNext = new_student;
	//g_pTail->m_pPrev = new_student; // 이래도 될것같은디2
}

void LinkedList::insertTargetBack(k_Student* new_student, k_Student* target_node)
{
	_ASSERT(target_node != g_pTail); // Tail 뒤에 뭔가를 낑길수는 없으니까 예외처리

	k_Student* pTemp = target_node->m_pNext; // 타겟노드 바로 뒤의 노드를 저장
	new_student->m_pNext = pTemp;
	new_student->m_pPrev = target_node;

	target_node->m_pNext = new_student;
	pTemp->m_pPrev = new_student;
}

void LinkedList::raft(k_Student* target_node) // 정렬에 쓰기 위해서 연결만 끊어내고 free는 하지 않음 
{
	target_node->m_pPrev->m_pNext = target_node->m_pNext;
	target_node->m_pNext->m_pPrev = target_node->m_pPrev;    // 이래도 잘 돌아갈성싶은디
}

void LinkedList::death(k_Student* target_node)
{
	target_node->m_pPrev->m_pNext = target_node->m_pNext;
	target_node->m_pNext->m_pPrev = target_node->m_pPrev;    // 이래도 잘 돌아갈성싶은디
	free(target_node);
}

k_Student* LinkedList::find_young_man(k_Student* temp) const
{
	if (temp->m_pNext == g_pTail) { return temp; }

	k_Student* pTemp = temp->m_pNext;
	k_Student* pYoung = nullptr;
	int age_temp = 101;
	while (pTemp != g_pTail)
	{
		if (pTemp->m_iAge < age_temp)
		{
			age_temp = pTemp->m_iAge;
			pYoung = pTemp;
		}
		pTemp = pTemp->m_pNext;
	}
	//print_one(pYoung);
	return(pYoung);
}

void LinkedList::sort()
{
	k_Student* pTemp = g_pHead;
	while (pTemp != g_pTail)
	{
		k_Student* youngman = find_young_man(pTemp);
		if (pTemp != youngman) // pTemp와 youngman이 같다면 굳이 멀쩡한 팔다리 짜를필요가 없음
		{
			raft(youngman);
			insertTargetBack(youngman, pTemp);
		}
		pTemp = pTemp->m_pNext;
	}
}

k_Student* LinkedList::find(int num) const
{
	k_Student* temp = g_pHead->m_pNext;
	while (temp != g_pTail)
	{
		if (temp->m_iNum == num)
		{
			return temp;
		}
		temp = temp->m_pNext;
	}
	return NULL;
}

k_Student* LinkedList::operator[](int index)
{
	k_Student* current = begin();
	for (int i = 0; i < index; i++) 
	{
		current = current->m_pNext;
	}
	return current;
}


k_Student* LinkedList::operator ++(void) 
{
	if (m_pCurr != NULL)
	{
		k_Student* temp = m_pCurr->m_pNext;
		m_pCurr = temp;
		return m_pCurr;
	};
}

k_Student* LinkedList::operator ++(int)
{
	if (m_pCurr != NULL)
	{
		k_Student* temp = m_pCurr->m_pNext;
		m_pCurr = temp;
		return m_pCurr;
	};
}
