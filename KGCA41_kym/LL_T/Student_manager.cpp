#define _CRT_SECURE_NO_WARNINGS
#include "Student_manager.h"

k_Student* Student_manager::NewStudent() //  
{
	k_Student* new_student = new k_Student; // ���� ��带 �ϳ�����
	memset(new_student, 0, sizeof(k_Student));

	new_student->m_iNum = LinkedList<k_Student>::m_iCount;
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

	LinkedList<k_Student>::m_iCount++;
	return new_student;
}


void Student_manager::InitData(int student_num) // �������� student_num �� ��ŭ �л��� ����� ������ �ο�
{
	for (int i = 0; i < student_num; i++)
	{
		m_List.insertBack(*NewStudent());
	}
}

void Student_manager::PrintOne(Box<k_Student>* target_node)
{
	printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%10.4f\n",
		target_node->m_Data.m_iNum,
		target_node->m_Data.m_sName,
		target_node->m_Data.m_iAge,
		target_node->m_Data.m_iKor,
		target_node->m_Data.m_iEng,
		target_node->m_Data.m_iMath,
		target_node->m_Data.m_iTotal,
		target_node->m_Data.m_fAverage);
}

void Student_manager::PrintAll()
{
	printf("��ȣ\t�̸�\t����\t����\t����\t����\t����\t  ���\n");
	Box<k_Student>* temp = m_List.begin();
	while (temp->m_pPrev != m_List.end())
	{
		PrintOne(temp);
		temp = temp->m_pNext;
	}
}

void Student_manager::ReleaseData()
{
	Box<k_Student>* temp = m_List.begin();

	while (temp->m_pPrev != m_List.end())
	{
		Box<k_Student>* next = temp->m_pNext;
		delete(temp);
		temp = next;
	}
	m_List.H_T_link(); // �����ڷ� ��ü�ϸ� �굵 ��ü
	LinkedList<k_Student>::m_iCount = 0;
}

Box<k_Student>* Student_manager::Find(int num) 
{
	Box<k_Student>* temp = m_List.begin();

	while (temp != m_List.end()->m_pNext)
	{
		if (temp->m_Data.m_iNum == num)
		{
			return temp;
		}
		else 
		{
			Box<k_Student>* next = temp->m_pNext;
			temp = next;
		}
	}
	return nullptr;
}

// �Ű������� ������ �л� ����� ���� ������ ���������� ���� �� ���� ���̰� ���� �л� ��带 ����, sort�� ������ ����
Box<k_Student>* Student_manager::FindYoungest(Box<k_Student>* start_node)
{
	if (start_node->m_pNext == m_List.m_pTail) { return start_node; }

	Box<k_Student>* pTemp = start_node->m_pNext;
	Box<k_Student>* pYoung = nullptr;
	int age_temp = 101;

	while (pTemp != m_List.m_pTail) 
	{
		if (pTemp->m_Data.m_iAge < age_temp) 
		{
			age_temp = pTemp->m_Data.m_iAge;
			pYoung = pTemp;
		}
		pTemp = pTemp->m_pNext;
	}
	return(pYoung);
}

// �ϴ� ���̼����� ����
void Student_manager::Sort() 
{
	Box<k_Student>* pTemp = m_List.begin()->m_pPrev; // pTemp�� head�� �ʱ�ȭ, �ƴϱٵ� �̷������� ���� �����ϸ� �� �׷��� �ʳ�

	while(pTemp != m_List.m_pTail) 
	{
		Box<k_Student>* pYoungman = FindYoungest(pTemp);
		if (pTemp != pYoungman) // pTemp�� youngman�� ���ٸ� ���� ������ �ȴٸ� ¥���ʿ䰡 ����
		{
			m_List.raft(pYoungman);

			pYoungman->m_pPrev = pTemp;
			pYoungman->m_pNext = pTemp->m_pNext;
			pTemp->m_pNext->m_pPrev = pYoungman;
			pTemp->m_pNext = pYoungman; // ���� insertTargetBack���� �ϴ� ���Ÿ����µ�  
		}
		pTemp = pTemp->m_pNext;
	}
}

void Student_manager::Save()
{
	FILE* fp = fopen("save.txt", "w");
	Box<k_Student>* temp = m_List.begin();

	while (temp != m_List.end()->m_pNext)
	{
		fprintf(fp, "%d	%s	%d	%d	%d	%d	%d	%10.4f \n",
			temp->m_Data.m_iNum,
			temp->m_Data.m_sName,
			temp->m_Data.m_iAge,
			temp->m_Data.m_iKor,
			temp->m_Data.m_iEng,
			temp->m_Data.m_iMath,
			temp->m_Data.m_iTotal,
			temp->m_Data.m_fAverage);
		temp = temp->m_pNext;
	}
	fclose(fp);
}

void Student_manager::Load()
{
	ReleaseData(); // �ϴ� �밡�� �������� �� ���ֻ�

	FILE* fp = fopen("save.txt", "r");
	if (fp == NULL)
	{
		puts("n������ �������� �ʽ��ϴ�.");
		return;
	}

	k_Student* temp = nullptr;
	while (!feof(fp))
	{
		temp = NewStudent();

		fscanf(fp, "%d	%s	%d	%d	%d	%d	%d	%f\n",
			&temp->m_iNum,
			temp->m_sName,
			&temp->m_iAge,
			&temp->m_iKor,
			&temp->m_iEng,
			&temp->m_iMath,
			&temp->m_iTotal,
			&temp->m_fAverage);

		m_List.insertBack(*temp);
	}
	fclose(fp);
 }