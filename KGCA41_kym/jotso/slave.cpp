#include "slave.h"
#include <stdio.h> // printf

slave::slave()
{
	this->name = nullptr;
	this->position = 0;
	this->salary_type = 0;
	this->note = 0;
}
slave::~slave() // ���ξ� �̰ſ���!!!!!!!!!!! �̰� �����ٰ� ���ž�!!!!!!!!!!!!!
{
	printf("�θ� Ŭ���� �Ҹ�\n");
}


daily_slave::daily_slave(char* name, int position, int salary_type, int note) 
{
	this->name = name;
	this->position = position;
	this->salary_type = salary_type;
	this->note = note;
}
//daily_slave::~daily_slave()  // ��� ��� �ȵ��ư��� �ٵ� ��ü ����???? // ����
//{
//	printf("�ڽ�-�ñ� Ŭ���� �Ҹ�\n");
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
//	printf("�ڽ�-���� Ŭ���� �Ҹ�\n");
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
//	printf("�ڽ�-���� Ŭ���� �Ҹ�\n");
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