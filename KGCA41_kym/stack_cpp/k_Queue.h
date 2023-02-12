#include "LinkedList.h"

class k_Queue : public LinkedList
{
public:
	void in(int a);
	int out();
	k_Student* front();
	k_Student* back();
	void out_all();

public:
	k_Queue();
	~k_Queue();
};



