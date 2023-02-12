#include "slave.h"
#include <stdio.h> // printf

slave::slave()
{
	this->name = nullptr;
	this->position = 0;
	this->salary_type = 0;
	this->note = 0;
}
slave::~slave() // 영민아 이거였어!!!!!!!!!!! 이게 없었다고 병신아!!!!!!!!!!!!!
{
	printf("부모 클래스 소멸\n");
}


daily_slave::daily_slave(char* name, int position, int salary_type, int note) 
{
	this->name = name;
	this->position = position;
	this->salary_type = salary_type;
	this->note = note;
}
//daily_slave::~daily_slave()  // 요게 없어서 안돌아갔네 근데 대체 웨지???? // ㅋㅋ
//{
//	printf("자식-시급 클래스 소멸\n");
//}


monthly_slave::monthly_slave(char* name, int position, int salary_type, int note)
{
	this->name = name;
	this->position = position;
	this->salary_type = salary_type;
	this->note = note;
}
//monthly_slave::~monthly_slave()
//{
//	printf("자식-월급 클래스 소멸\n");
//}

yearly_slave::yearly_slave(char* name, int position, int salary_type, int note)
{
	this->name = name;
	this->position = position;
	this->salary_type = salary_type;
	this->note = note;
}
//yearly_slave::~yearly_slave()
//{
//	printf("자식-연봉 클래스 소멸\n");
//}


int daily_slave::GetMonthlySalary()
{
	int salary = this->note;
	for (int i = 1; i < this->position; i++) { salary = salary * 2; }

	return salary;
}

int monthly_slave::GetMonthlySalary()
{
	int salary = 200;
	salary += (this->position-1) * 20;
	salary += (this->note * 5);

	return salary;
}

int yearly_slave::GetMonthlySalary()
{
	int salary = 2000;
	salary += (this->position - 1) * 1000;
	salary += (this->note * 500);
	return (salary / 12);
}