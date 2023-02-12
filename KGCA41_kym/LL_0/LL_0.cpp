#include "Linked_List.h"



void main() 
{
	Init();
	int iWork = 1;
	int iWorkType = 0;
	bool Init_Flag = false;

	while (iWork)
	{
		printf("1:초기화 및 전체출력 2:추가 3:검색 4:원하는위치뒤에추가 0:종료\n");
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
			if (Init_Flag == false) { InitData(10); Init_Flag = true; }
			print_all();
		}break;
		case 2:
		{
			insertBack(NewStudent());
			print_all();
		}break;
		case 3:
		{
			printf("찾을 번호를 입력하세 : ");
			int find_num = -1;
			scanf_s("%d", &find_num);
			if (find(find_num) == NULL) { printf("찾으시는 번호가 없습니다.\n"); }
		}break;
		case 4:
		{
			printf("뒤에 추가할 학생 번호를 입력하세 : ");
			int find_num = -1;
			scanf_s("%d", &find_num);
			k_Student* middle_insert = find(find_num);
			if (middle_insert == NULL) { printf("찾으시는 번호가 없습니다.\n"); }
			insertFront(NewStudent(), middle_insert);
		}break;
		}
	}

	ReleaseAll();

	//Init();
	//InitData(10);
	//insertFront(NewStudent());
	//insertBack(NewStudent());
	//print_all();
	//printf("\n");
	//find(7);
	//ReleaseAll();
}





