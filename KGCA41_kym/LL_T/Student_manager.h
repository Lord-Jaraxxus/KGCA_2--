#include "LinkedList.h"
#include "k_Student.h"

class Student_manager
{

public:
	LinkedList<k_Student> m_List; // ��ü�� ������ ����� ���ϴϱ� �ϳ� ����?��?

public:
	k_Student* NewStudent();
	void InitData(int student_num);
	void ReleaseData();

	void PrintOne(Box<k_Student>* target_node);
	void PrintAll();

	Box<k_Student>* Find(int num);

	Box<k_Student>* FindYoungest(Box<k_Student>* start_node);
	void Sort();

	void Save();
	void Load();

};

