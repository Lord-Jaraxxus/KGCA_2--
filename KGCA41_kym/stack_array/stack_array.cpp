#include <stdio.h>

int stack[5] = { 0, };  //이래도 되나? 
int top = 0;

void push(int a);
int pop();
bool is_empty();
void destroy();
void print_stack();

void main()
{
	push(1);
	push(2);
	push(3);
	push(4);
	push(5);
	push(6);
	print_stack();

	pop();
	pop();
	pop();
	pop();
	pop();
	pop();
	print_stack();

	push(6);
	print_stack();

	destroy();
	print_stack();
}

void push(int a)
{
	if (top >= 5) { printf("스택 오버플로우\n"); }
	else 
	{
		stack[top] = a;
		top++;
	}
	// stack[++top] = a; - 이러케도 가능은 한듯
}

int pop() 
{
	if (is_empty()) { printf("스택 언더플로우\n"); return 0; }
	top--;
	int temp = stack[top];
	stack[top] = 0;

	printf("%d가 튀어나옴\n", temp);
	return temp;
}

bool is_empty() 
{
	if (top <= 0) { return true; }
	else { return false; }
}

void destroy() 
{
	for (int i = 0; i < 10; i++) 
	{
		stack[i] = 0;
	}
	top = 0;
}

void print_stack() 
{
	for (int i = 0; i < 5; i++)
	{
		printf("%d\n", stack[4-i]); //stack답게 거꾸로 출력
	}
	printf("\n");
}