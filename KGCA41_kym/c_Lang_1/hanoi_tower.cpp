#include <stdio.h>
#include <string>

//int check(char *rings)                   //<<일단 그냥 안에다 넣어놔야쓰겠는데
//{
//	for (int i = 0; i < (sizeof(rings)/sizeof(char)) -1; i++)
//	{
//		//if (rings[i] == 'C') { printf("ring[%d]는 C에있음\n",i);}
//		//else { return 0; }
//		if (rings[i] != 'C') { return 0; }
//	}
//	printf("모든 링이 C로 옮겨짐");
//	return 1;
//}

void main()
{
	const int num = 10;
	char tower[4] = { "ABC" };
	char ring[num + 1] = { "" };  // 문자열의 마지막은 널값으로 끝내줘야 야랄안남
	bool finish = false;
	int count = 0;
	bool first_ling_flag = true;
	int tower_num = 2;

	for (int i = 0; i < sizeof(ring) / sizeof(char); i++) { ring[i] = tower[0]; }  //{ ring[i] = i + 1; printf("%d \n", ring[i]); }

	ring[num] = '\0';

	while (finish == 0)
	{
		if (num % 2 == 1) // 링 갯수가 홀수일 경우, C시작 C->B->A
		{
			if (first_ling_flag == true)
			{
				printf("0번링 %c -> %c \n", ring[0], tower[tower_num % 3]);
				ring[0] = tower[tower_num % 3];
				tower_num += 2;
				first_ling_flag = false;
				count++;
			}
			else if (first_ling_flag == false)
			{
				for (int j = 1; j < num; j++)
				{
					if (first_ling_flag == true) break; // 이중for문 탈출용

					if (ring[j] != ring[0]) // a랑 같이있지 않은놈들중에서 제일작은놈 
					{
						
						for (int k = 0; k < 3; k++)
						{
							if (tower[k] != ring[j] && tower[k] != ring[0]) // 1번링이랑 같은위치도 아니며, 지금 있는 탑도 아닌 위치로 이동
							{
								printf("%d번링 %c -> %c \n", j, ring[j], tower[k]);
								ring[j] = tower[k];
								first_ling_flag = true;
								count++;
								break;
							}
						}
					}
				}
			}
		}
		else if (num % 2 == 0) // 링 갯수가 짝수일 경우, B시작 B->C->A  : tower_num++ 하면댐
		{
			if (first_ling_flag == true)
			{
				printf("0번링 %c -> %c \n", ring[0], tower[tower_num % 3]);
				ring[0] = tower[(tower_num + 2) % 3];
				tower_num++;
				first_ling_flag = false;
				count++;
			}
			else if (first_ling_flag == false)
			{
				for (int j = 1; j < num; j++)
				{
					if (first_ling_flag == true) break; // 이중for문 탈출용

					if (ring[j] != ring[0]) // a랑 같이있지 않은놈들중에서 제일작은놈 
					{
						for (int k = 0; k < 3; k++)
						{
							if (tower[k] != ring[j] && tower[k] != ring[0]) // 1번링이랑 같은위치도 아니며, 지금 있는 탑도 아닌 위치로 이동
							{
								printf("%d번링 %c -> %c \n", j, ring[j], tower[k]);
								ring[j] = tower[k];
								first_ling_flag = true;
								count++;
								break;
							}
						}
					}
				}
			}
		}
		else { printf("?망함"); }

		finish = 1;
		for (int i = 0; i < (sizeof(ring) / sizeof(char)) - 1; i++)
		{
			if (ring[i] != 'C') { finish = 0; break; }
		}
	}

	printf("모든 링이 C타워로 이동했습니다. %d회 이동 \n", count);
}