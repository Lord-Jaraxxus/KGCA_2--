#include <stdio.h>

void pointer_clear(int* ip) 
{
	ip = nullptr;
}

void pointer_clear2(int* & ip)
{
	ip = nullptr;
}

int main() 
{
	int* a = new int;
	*a = 3;

	pointer_clear(a); // 
	pointer_clear2(a);

}