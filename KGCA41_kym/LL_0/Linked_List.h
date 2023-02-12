#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void insertFront(k_Student* new_student, k_Student* target_node);

void insertBack(k_Student* new_student);

void print_one(k_Student* a);

void print_all();

k_Student* find(int num);

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

void insertFront(k_Student* new_student, k_Student* target_node)
{
	if (target_node == NULL)
	{
		//k_Student* pTemp = g_pHead->m_pNext; // head �ٷ� ���� ��带 ����
		//new_student->m_pNext = pTemp;
		//new_student->m_pPrev = g_pHead;

		//g_pHead->m_pNext = new_student;
		//pTemp->m_pPrev = new_student;

		new_student->m_pNext = g_pHead->m_pNext;
		new_student->m_pPrev = g_pHead;
		g_pHead->m_pNext->m_pPrev = new_student;
		g_pHead->m_pNext = new_student;                  //- �Ƹ� �̷����ص� ���ƴ� ���ٵ� �ʹ� �����ؼ� Temp�� ���°ǰ�?


		//while (g_pAss->m_pNext != g_pTail) // insertFront �����ϰ� insertBack ���߿��ϸ� Ass�� Head�� Front�� ������ �� ©��, �̰�����ߴ�
		//{									// �ٵ� ���� while�� �ʿ䰡 �ֳ�? �ѹ��� �о��ָ� �Ǳ� �ϴµ�
		//	g_pAss = g_pAss->m_pNext;
		//}
	}

	else
	{
		k_Student* pTemp = target_node->m_pNext; // Ÿ�ٳ�� �ٷ� ���� ��带 ����
		new_student->m_pNext = pTemp;
		new_student->m_pPrev = target_node;

		target_node->m_pNext = new_student;
		pTemp->m_pPrev = new_student;
		//while (g_pAss->m_pNext != g_pTail) // insertFront �����ϰ� insertBack ���߿��ϸ� Ass�� Head�� Front�� ������ �� ©��, �̰�����ߴ�
		//{									// ���⵵ �־���߰ڳ� ASS�� Ÿ������ �ٿ������� �갡 ASS�� �Ǿ���ϴϱ�
		//	g_pAss = g_pAss->m_pNext;
		//}
	}
	g_iCount++;
}

void insertBack(k_Student* new_student)
{
	//k_Student* pTemp = g_pTail->m_pPrev; // Tail �ٷ� ���� ��带 ����
	//new_student->m_pNext = g_pTail;
	//new_student->m_pPrev = pTemp;

	//pTemp->m_pNext = new_student;
	//g_pTail->m_pPrev = new_student;
	

	new_student->m_pNext = g_pTail;
	new_student->m_pPrev = g_pTail->m_pPrev;
	g_pTail->m_pPrev->m_pNext = new_student;
	g_pTail->m_pPrev = new_student; // �̷��� �ɰͰ�����2
	
	g_iCount++;

	//new_student->m_pNext = g_pTail;
	//g_pAss->m_pNext = new_student;
	//g_pAss = new_student;
	//g_iCount++;
}

void Init()
{
	g_pHead = (k_Student*)malloc(sizeof(k_Student));
	memset(g_pHead, 0, sizeof(k_Student));
	g_pTail = (k_Student*)malloc(sizeof(k_Student));
	memset(g_pTail, 0, sizeof(k_Student));

	g_pHead->m_pNext = g_pTail;
	g_pTail->m_pPrev = g_pHead;
	g_pAss = g_pHead;
}

void InitData(int student_num) // �������� student_num �� ��ŭ �л��� ����� ������ �ο�
{

	//for (int i = 0; i < student_num; i++) 
	//{
	//	insertFront(NewStudent());
	//}

	for (int i = student_num; i < student_num * 2; i++)
	{
		insertBack(NewStudent());
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
			print_one(temp);
			return temp;
		}
		temp = temp->m_pNext;
	}
	return NULL;
}