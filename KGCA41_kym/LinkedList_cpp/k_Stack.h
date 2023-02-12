#include "LinkedList.h"

class k_Stack : public LinkedList
{
public:
	void push(int a);
	int pop();
	k_Student* top();
	void pop_all();

public:
	k_Stack();
	~k_Stack();
};

