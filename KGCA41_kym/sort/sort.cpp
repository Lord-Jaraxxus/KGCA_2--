#include <stdio.h>
#include<stdlib.h> // rand
#include<time.h> // srand, etc

void SetData(int data[], int count)
{
	//srand((unsigned int)time(NULL));

	for (int a = 0; a < count; a++)
	{
		data[a] = (rand() % count) + 1; // 적어도 1부터 시작, 0 생기면 초기화한거랑 헷갈리니꼐
	}
}

void Swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void BubbleSort(int data[], int count)
{
	bool sort_complete = false;
	
	while (sort_complete == false) 
	{
		sort_complete = true;
		for (int i = 0; i < count-1; i++) 
		{
			if (data[i] > data[i + 1])
			{
				Swap(&data[i], &data[i + 1]);
				sort_complete = false;
			}
		}
		count--;
	}
}

void SelectSort(int data[], int count) 
{
	int i = 0;
	bool sort_complete = false;

	while (sort_complete == false) 
	{
		sort_complete = true;
		for (int j = i + 1; j < count; j++) 
		{
			if (data[j] < data[i]) 
			{
				Swap(&data[i], &data[j]);
				sort_complete = false;
			}
		}
		i++;
	}
}

void InsertSort(int data[], int count) 
{
	int j = 0;
	int temp;
	for (int i = 0; i < count; i++) 
	{
		temp = data[i];
		for (j = i - 1; j >= 0; j--) 
		{
			if (temp > data[j])
			{
				break;
			}
			data[j + 1] = data[j];
		}
		data[j + 1] = temp; // j가 0이면 j-- 을 실행하고 다음 조건문에 걸려서 끝남, 이때 j는 -1
							// 즉, 제일 작은 값이 들어왔어서 위의 for문이 끝까지 돌았다면 data[j + 1] 은 data[0]임
	}
}

void QuickSort(int data[], int start, int end) 
{
	//int blood_friend = 0;
	int big = 0;
	int small = end;
	int pivot = data[start];

	while (data[big] < pivot && big < end) { big++; }
	while (data[small] > pivot && small >start ) { small--; }
	if (big > small) { }
	else 
	{ 
		Swap(&data[big], &data[small]); 
		big++;
		small--;
	}
}


int main()
{
	int data[10000] = { 0, };
	SetData(data, 100);
	int test_data[6] = { 8, 5, 7, 1, 9, 3 };
	BubbleSort(data, 100);
	//SelectSort(data, 100);
	//InsertSort(data, 100);
	//QuickSort(data,);
	printf("bp더스");
}
