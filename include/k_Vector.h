#pragma once
#include <windows.h>
#include <iostream>
#include <math.h>

#define k_PI 3.141592f						  // 파이
#define DegreeToRadian(x) (x * (k_PI/180.0f)) // 디그리를 라디안으로
#define RadianToDegree(x) (x * (180.0f/k_PI)) // 라디안을 디그리로
#define k_Epsilon 0.001f						  // 허용오차	

class K_Matrix; // 전방선언, 아마 행렬 클래스랑 include 물빨하면 지랄나니까 일케 한듯?

class k_Vector
{
public:
	float x;
	float y;
	float z;

public:
	k_Vector();
	k_Vector(float x, float y, float z);
	//k_Vector(k_Vector& v); // 복사 생성자, 왜 만들었는지는 아직 몰?루겠으니 일단 주석처리
	void Set(float x, float y, float z);

public:
	k_Vector	operator + (k_Vector& v);
	k_Vector	operator += (k_Vector& v);
	k_Vector	operator - (k_Vector& v);
	k_Vector	operator * (float scala);
	k_Vector	operator * (K_Matrix mat); // 곱셈 연산자 재정의, 행렬용
	k_Vector	operator / (float scala);
	k_Vector	operator *= (float scala); 
	bool		operator == (k_Vector& v);
	bool		operator <= (k_Vector& v);
	bool		operator >= (k_Vector& v);
	// Dot Prouduct, 내적
	float		operator | (k_Vector& const v0);
	// Cross Product, 외적
	k_Vector	operator ^ (k_Vector& const v0);

public:
	float		LengthSquared();	// 크기에 루트를 씌우지 않은 값을 반환
	float		Length();			// 크기 반환 (직각삼각형의 빗변의 길이)
	void		Normalize();		// 정규화 (스스로를)
	k_Vector	Normal();			// 스스로를 정규화한 벡터(단위벡터)를 반환 (지는 안바뀜)
	//float		Angle(k_Vector);	// 두 벡터를 내적해서 사이각인 세타를 반환, 일단 오늘은 안쓸듯하니 주석
	
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
	//k_Vector(k_Vector& v); // 복사 생성자, 왜 만들었는지는 아직 몰?루겠으니 일단 주석처리
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
	float		LengthSquared();	// 크기에 루트를 씌우지 않은 값을 반환
	float		Length();			// 크기 반환 (직각삼각형의 빗변의 길이)
	void		Normalize();		// 정규화 (스스로를)
	k_Vector4D	Identity();			// 스스로를 정규화한 벡터(단위벡터)를 반환 (지는 안바뀜)
	//float		Angle(k_Vector);	// 두 벡터를 내적해서 사이각인 세타를 반환, 일단 오늘은 안쓸듯하니 주석

};