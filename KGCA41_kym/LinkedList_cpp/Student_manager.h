#include "LinkedList.h"

class Student_manager
{

public:
	LinkedList m_List; // ��ü�� ������ ����� ���ϴϱ� �ϳ� ����?��?

public:
	k_Student* NewStudent();

	void GRV(k_Student* new_student);

	void InitData(int student_num);

	void ReleaseData();

	void print_one(k_Student* a);

	void print_all();

	void save();

	void load();

};

