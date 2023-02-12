#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h>

int g_iCount = 0;

struct k_Student {
	int		m_iNum;
	int		m_iAge;
	int		m_iKor;
	int		m_iEng;
	int		m_iMath;
	int		m_iTotal;
	float	m_fAverage;
	char	m_sName[4];
	k_Student* m_pPrev;
	k_Student* m_pNext;
};

k_Student* g_pHead = NULL;
k_Student* g_pTail = NULL;
k_Student* g_pAss = NULL; // Tail �ٷ� �� ��带 ����, insertBack�� ���


k_Student* NewStudent();

void Init(); // head, tail, ass�� �ʱ�ȭ

void InitData(int student_num);

void insertFront(k_Student* new_student);

void insertTargetFront(k_Student* new_student, k_Student* target_node);

void insertBack(k_Student* new_student);

void insertTargetBack(k_Student* new_student, k_Student* target_node);

void print_one(k_Student* a);

void print_all();

k_Student* find(int num);

void death(k_Student* target_node);

k_Student* find_young_man(k_Student* temp);

void sort();

void save();

void load();

void Release();

void ReleaseData();

void ReleaseAll();









void GRV(k_Student* new_student) // give random value, �޾ƿ� ����ü�� ����鿡�� �������� �ο����� 
{
	new_student->m_iNum = g_iCount;
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

k_Student* NewStudent() //  
{
	k_Student* new_student = (k_Student*)malloc(sizeof(k_Student)); // ���� ��带 �ϳ�����
	memset(new_student, 0, sizeof(k_Student));

	new_student->m_iNum = g_iCount;
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

	g_iCount++;
	return new_student;
}

void print_one(k_Student* a)
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

void print_all()
{
	k_Student* temp = g_pHead->m_pNext;
	while (temp != g_pTail)
	{
		print_one(temp);
		temp = temp->m_pNext;
	}
}

void insertFront(k_Student* new_student)
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

void insertTargetFront(k_Student* new_student, k_Student* target_node)
{
	_ASSERT(target_node != g_pHead); // head�տ� ������ ������� �����ϱ� ����ó��

	if (target_node == NULL) { insertFront(new_student); } // Ÿ�� ��尡 NULL�� ���ö�

	k_Student* pTemp = target_node->m_pPrev; // Ÿ�ٳ�� �ٷ� ���� ��带 ����
	new_student->m_pNext = target_node;
	new_student->m_pPrev = pTemp;

	target_node->m_pPrev = new_student;
	pTemp->m_pNext = new_student;
}

void insertBack(k_Student* new_student)
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

void insertTargetBack(k_Student* new_student, k_Student* target_node) 
{
	_ASSERT(target_node != g_pTail); // Tail �ڿ� ������ ������� �����ϱ� ����ó��

	k_Student* pTemp = target_node->m_pNext; // Ÿ�ٳ�� �ٷ� ���� ��带 ����
	new_student->m_pNext = pTemp;
	new_student->m_pPrev = target_node;

	target_node->m_pNext = new_student;
	pTemp->m_pPrev = new_student;
}

void Init()
{
	g_pHead = (k_Student*)malloc(sizeof(k_Student));
	memset(g_pHead, 0, sizeof(k_Student));
	g_pTail = (k_Student*)malloc(sizeof(k_Student));
	memset(g_pTail, 0, sizeof(k_Student));

	g_pHead->m_pNext = g_pTail;
	g_pTail->m_pPrev = g_pHead;
	//g_pAss = g_pHead; - �����̴ϱ� �̵� BADASS�� �ʿ����

	g_pHead->m_sName[0] = 'H';
	g_pHead->m_sName[1] = 'D';
	g_pHead->m_sName[2] = ' ';
	g_pHead->m_sName[3] = '\0';
	g_pTail->m_sName[0] = 'T';
	g_pTail->m_sName[1] = 'L';
	g_pTail->m_sName[2] = ' ';
	g_pTail->m_sName[3] = '\0';  // ������Ҷ� �� ���Ϸ���
}

void InitData(int student_num) // �������� student_num �� ��ŭ �л��� ����� ������ �ο�
{
	ReleaseData(); // �ϴ�������

	for (int i = 0; i < student_num; i++) 
	{
		insertFront(NewStudent());
	}

	for (int i = student_num; i < student_num * 2; i++)
	{
		insertBack(NewStudent());
	}
}

void raft(k_Student* target_node) // ���Ŀ� ���� ���ؼ� ���Ḹ ����� free�� ���� ���� 
{
	target_node->m_pPrev->m_pNext = target_node->m_pNext;
	target_node->m_pNext->m_pPrev = target_node->m_pPrev;    // �̷��� �� ���ư���������
	//free(target_node);
}

void death(k_Student* target_node)
{
	target_node->m_pPrev->m_pNext = target_node->m_pNext;
	target_node->m_pNext->m_pPrev = target_node->m_pPrev;    // �̷��� �� ���ư���������
	free(target_node);
}

k_Student* find_young_man(k_Student* temp)
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

void sort() 
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

void Release()
{
	free(g_pHead);
	free(g_pTail);
}

void ReleaseData()
{
	k_Student* temp = g_pHead->m_pNext;

	while (temp != g_pTail)
	{
		k_Student* next = temp->m_pNext;
		free(temp);
		temp = next;
	}
	g_pHead->m_pNext = g_pTail;
	g_pTail->m_pPrev = g_pHead;
	g_iCount = 0;                                            
}

void ReleaseAll()
{
	k_Student* temp = g_pHead->m_pNext;
	free(g_pHead);

	while (temp != g_pTail)
	{
		k_Student* next = temp->m_pNext;
		free(temp);
		temp = next;
	}
	free(g_pTail);
}

k_Student* find(int num)
{
	k_Student* temp = g_pHead->m_pNext;
	while (temp != g_pTail)
	{
		if (temp->m_iNum == num)
		{
			//print_one(temp); //�߳�����Ȯ�ο�
			return temp;
		}
		temp = temp->m_pNext;
	}
	return NULL;
}

void save() 
{
	FILE* fp = fopen("save.txt", "w");
	k_Student* temp = g_pHead->m_pNext;

	while (temp != g_pTail)
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

void load() 
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
		insertBack(temp);
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
