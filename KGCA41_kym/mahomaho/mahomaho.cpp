#include <stdio.h>
#include <stdlib.h>

void main()
{
	const int max = 9;
	int x = max / 2;
	int y = 0;

	int db_array[max * max][2]; // 마방진 블록 갯수만큼 n*n , xy값 넣게 이중배열로
	for (int icnt = 0; icnt < max * max; icnt++)
	{
		db_array[icnt][0] = 99;
		db_array[icnt][1] = 99;        //이중for문으로 초기화해도 되는데 그냥 귀찮아서 - 0,0으로 초기화하니까 좌표찍다오류남; +1 해줘야하나
	}
	//for (int i = 0; i < max * max; i++) printf("%d", db_array[i][0]);

	for (int i = 0; i < max * max; i++)
	{
		db_array[i][0] = x;
		db_array[i][1] = y;
		x++; y--;

		if (x > max - 1 && y < 0)
		{
			x--; y++; y++;
		} // 맵 뚫었는데 둘다 뚫으면 바로 아래로

		else if (x > max - 1)
		{
			x = x - max;
		} // x 맵뚫었으면 젤 왼쪽으로
		else if (y < 0)
		{
			y = y + max;
		} // y 맵뚫었으면 젤 아래쪽으로

		for (int j = 0; j < max * max; j++) // 겹치는거 있나 검사, 좀 무식한 방법인데 일단 귀찮으니까
		{
			if (db_array[j][0] == x && db_array[j][1] == y) { x--; y++; y++;  break; } // 겹치면 바로아래로
		}
	}

	
		for (int y_v = 0; y_v < max; y_v++)  // 3중 for문ㅋㅋㅋㅋ 쥰내무식함ㅋㅋㅋ
		{
			for (int x_v = 0; x_v < max; x_v++) 
			{
				for (int k=0; k<max*max; k++) 
				{
					if (db_array[k][0] == x_v && db_array[k][1] == y_v) { printf("%d\t", k + 1); break; }
				}
			}
			printf("\n");
		}

		printf("?");
	}
