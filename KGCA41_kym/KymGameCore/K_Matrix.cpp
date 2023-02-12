#include "K_Matrix.h"

// ������, ������ķ� �ʱ�ȭ ����
K_Matrix::K_Matrix()
{
	Identity();
}

// �����θ� ������ķ� �ٲٴ� �Լ�
void K_Matrix::Identity()
{
	_12 = _13 = _14 = 0.0f;
	_21 = _23 = _24 = 0.0f;
	_31 = _32 = _34 = 0.0f;
	_41 = _42 = _43 = 0.0f;
	_11 = _22 = _33 = _44 = 1.0f;
}

// �������� ��ġ����� ��ȯ�ϴ� �Լ�
K_Matrix K_Matrix::Transpose()
{
	K_Matrix transposed;

	transposed._11 = _11; transposed._12 = _21; transposed._13 = _31; transposed._14 = _41;
	transposed._21 = _12; transposed._22 = _22; transposed._23 = _32; transposed._24 = _42;
	transposed._31 = _13; transposed._32 = _23; transposed._33 = _33; transposed._34 = _43;
	transposed._41 = _14; transposed._42 = _24; transposed._43 = _34; transposed._44 = _44;

	return transposed;
}

// ��������� X�࿡ ���� fRadian��ŭ ȸ����Ų ����� ��ȯ�ϴ� �Լ�
K_Matrix K_Matrix::RotationX(float fRadian)
{
	float fCosTheta = cos(fRadian);
	float fSinTheta = sin(fRadian);

	K_Matrix result;
	result._22 = fCosTheta;		result._23 = fSinTheta;
	result._32 = -fSinTheta;	result._33 = fCosTheta;

	return result;
}

// ��������� Y�࿡ ���� fRadian��ŭ ȸ����Ų ����� ��ȯ�ϴ� �Լ�
K_Matrix K_Matrix::RotationY(float fRadian)
{
	float fCosTheta = cos(fRadian);
	float fSinTheta = sin(fRadian);

	K_Matrix result;
	result._11 = fCosTheta;		result._13 = -fSinTheta;
	result._31 = fSinTheta;		result._33 = fCosTheta;

	return result;
}

// ��������� Z�࿡ ���� fRadian��ŭ ȸ����Ų ����� ��ȯ�ϴ� �Լ�
K_Matrix K_Matrix::RotationZ(float fRadian)
{
	float fCosTheta = cos(fRadian);
	float fSinTheta = sin(fRadian);

	K_Matrix result;
	result._11 = fCosTheta;		result._12 = fSinTheta;
	result._21 = -fSinTheta;	result._22 = fCosTheta;

	return result;
}

// ��������� �Ű�������ŭ Ȯ��/����� ����� ��ȯ�ϴ� �Լ�. xyz ���� ���� ���� ��������
K_Matrix K_Matrix::Scale(float x, float y, float z)
{
	K_Matrix result;

	result._11 = x;
	result._22 = y;
	result._33 = z;

	return result;
}

// ��������� �Ű�������ŭ �̵��� ����� ��ȯ�ϴ� �Լ�
K_Matrix K_Matrix::Translation(float x, float y, float z)
{
	K_Matrix result;

	result._41 = x;
	result._42 = y;
	result._43 = z;

	return result;
}

// �� ����� ��ȯ�ϴ� �Լ�(�ٵ� �� �����ε� ������� ��), ���� ���
K_Matrix K_Matrix::ViewLookAt(k_Vector& vPosition, k_Vector& vTarget, k_Vector& vUp)
{
	K_Matrix matrix;
	k_Vector vDirection = (vTarget - vPosition).Normal(); // z�� ����, ī�޶��� ������ �ǰδ�
	k_Vector vRightVector = (vUp ^ vDirection).Normal(); // x�� ����, �ӽ� ������ y���� �����Ͷ� �����ؼ� ���� 
	k_Vector vUpVector = (vDirection ^ vRightVector).Normal(); // ��¥ y�� ����
	// �ؿ� �ΰ� ����ȭ ���� ������ �ϴ� �������Ͷ� �׷���? 
	// ����ȭ�� ���� �ΰ� �����ؼ� ���� ����� ����ȭ�� �����ΰ��� ��
	// �� �ƴϾ���~ �̰Ŷ����� �߶�������~

	// ��� ī�޶� �����̴� �� �ƴ϶� ������Ʈ�� �ݴ�� �����̴� ���̱� ������, ������Ʈ ��ȯ ����� ������� �� ����� �ȴ�
	// �ٵ� ��� ��������̶� ��ġ��� == ������̴ϱ� �׳� ��ġ�����
	_11 = vRightVector.x;	_12 = vUpVector.x;	_13 = vDirection.x;
	_21 = vRightVector.y;	_22 = vUpVector.y;	_23 = vDirection.y;
	_31 = vRightVector.z;	_32 = vUpVector.z;	_33 = vDirection.z;

	// ������ �̵���Ʈ, ������ ������ ��ġ��� �ع����� 4�࿡ �־�� �� ����� 4���� �������ϱ� �׳� �̷��� ���ش�
	_41 = -(vPosition.x * _11 + vPosition.y * _21 + vPosition.z * _31);
	_42 = -(vPosition.x * _12 + vPosition.y * _22 + vPosition.z * _32);
	_43 = -(vPosition.x * _13 + vPosition.y * _23 + vPosition.z * _33);

	memcpy(&matrix, this, 16 * sizeof(float)); // �׷��� �� ����? �� ������ matrix�� �־��ذ� �ƴ϶� �̰ɷ� this�� �־��ذǰ�

	return matrix;
}

