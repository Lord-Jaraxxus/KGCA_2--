#include "Student_manager.h"

k_Student* Student_manager::NewStudent() //  
{
	k_Student* new_student = (k_Student*)malloc(sizeof(k_Student)); // 새로 노드를 하나만듬
	memset(new_student, 0, sizeof(k_Student));

	new_student->m_iNum = LinkedList::m_iCount;
	new_student->m_sName[0] = 65 + rand() % 26;
	new_student->m_sName[1] = 65 + rand() % 26;
	new_student->m_sName[2] = 65 + rand() % 26;
	new_student->m_sName[3] = 0;
	new_student->m_iAge = 10 + rand() % 91;
	new_student->m_iKor = 0 + rand() % 101;
	new_student->m_iEng = 0 + rand() % 101;
	new_student->m_iMath = 0 + rand() % 101;
	new_student->m_iTotal = new_student->m_iKor + new_student->m_iEng + new_student->m_iMath;
	new_student->m_fAverage = new_student->m_iTotal / 3;

	LinkedList::m_iCount++;
	return new_student;
}

void Student_manager::GRV(k_Student* new_student) // give random value, 받아온 구조체의 멤버들에게 랜덤값을 부여해줌 
{
	new_student->m_iNum = LinkedList::m_iCount;
	new_student->m_sName[0] = 65 + rand() % 26;
	new_student->m_sName[1] = 65 + rand() % 26;
	new_student->m_sName[2] = 65 + rand() % 26;
	new_student->m_sName[3] = 0;
	new_student->m_iAge = 10 + rand() % 91;
	new_student->m_iKor = 0 + rand() % 101;
	new_student->m_iEng = 0 + rand() % 101;
	new_student->m_iMath = 0 + rand() % 101;
	new_student->m_iTotal = new_student->m_iKor + new_student->m_iEng + new_student->m_iMath;
	new_student->m_fAverage = new_student->m_iTotal / 3;
	new_student->m_pNext = new_student;
}

void Student_manager::InitData(int student_num) // 랜덤으로 student_num 명 만큼 학생을 만들고 데이터 부여
{

	for (int i = 0; i < student_num; i++)
	{
		m_List.insertFront(NewStudent());
	}

	for (int i = student_num; i < student_num * 2; i++)
	{
		m_List.insertBack(NewStudent());
	}
}

void Student_manager::print_one(k_Student* a)
{
	printf("%d	%s	%d	%d	%d	%d	%d	%10.4f\n",
		a->m_iNum,
		a->m_sName,
		a->m_iAge,
		a->m_iKor,
		a->m_iEng,
		a->m_iMath,
		a->m_iTotal,
		a->m_fAverage);
}

void Student_manager::print_all()
{
	k_Student* temp = m_List.begin();
	while (temp->m_pPrev != m_List.end())
	{
		print_one(temp);
		temp = temp->m_pNext;
	}
}

void Student_manager::ReleaseData()
{
	k_Student* temp = m_List.begin();

	while (temp->m_pPrev != m_List.end())
	{
		k_Student* next = temp->m_pNext;
		free(temp);
		temp = next;
	}
	m_List.H_T_link(); // 생성자로 교체하면 얘도 교체
	LinkedList::m_iCount = 0;
}

void Student_manager::save()
{
	FILE* fp = fopen("save.txt", "w");
	k_Student* temp = m_List.begin();

	while (temp->m_pNext != m_List.end())
	{
		fprintf(fp, "%d	%s	%d	%d	%d	%d	%d	%10.4f \n",
			temp->m_iNum,
			temp->m_sName,
			temp->m_iAge,
			temp->m_iKor,
			temp->m_iEng,
			temp->m_iMath,
			temp->m_iTotal,
			temp->m_fAverage);
		temp = temp->m_pNext;
	}
	fclose(fp);
}

void Student_manager::load()
{
	ReleaseData(); // 일단 대가리 꼬리빼고 다 없애삠

	FILE* fp = fopen("save.txt", "r");
	if (fp == NULL)
	{
		puts("n파일이 존재하지 않습니다.");
		return;
	}

	k_Student* temp = nullptr;
	while (!feof(fp))
	{
		temp = NewStudent();
		m_List.insertBack(temp);
		fscanf(fp, "%d	%s	%d	%d	%d	%d	%d	%f\n",
			&temp->m_iNum,
			temp->m_sName,
			&temp->m_iAge,
			&temp->m_iKor,
			&temp->m_iEng,
			&temp->m_iMath,
			&temp->m_iTotal,
			&temp->m_fAverage);
		print_one(temp);
	}
	fclose(fp);
}