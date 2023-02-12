#pragma once
#include <windows.h>
#include <iostream>
#include <math.h>

#define k_PI 3.141592f						  // ����
#define DegreeToRadian(x) (x * (k_PI/180.0f)) // ��׸��� ��������
#define RadianToDegree(x) (x * (180.0f/k_PI)) // ������ ��׸���
#define k_Epsilon 0.001f						  // ������	

class K_Matrix; // ���漱��, �Ƹ� ��� Ŭ������ include �����ϸ� �������ϱ� ���� �ѵ�?

class k_Vector
{
public:
	float x;
	float y;
	float z;

public:
	k_Vector();
	k_Vector(float x, float y, float z);
	//k_Vector(k_Vector& v); // ���� ������, �� ����������� ���� ��?������� �ϴ� �ּ�ó��
	void Set(float x, float y, float z);

public:
	k_Vector	operator + (k_Vector& v);
	k_Vector	operator += (k_Vector& v);
	k_Vector	operator - (k_Vector& v);
	k_Vector	operator * (float scala);
	k_Vector	operator * (K_Matrix mat); // ���� ������ ������, ��Ŀ�
	k_Vector	operator / (float scala);
	k_Vector	operator *= (float scala); 
	bool		operator == (k_Vector& v);
	bool		operator <= (k_Vector& v);
	bool		operator >= (k_Vector& v);
	// Dot Prouduct, ����
	float		operator | (k_Vector& const v0);
	// Cross Product, ����
	k_Vector	operator ^ (k_Vector& const v0);

public:
	float		LengthSquared();	// ũ�⿡ ��Ʈ�� ������ ���� ���� ��ȯ
	float		Length();			// ũ�� ��ȯ (�����ﰢ���� ������ ����)
	void		Normalize();		// ����ȭ (�����θ�)
	k_Vector	Normal();			// �����θ� ����ȭ�� ����(��������)�� ��ȯ (���� �ȹٲ�)
	//float		Angle(k_Vector);	// �� ���͸� �����ؼ� ���̰��� ��Ÿ�� ��ȯ, �ϴ� ������ �Ⱦ����ϴ� �ּ�
	
};

class k_Vector4D
{
public:
	float x;
	float y;
	float z;
	float w;

public:
	k_Vector4D();
	k_Vector4D(float x, float y, float z, float w);
	//k_Vector(k_Vector& v); // ���� ������, �� ����������� ���� ��?������� �ϴ� �ּ�ó��
	void Set(float x, float y, float z, float w);

public:
	k_Vector4D	operator + (k_Vector4D& v);
	k_Vector4D	operator += (k_Vector4D& v);
	k_Vector4D	operator - (k_Vector4D& v);
	k_Vector4D	operator * (float scala);
	k_Vector4D	operator / (float scala);
	k_Vector4D	operator *= (float scala);
	bool		operator == (k_Vector4D& v);
	bool		operator <= (k_Vector4D& v);
	bool		operator >= (k_Vector4D& v);

public: 
	float		LengthSquared();	// ũ�⿡ ��Ʈ�� ������ ���� ���� ��ȯ
	float		Length();			// ũ�� ��ȯ (�����ﰢ���� ������ ����)
	void		Normalize();		// ����ȭ (�����θ�)
	k_Vector4D	Identity();			// �����θ� ����ȭ�� ����(��������)�� ��ȯ (���� �ȹٲ�)
	//float		Angle(k_Vector);	// �� ���͸� �����ؼ� ���̰��� ��Ÿ�� ��ȯ, �ϴ� ������ �Ⱦ����ϴ� �ּ�

};