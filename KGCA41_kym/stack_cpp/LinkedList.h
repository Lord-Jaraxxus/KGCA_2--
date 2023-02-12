#include "k_Student.h"

class LinkedList
{
public:
	LinkedList(); //������
	~LinkedList(); //�Ҹ��� , ��� �� ���� �̷��� �Ƚ���� �ڵ����� ����

protected:
	k_Student* g_pHead = NULL;
	k_Student* g_pTail = NULL;

public:
	static int m_iCount; // Ŭ���� ���ο� �ִ� static int�� �ܺο��� �ʱ�ȭ�������

public:
	k_Student* begin();
	k_Student* end();

public:
	void insertFront(k_Student* new_student);
	void insertTargetFront(k_Student* new_student, k_Student* target_node);
	void insertBack(k_Student* new_student);
	void insertTargetBack(k_Student* new_student, k_Student* target_node);
	k_Student* find(int num);
	void death(k_Student* target_node);
	void raft(k_Student* target_node);
	k_Student* find_young_man(k_Student* temp);
	void sort();
	void H_T_link();
};

