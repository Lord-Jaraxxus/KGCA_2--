
class slave //부모 클래스, 
{
public:
	slave();
	virtual ~slave(); //이거쓰면 자식 생성자들 왜 야랄남?? 아 자식 소멸자를 cpp에 안써놔서 그렇구나 // 아님;
	//~slave(); //- 왜 이렇게 해놨을때는 -자식-ㄴㄴ 부모 소멸자를 선언 안해놨어도 잘 돌아갔을꼬? 근데 이젠 안되는데?

	virtual int GetMonthlySalary() = 0; // 순수 가상 함수

public:
	char* name;
	int position; // 1:사원 2:대리 3:과장 4:부장 5:이사
	int salary_type; // 1:시급 2:월급 3:연봉
	int note; // 비고란에 들어간 숫자
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