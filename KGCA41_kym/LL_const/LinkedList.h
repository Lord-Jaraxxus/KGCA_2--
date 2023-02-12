#include "k_Student.h"

class LinkedList
{
public:
	LinkedList(); //������
	~LinkedList(); //�Ҹ��� , ��� �� ���� �̷��� �Ƚ���� �ڵ����� ����

protected:
	k_Student* g_pHead = nullptr;
	k_Student* g_pTail = nullptr;

public:
	static int m_iCount; // Ŭ���� ���ο� �ִ� static int�� �ܺο��� �ʱ�ȭ�������
	static k_Student* m_pCurr;

public:
	k_Student* begin() const;
	k_Student* end() const;

public:
	void insertFront(k_Student* new_student);
	void insertTargetFront(k_Student* new_student, k_Student* target_node);
	void insertBack(k_Student* new_student);
	void insertTargetBack(k_Student* new_student, k_Student* target_node);
	k_Student* find(int num) const;
	void death(k_Student* target_node);
	void raft(k_Student* target_node);
	k_Student* find_young_man(k_Student* temp) const;
	void sort();
	void H_T_link();

public:
	k_Student* operator[](int index);
	k_Student* operator ++(void);
	k_Student* operator ++(int);
};

