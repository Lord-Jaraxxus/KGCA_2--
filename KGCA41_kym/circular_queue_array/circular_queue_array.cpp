// 원형 큐에서 한자리 안비우고 내맘대로 구현한 버전

#include <stdio.h>

int cir_cue[5] = { 0, };
int front = 0;
int back = 0;

bool is_empty();
bool is_full();
void in(int a);
int out();
void print_cirque();

void main() 
{
	in(1);
	in(2);
	in(3);
	in(4);
	out();
	out();
	in(5);
	out();
	in(6);
	in(7);
	in(8);
	in(9);
	print_cirque();

	out();
	out();
	out();
	in(10);
	in(11);
	out();
	out();
	out();
	out();
	out();
}

bool is_empty()
{
	if (front == back) 
	{
		if (cir_cue[0] == 0) { return true; }
		else return false;
	} 
	else return false;
}

bool is_full() 
{
	if (front == back) 
	{
		if (cir_cue[0] != 0) { return true; }
		else return false;
	}
	else return false;
}

void in(int a) 
{
	if (is_full()) { printf("원형큐 오버플로우\n"); }
	else 
	{
		cir_cue[back] = a;
		back++;
		back = back%5;
		printf("%d in\n", a);
	}
}

int out() 
{
	if (is_empty()) { printf("원형큐 언더플로우\n"); }
	else 
	{
		int temp = cir_cue[front];
		cir_cue[front] = 0;
		front++;
		front = front % 5;
		printf("%d out\n", temp);
		return temp;
	}
}

void print_cirque()
{
	for (int i = 0; i < 5; i++)
	{
		printf("%d\n", cir_cue[i]);
	}
	printf("\n");
}