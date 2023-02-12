#pragma once
#include <iostream>
#include <math.h>

#define k_PI 3.141592f						  // ����
#define DegreeToRadian(x) (x * (k_PI/180.0f)) // ��׸��� ��������
#define RadianToDegree(x) (x * (180.0f/k_PI)) // ������ ��׸���
#define k_Epsilon 0.001f						  // ������	

class k_Vector2D
{
public:
	float x;
	float y;

public:
	k_Vector2D();
	k_Vector2D(float x, float y);
	//k_Vector2D(k_Vector2D& v); // ���� ������, �� ����������� ���� ��?������� �ϴ� �ּ�ó��
	void Set(float x, float y);

public:
	k_Vector2D	operator + (k_Vector2D& v);
	k_Vector2D	operator += (k_Vector2D& v);
	k_Vector2D	operator - (k_Vector2D& v);
	k_Vector2D	operator * (float scala);
	k_Vector2D	operator / (float scala);
	k_Vector2D operator *= (float scala); 
	bool		operator == (k_Vector2D& v);

public:
	float		LengthSquared();	// ũ�⿡ ��Ʈ�� ������ ���� ���� ��ȯ
	float		Length();			// ũ�� ��ȯ (�����ﰢ���� ������ ����)
	void		Normalize();		// ����ȭ (�����θ�)
	k_Vector2D	Identity();			// �����θ� ����ȭ�� ����(��������)�� ��ȯ (���� �ȹٲ�)
	//float		Angle(k_Vector2D);	// �� ���͸� �����ؼ� ���̰��� ��Ÿ�� ��ȯ, �ϴ� ������ �Ⱦ����ϴ� �ּ�
	
};

