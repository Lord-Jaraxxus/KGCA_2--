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
k_Student* g_pAss = NULL; // Tail 바로 앞 노드를 저장, insertBack에 사용


k_Student* NewStudent();

void Init(); // head, tail, ass만 초기화

void InitData(int student_num);

void insertFront(k_Student* new_student, k_Student* target_node);

void insertBack(k_Student* new_student);

void print_one(k_Student* a);

void print_all();

k_Student* find(int num);

void Release();

void ReleaseData();

void ReleaseAll();









void GRV(k_Student* new_student) // give random value, 받아온 구조체의 멤버들에게 랜덤값을 부여해줌 
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
	k_Student* new_student = (k_Student*)malloc(sizeof(k_Student)); // 새로 노드를 하나만듬
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
		//k_Student* pTemp = g_pHead->m_pNext; // head 바로 뒤의 노드를 저장
		//new_student->m_pNext = pTemp;
		//new_student->m_pPrev = g_pHead;

		//g_pHead->m_pNext = new_student;
		//pTemp->m_pPrev = new_student;

		new_student->m_pNext = g_pHead->m_pNext;
		new_student->m_pPrev = g_pHead;
		g_pHead->m_pNext->m_pPrev = new_student;
		g_pHead->m_pNext = new_student;                  //- 아마 이러케해도 돌아는 갈텐데 너무 복잡해서 Temp를 쓰는건가?


		//while (g_pAss->m_pNext != g_pTail) // insertFront 먼저하고 insertBack 나중에하면 Ass가 Head라서 Front로 넣은거 다 짤림, 이거해줘야댐
		//{									// 근데 굳이 while일 필요가 있나? 한번만 밀어주면 되긴 하는데
		//	g_pAss = g_pAss->m_pNext;
		//}
	}

	else
	{
		k_Student* pTemp = target_node->m_pNext; // 타겟노드 바로 뒤의 노드를 저장
		new_student->m_pNext = pTemp;
		new_student->m_pPrev = target_node;

		target_node->m_pNext = new_student;
		pTemp->m_pPrev = new_student;
		//while (g_pAss->m_pNext != g_pTail) // insertFront 먼저하고 insertBack 나중에하면 Ass가 Head라서 Front로 넣은거 다 짤림, 이거해줘야댐
		//{									// 여기도 넣어줘야겠네 ASS를 타겟으로 붙여버리면 얘가 ASS가 되어야하니까
		//	g_pAss = g_pAss->m_pNext;
		//}
	}
	g_iCount++;
}

void insertBack(k_Student* new_student)
{
	//k_Student* pTemp = g_pTail->m_pPrev; // Tail 바로 앞의 노드를 저장
	//new_student->m_pNext = g_pTail;
	//new_student->m_pPrev = pTemp;

	//pTemp->m_pNext = new_student;
	//g_pTail->m_pPrev = new_student;
	

	new_student->m_pNext = g_pTail;
	new_student->m_pPrev = g_pTail->m_pPrev;
	g_pTail->m_pPrev->m_pNext = new_student;
	g_pTail->m_pPrev = new_student; // 이래도 될것같은디2
	
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

void InitData(int student_num) // 랜덤으로 student_num 명 만큼 학생을 만들고 데이터 부여
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