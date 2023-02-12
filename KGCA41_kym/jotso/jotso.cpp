#include <iostream>
#include "slave.h"

int main()
{
	slave* Slaves[] = { 
		new daily_slave(const_cast<char*>("권상우"), 1, 1, 200),
		new daily_slave(const_cast<char*>("조인성"), 1, 1, 220),
		new monthly_slave(const_cast<char*>("감우성"), 2, 2, 3),
		new monthly_slave(const_cast<char*>("강동원"), 2, 2, 5),
		new yearly_slave(const_cast<char*>("황정민"), 3, 3, 3),
		new monthly_slave(const_cast<char*>("박중훈"), 3, 2, 15),
		new monthly_slave(const_cast<char*>("최민식"), 4, 2, 20),
		new yearly_slave(const_cast<char*>("정진영"), 4, 3, 3),
		new daily_slave(const_cast<char*>("안성기"), 5, 1, 50),
		new yearly_slave(const_cast<char*>("송강호"), 5, 3, 4),
	};

	for (int i = 0; i < 10; i++) 
	{
		printf("%s\t", Slaves[i]->name);
		printf("%d\t", Slaves[i]->position);
		printf("%d\t", Slaves[i]->salary_type);
		printf("%d\t\n", Slaves[i]->GetMonthlySalary());
		delete Slaves[i];
		Slaves[i] = nullptr;
	}
}