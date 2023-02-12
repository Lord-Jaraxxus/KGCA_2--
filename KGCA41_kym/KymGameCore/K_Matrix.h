#pragma once
#include "k_Vector.h"

struct float4 
{
	union 
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float matrix[4][4];
	};
};

class K_Matrix : public float4
{
public:
	K_Matrix(); // �� ������
	void Identity(); // �����θ� ������ķ� �ٲٴ� �Լ�
	K_Matrix Transpose() ; // �������� ��ġ����� ��ȯ�ϴ� �Լ�
	K_Matrix RotationX(float fRadian) ;
	K_Matrix RotationY(float fRadian) ;
	K_Matrix RotationZ(float fRadian) ; // ��������� ���� X, Y, Z�࿡ ���� fRadian��ŭ ȸ����Ų ����� ��ȯ�ϴ� �Լ�
	K_Matrix Scale(float x, float y, float z); // ��������� Ȯ��/����� ����� ��ȯ�ϴ� �Լ�. xyz ���� ���� ���� �������� 
	K_Matrix Translation(float x, float y, float z); // �̵�. �굵 xyz ���� ���� ��ŭ �������� ���ð���

public:
	K_Matrix ViewLookAt(k_Vector& vPosition, k_Vector& vTarget, k_Vector& vUp); // �� ����� ��ȯ�ϴ� �Լ�
	void ObjectLookAt(k_Vector& vPosition, k_Vector& vTarget, k_Vector& vUp); // �����θ� ������Ʈ ��ȯ ��ķ� ����� �Լ�
	K_Matrix OrthoLH(float w, float h, float n, float f); // �����θ� ���� ��ķ� ����� ��ȯ�ϴ� �Լ�, ���� ����
	K_Matrix OrthoOffCenterLH(float l, float r, float b, float t, float n, float f); // �����θ� ���� ��ķ� ����� ��ȯ�ϴ� �Լ�, ���� ���� + ���� ����
	K_Matrix PerspectiveFovLH(float fNearPlane, float fFarPlane, float fovY, float Aspect); // �����θ� ���� ��ķ� ����� ��ȯ�ϴ� �Լ�, ���� ����

public:
	K_Matrix operator * (K_Matrix &back_matrix); // ���� ������ ������
};

