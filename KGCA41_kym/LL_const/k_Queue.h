#include "LinkedList.h"

class k_Queue : public LinkedList
{
public:
	void in(int a);
	int out();
	k_Student* front() const;
	k_Student* back() const;
	void out_all();

public:
	k_Queue();
	~k_Queue();
};



