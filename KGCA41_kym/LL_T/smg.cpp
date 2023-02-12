#include <stdio.h>
#include "Student_manager.h"

void main() 
{
	Student_manager smg;

	int iWork = 1;
	int iWorkType = 0;

	while (iWork)
	{
		printf("1:초기화 2:추가 3:검색 4:앞에추가 5:뒤에추가 6:삭제 7:정렬 8:저장 9:로드 0:종료\n");
		scanf_s("%d", &iWorkType);
		system("cls"); // 위치는 여기가 맞네

		switch (iWorkType)
		{
		case 0:
		{
			printf("프로그램을 종료합니다.");
			iWork = 0;
		}break;
		case 1:
		{
			smg.ReleaseData();
			smg.InitData(10);
			smg.PrintAll();
		}break;
		case 2:
		{
			smg.m_List.insertBack(*smg.NewStudent()); 
			smg.PrintAll();
		}break;
		case 3:
		{
			smg.PrintAll();
			printf("찾을 학생의 번호를 입력하세요 : ");
			int find_num = -1;
			scanf_s("%d", &find_num);
			Box<k_Student>* target_student = smg.Find(find_num);
			system("cls");
			if (target_student == NULL) { printf("찾으시는 번호를 가진 학생이 없습니다.\n\n"); smg.PrintAll(); break; }
			smg.PrintOne(target_student);
			printf("\n\n");
			smg.PrintAll();
		}break;
		case 4:
		{
			smg.PrintAll();
			printf("앞에 새 학생을 추가할 학생 번호를 입력하세요 : ");
			int find_num = -1;
			scanf_s("%d", &find_num);
			Box<k_Student>* target_student = smg.Find(find_num);
			system("cls");
			if (target_student == NULL) { printf("찾으시는 번호를 가진 학생이 없습니다.\n\n"); smg.PrintAll(); break; }
			smg.m_List.insertTargetFront(*smg.NewStudent(), target_student);
			smg.PrintAll();
		}break;
		case 5:
		{
			smg.PrintAll();
			printf("뒤에 새 학생을 추가할 학생 번호를 입력하세요 : ");
			int find_num = -1;
			scanf_s("%d", &find_num);
			Box<k_Student>* target_student = smg.Find(find_num);
			system("cls");
			if (target_student == NULL) { printf("찾으시는 번호를 가진 학생이 없습니다.\n\n"); smg.PrintAll(); break; }
			smg.m_List.insertTargetBack(*smg.NewStudent(), target_student);
			smg.PrintAll();
		}break;
		case 6:
		{
			//printf("삭제해라 애송이 : ");
			smg.PrintAll();
			printf("삭제할 학생의 번호를 입력하세요 : ");
			int find_num = -1;
			scanf_s("%d", &find_num);
			Box<k_Student>* target_student = smg.Find(find_num);
			system("cls");
			if (target_student == nullptr) { printf("찾으시는 번호를 가진 학생이 없습니다.\n\n"); smg.PrintAll(); break; }
			smg.m_List.death(target_student);
			smg.PrintAll();
		}break;
		case 7:
		{
			smg.Sort();
			smg.PrintAll();
		}break;
		case 8:
		{
			smg.Save();
			smg.PrintAll();
		}break;
		case 9:
		{
			smg.Load();
			smg.PrintAll();
		}
		}
	}

	smg.ReleaseData();
}