// �����θ� ������Ʈ ��ȯ ��ķ� ����� �Լ�, ��� ���� ��� 
// (��� ViewLookAt�� ���� �ᵵ �ǰ� �굵 ���� �ᵵ ��, �׳� �پ��� ��� �Ẽ��� ���ɸ���ŵ�)
void K_Matrix::ObjectLookAt(k_Vector& vPosition, k_Vector& vTarget, k_Vector& vUp)
{
	k_Vector vDirection = vTarget - vPosition;
	vDirection = vDirection.Normal(); // z�� ����, ī�޶��� ������ �ǰδ�

	// �׶�����Ʈ �˰���? ��ư ���ñ⸦ �̿��ؼ� ��¥ y�຤�͸� ������
	float fDot = vUp | vDirection;
	k_Vector vC = vDirection * fDot;
	k_Vector vD = (vDirection * fDot);
	k_Vector vUpVector = vUp - vD; 
	vUpVector = vUpVector.Normal(); // ��¥ y�� ����, ����ȭ�� ����

	k_Vector vRightVector = vUpVector ^ vDirection;

	// ȸ�� ��Ʈ, �̰� ������� �ƴϴϱ� �׳� ������ �־���
	_11 = vRightVector.x;	_12 = vRightVector.y;	_13 = vRightVector.z;
	_21 = vUpVector.x;		_22 = vUpVector.y;		_23 = vUpVector.z;
	_31 = vDirection.x;		_32 = vDirection.y;		_33 = vDirection.z;

	// �̵� ��Ʈ
	_41 = vPosition.x;
	_42 = vPosition.y;
	_43 = vPosition.z;
}

// �����θ� ���� ��ķ� ����� ��ȯ�ϴ� �Լ�, ���� ����
K_Matrix K_Matrix::OrthoLH(float w, float h, float n, float f)
{
	Identity();
	_11 = 2.0f / w;
	_22 = 2.0f / h;
	_33 = 1.0f / (f - n);
	_43 = -n / (f - n);
	return *this;
}

// �����θ� ���� ��ķ� ����� ��ȯ�ϴ� �Լ�, ���� ���� + ���� ����
K_Matrix K_Matrix::OrthoOffCenterLH(float l, float r, float b, float t, float n, float f)
{
	Identity();
	_11 = 2.0f / (r - l);
	_22 = 2.0f / (t - b);
	_33 = 1.0f / (f - n);
	_43 = -n / (f - n);
	_41 = (l + r) / (l - r);
	_42 = (t + b) / (b - t);
	return *this;
}

// �����θ� ���� ��ķ� ����� ��ȯ�ϴ� �Լ�, ���� ����
K_Matrix K_Matrix::PerspectiveFovLH(float fNearPlane, float fFarPlane, float fovY, float Aspect)
{
	float    h, w, Q;

	h = 1 / tan(fovY * 0.5f);  // 1/tans(x) = cot(x)
	w = h / Aspect;

	Q = fFarPlane / (fFarPlane - fNearPlane);

	K_Matrix ret;
	ZeroMemory(this, sizeof(K_Matrix));

	_11 = w;
	_22 = h;
	_33 = Q;
	_43 = -Q * fNearPlane;
	_34 = 1;
	_44 = 0.0f;

	memcpy((void*)&ret, this, 16 * sizeof(float));
	return ret;
}

// ���� ������ ������, �ڿ� ���� ��İ� �������� �������� ��ȯ
K_Matrix K_Matrix::operator*(K_Matrix& back_matrix)
{
	K_Matrix result;

	for (int iColumn = 0; iColumn < 4; iColumn++)
	{
		for (int iRow = 0; iRow < 4; iRow++)
		{
			result.matrix[iRow][iColumn] =
				matrix[iRow][0] * back_matrix.matrix[0][iColumn] +
				matrix[iRow][1] * back_matrix.matrix[1][iColumn] +
				matrix[iRow][2] * back_matrix.matrix[2][iColumn] +
				matrix[iRow][3] * back_matrix.matrix[3][iColumn];
		}
	}
	return result;
}
