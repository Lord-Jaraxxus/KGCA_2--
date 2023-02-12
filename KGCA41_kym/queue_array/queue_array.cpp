#include<stdio.h>

int queue[5] = { 0, };
int back = 0; 

void in(int a);
int out();
void print_queue();

void main() 
{
	print_queue();

	in(1);
	in(2);
	in(3);
	in(4);
	out();
	print_queue();

	in(5);
	in(6);
	in(7);
	print_queue();

	out();
	out();
	out();
	out();
	out();
	out();

}

void in(int a) 
{
	if (back >= 5) { printf("큐 오버플로우\n"); }
	else 
	{
		queue[back] = a;
		back++;
	}
}

int out() 
{
	if (back <= 0 ) { printf("큐 언더플로우\n"); }
	else 
	{
		int temp = queue[0];
		queue[0] = 0;
		for (int i = 0; i < 4; i++) 
		{
			queue[i] = queue[i + 1]; // 하나 빼냈으니 왼쪽으로 한칸씩 땡김
		}
		back--;
		printf("%d out\n", temp);
		return temp;
	}
}

void print_queue()
{
	for (int i = 0; i < 5; i++)
	{
		printf("%d\n", queue[i]); 
	}
	printf("\n");
}