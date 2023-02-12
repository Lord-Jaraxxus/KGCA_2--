#include "k_Student.h"

class LinkedList
{
public:
	LinkedList(); //생성자
	~LinkedList(); //소멸자 , 사실 얘 둘은 이러케 안써놔도 자동으로 생김

protected:
	k_Student* g_pHead = nullptr;
	k_Student* g_pTail = nullptr;

public:
	static int m_iCount; // 클래스 내부에 있는 static int는 외부에서 초기화해줘야함
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

