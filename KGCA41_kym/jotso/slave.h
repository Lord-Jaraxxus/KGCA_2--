
class slave //�θ� Ŭ����, 
{
public:
	slave();
	virtual ~slave(); //�̰ž��� �ڽ� �����ڵ� �� �߶���?? �� �ڽ� �Ҹ��ڸ� cpp�� �Ƚ���� �׷����� // �ƴ�;
	//~slave(); //- �� �̷��� �س������� -�ڽ�-���� �θ� �Ҹ��ڸ� ���� ���س�� �� ���ư�����? �ٵ� ���� �ȵǴµ�?

	virtual int GetMonthlySalary() = 0; // ���� ���� �Լ�

public:
	char* name;
	int position; // 1:��� 2:�븮 3:���� 4:���� 5:�̻�
	int salary_type; // 1:�ñ� 2:���� 3:����
	int note; // ������ �� ����
};


class daily_slave : public slave
{
public:
	daily_slave(char* name, int position, int salary_type, int note);
	//~daily_slave() 
	;

public:
	int GetMonthlySalary();
};

class monthly_slave : public slave
{
public:
	monthly_slave(char* name, int position, int salary_type, int note);
	//~monthly_slave();
public:
	int GetMonthlySalary();
};

class yearly_slave : public slave
{
public:
	yearly_slave(char* name, int position, int salary_type, int note);
	//~yearly_slave();
public:
	int GetMonthlySalary();
};