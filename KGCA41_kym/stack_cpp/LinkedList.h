#include "k_Student.h"

class LinkedList
{
public:
	LinkedList(); //생성자
	~LinkedList(); //소멸자 , 사실 얘 둘은 이러케 안써놔도 자동으로 생김

protected:
	k_Student* g_pHead = NULL;
	k_Student* g_pTail = NULL;

public:
	static int m_iCount; // 클래스 내부에 있는 static int는 외부에서 초기화해줘야함

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

