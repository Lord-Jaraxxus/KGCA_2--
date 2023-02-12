#pragma once
#include <iostream>
#include <math.h>

#define k_PI 3.141592f						  // 파이
#define DegreeToRadian(x) (x * (k_PI/180.0f)) // 디그리를 라디안으로
#define RadianToDegree(x) (x * (180.0f/k_PI)) // 라디안을 디그리로
#define k_Epsilon 0.001f						  // 허용오차	

class k_Vector2D
{
public:
	float x;
	float y;

public:
	k_Vector2D();
	k_Vector2D(float x, float y);
	//k_Vector2D(k_Vector2D& v); // 복사 생성자, 왜 만들었는지는 아직 몰?루겠으니 일단 주석처리
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
	float		LengthSquared();	// 크기에 루트를 씌우지 않은 값을 반환
	float		Length();			// 크기 반환 (직각삼각형의 빗변의 길이)
	void		Normalize();		// 정규화 (스스로를)
	k_Vector2D	Identity();			// 스스로를 정규화한 벡터(단위벡터)를 반환 (지는 안바뀜)
	//float		Angle(k_Vector2D);	// 두 벡터를 내적해서 사이각인 세타를 반환, 일단 오늘은 안쓸듯하니 주석
	
};

