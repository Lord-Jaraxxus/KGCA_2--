#include "LinkedList.h"

int LinkedList::m_iCount = 0;  // Ŭ���� ���� ����ƽ ������ �ݵ�� Ŭ���� �ܺο��� �ʱ�ȭ
k_Student* LinkedList::m_pCurr = nullptr;

LinkedList::LinkedList() // ������, �Ӹ��� ���� �ʱ�ȭ
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
	g_pTail->m_sName[3] = '\0';  // ������Ҷ� �� ���Ϸ���
}

LinkedList::~LinkedList() // �Ҹ���, �Ӹ��� ���� �޸� �Ҵ� ����
{
	delete g_pHead;
	delete g_pTail;
}

void LinkedList::H_T_link() // Student_manager���� ReleaseData�� �ϴ� ���� �Ŀ� �Ӹ��� ���� �ٽ� ����, ����ֳ�?
{
	g_pHead->m_pNext = g_pTail;
	g_pTail->m_pPrev = g_pHead;
}

k_Student* LinkedList::begin() const // head �ٷ� ���� ��带 ����
{
	k_Student* temp = g_pHead->m_pNext;
	return temp;
}

k_Student* LinkedList::end() const // Tail �ٷ� �� ��带 ����
{
	k_Student* temp = g_pTail->m_pPrev;
	return temp;
}

void LinkedList::insertFront(k_Student* new_student)
{
	k_Student* pTemp = g_pHead->m_pNext; // head �ٷ� ���� ��带 ����
	new_student->m_pNext = pTemp;
	new_student->m_pPrev = g_pHead;

	g_pHead->m_pNext = new_student;
	pTemp->m_pPrev = new_student;

	//new_student->m_pNext = g_pHead->m_pNext;
	//new_student->m_pPrev = g_pHead;
	//g_pHead->m_pNext->m_pPrev = new_student;
	//g_pHead->m_pNext = new_student;                  //- �Ƹ� �̷����ص� ���ƴ� ���ٵ� �ʹ� �����ؼ� Temp�� ���°ǰ�?
}

void LinkedList::insertTargetFront(k_Student* new_student, k_Student* target_node)
{
	_ASSERT(target_node != g_pHead); // head�տ� ������ ������� �����ϱ� ����ó��

	if (target_node == NULL) { insertFront(new_student); } // Ÿ�� ��尡 NULL�� ���ö�

	k_Student* pTemp = target_node->m_pPrev; // Ÿ�ٳ�� �ٷ� ���� ��带 ����
	new_student->m_pNext = target_node;
	new_student->m_pPrev = pTemp;

	target_node->m_pPrev = new_student;
	pTemp->m_pNext = new_student;
}

void LinkedList::insertBack(k_Student* new_student)
{
	k_Student* pTemp = g_pTail->m_pPrev; // Tail �ٷ� ���� ��带 ����
	new_student->m_pNext = g_pTail;
	new_student->m_pPrev = pTemp;

	pTemp->m_pNext = new_student;
	g_pTail->m_pPrev = new_student;

	//new_student->m_pNext = g_pTail;
	//new_student->m_pPrev = g_pTail->m_pPrev;
	//g_pTail->m_pPrev->m_pNext = new_student;
	//g_pTail->m_pPrev = new_student; // �̷��� �ɰͰ�����2
}

void LinkedList::insertTargetBack(k_Student* new_student, k_Student* target_node)
{
	_ASSERT(target_node != g_pTail); // Tail �ڿ� ������ ������� �����ϱ� ����ó��

	k_Student* pTemp = target_node->m_pNext; // Ÿ�ٳ�� �ٷ� ���� ��带 ����
	new_student->m_pNext = pTemp;
	new_student->m_pPrev = target_node;

	target_node->m_pNext = new_student;
	pTemp->m_pPrev = new_student;
}

void LinkedList::raft(k_Student* target_node) // ���Ŀ� ���� ���ؼ� ���Ḹ ����� free�� ���� ���� 
{
	target_node->m_pPrev->m_pNext = target_node->m_pNext;
	target_node->m_pNext->m_pPrev = target_node->m_pPrev;    // �̷��� �� ���ư���������
}

void LinkedList::death(k_Student* target_node)
{
	target_node->m_pPrev->m_pNext = target_node->m_pNext;
	target_node->m_pNext->m_pPrev = target_node->m_pPrev;    // �̷��� �� ���ư���������
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
		if (pTemp != youngman) // pTemp�� youngman�� ���ٸ� ���� ������ �ȴٸ� ¥���ʿ䰡 ����
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
