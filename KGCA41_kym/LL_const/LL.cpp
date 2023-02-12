#include "Student_manager.h"

void main()
{
	Student_manager std_mng;
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
			std_mng.InitData(4);
			std_mng.print_all();
		}break;
		case 2:
		{
			std_mng.m_List.insertBack(std_mng.NewStudent()); // 돌겠네 ㅋㅋ 이거 맞음? 겁나 주렁주렁이네
			std_mng.print_all();
		}break;
		case 3:
		{
			printf("찾을 번호를 입력하세 : ");
			int find_num = -1;
			scanf_s("%d", &find_num);
			if (std_mng.m_List.find(find_num) == NULL) { printf("찾으시는 번호가 없습니다.\n"); break; }
			else 
			{
				std_mng.print_one(std_mng.m_List.find(find_num));
				//std_mng.print_one(std_mng.m_List[find_num]); //- []오버로딩 확인용, 잘 작동하는구먼
			}
		}break;
		case 4:
		{
			printf("앞에 추가할 학생 번호를 입력하세 : ");
			int find_num = -1;
			scanf_s("%d", &find_num);
			k_Student* target_student = std_mng.m_List.find(find_num);
			if (target_student == NULL) { printf("찾으시는 번호가 없습니다.\n"); break; }
			std_mng.m_List.insertTargetFront(std_mng.NewStudent(), target_student);
			system("cls");
			std_mng.print_all();
		}break;
		case 5:
		{
			printf("뒤에 추가할 학생 번호를 입력하세 : ");
			int find_num = -1;
			scanf_s("%d", &find_num);
			k_Student* target_student = std_mng.m_List.find(find_num);
			if (target_student == NULL) { printf("찾으시는 번호가 없습니다.\n"); break; }
			std_mng.m_List.insertTargetBack(std_mng.NewStudent(), target_student);
			system("cls");
			std_mng.print_all();
		}break;
		case 6:
		{
			printf("삭제해라 애송이 : ");
			int find_num = -1;
			scanf_s("%d", &find_num);
			k_Student* target_student = std_mng.m_List.find(find_num);
			if (target_student == NULL) { printf("찾으시는 번호가 없습니다.\n"); break; }
			std_mng.m_List.death(target_student);
			std_mng.print_all();
		}break;
		case 7:
		{
			std_mng.m_List.sort();
			std_mng.print_all();
		}break;
		case 8:
		{
			std_mng.save();
			std_mng.print_all();
		}break;
		case 9:
		{
			std_mng.load();
			//print_all();
		}
		}
	}

	std_mng.ReleaseData();

	//Init();
	//InitData(10);
	//insertFront(NewStudent());
	//insertBack(NewStudent());
	//print_all();
	//printf("\n");
	//find(7);
	//ReleaseAll();
}