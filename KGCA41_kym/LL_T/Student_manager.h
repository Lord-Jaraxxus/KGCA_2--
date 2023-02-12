#include "LinkedList.h"
#include "k_Student.h"

class Student_manager
{

public:
	LinkedList<k_Student> m_List; // 실체가 없으면 써먹질 못하니까 하나 만든?듯?

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

