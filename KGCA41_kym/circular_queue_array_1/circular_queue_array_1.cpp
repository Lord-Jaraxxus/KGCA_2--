// 정석대로 원형큐 한자리 비우고 구현

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
	if (front == back){ return true; }
	else return false;
}

bool is_full()
{
	if ((back + 1) % 5 == front) { return true; }
	else return false;
}

void in(int a)
{
	if (is_full()) { printf("원형큐 오버플로우\n"); }
	else
	{
		cir_cue[back] = a;
		back++;
		back = back % 5;
		printf("%d in\n", a);
	}
}

int out()
{
	if (is_empty()) { printf("원형큐 언더플로우\n"); }
	else
	{
		int temp = cir_cue[front];
		//cir_cue[front] = 0;  - 한칸을 비워두면 이지랄 안해도댐.. 이것때문인가?
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