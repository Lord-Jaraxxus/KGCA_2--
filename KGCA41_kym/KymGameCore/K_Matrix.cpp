#include "K_Matrix.h"

// 생성자, 단위행렬로 초기화 해줌
K_Matrix::K_Matrix()
{
	Identity();
}

// 스스로를 단위행렬로 바꾸는 함수
void K_Matrix::Identity()
{
	_12 = _13 = _14 = 0.0f;
	_21 = _23 = _24 = 0.0f;
	_31 = _32 = _34 = 0.0f;
	_41 = _42 = _43 = 0.0f;
	_11 = _22 = _33 = _44 = 1.0f;
}

// 스스로의 전치행렬을 반환하는 함수
K_Matrix K_Matrix::Transpose()
{
	K_Matrix transposed;

	transposed._11 = _11; transposed._12 = _21; transposed._13 = _31; transposed._14 = _41;
	transposed._21 = _12; transposed._22 = _22; transposed._23 = _32; transposed._24 = _42;
	transposed._31 = _13; transposed._32 = _23; transposed._33 = _33; transposed._34 = _43;
	transposed._41 = _14; transposed._42 = _24; transposed._43 = _34; transposed._44 = _44;

	return transposed;
}

// 단위행렬을 X축에 대해 fRadian만큼 회전시킨 행렬을 반환하는 함수
K_Matrix K_Matrix::RotationX(float fRadian)
{
	float fCosTheta = cos(fRadian);
	float fSinTheta = sin(fRadian);

	K_Matrix result;
	result._22 = fCosTheta;		result._23 = fSinTheta;
	result._32 = -fSinTheta;	result._33 = fCosTheta;

	return result;
}

// 단위행렬을 Y축에 대해 fRadian만큼 회전시킨 행렬을 반환하는 함수
K_Matrix K_Matrix::RotationY(float fRadian)
{
	float fCosTheta = cos(fRadian);
	float fSinTheta = sin(fRadian);

	K_Matrix result;
	result._11 = fCosTheta;		result._13 = -fSinTheta;
	result._31 = fSinTheta;		result._33 = fCosTheta;

	return result;
}

// 단위행렬을 Z축에 대해 fRadian만큼 회전시킨 행렬을 반환하는 함수
K_Matrix K_Matrix::RotationZ(float fRadian)
{
	float fCosTheta = cos(fRadian);
	float fSinTheta = sin(fRadian);

	K_Matrix result;
	result._11 = fCosTheta;		result._12 = fSinTheta;
	result._21 = -fSinTheta;	result._22 = fCosTheta;

	return result;
}

// 단위행렬을 매개변수만큼 확대/축소한 행렬을 반환하는 함수. xyz 각각 따로 배율 지정가능
K_Matrix K_Matrix::Scale(float x, float y, float z)
{
	K_Matrix result;

	result._11 = x;
	result._22 = y;
	result._33 = z;

	return result;
}

// 단위행렬을 매개변수만큼 이동한 행렬을 반환하는 함수
K_Matrix K_Matrix::Translation(float x, float y, float z)
{
	K_Matrix result;

	result._41 = x;
	result._42 = y;
	result._43 = z;

	return result;
}

// 뷰 행렬을 반환하는 함수(근데 지 스스로도 뷰행렬이 됨), 외적 사용
K_Matrix K_Matrix::ViewLookAt(k_Vector& vPosition, k_Vector& vTarget, k_Vector& vUp)
{
	K_Matrix matrix;
	k_Vector vDirection = (vTarget - vPosition).Normal(); // z축 벡터, 카메라의 방향이 되겄다
	k_Vector vRightVector = (vUp ^ vDirection).Normal(); // x축 벡터, 임시 설정한 y축인 업벡터랑 외적해서 구함 
	k_Vector vUpVector = (vDirection ^ vRightVector).Normal(); // 진짜 y축 벡터
	// 밑에 두개 정규화 안한 이유는 싹다 단위벡터라서 그런듯? 
	// 정규화된 벡터 두개 외적해서 나온 결과도 정규화된 벡터인갑지 뭐
	// 응 아니었어~ 이거때문에 야랄났었어~

	// 사실 카메라가 움직이는 게 아니라 오브젝트가 반대로 움직이는 것이기 때문에, 오브젝트 변환 행렬의 역행렬이 뷰 행렬이 된다
	// 근데 얘는 직교행렬이라서 전치행렬 == 역행렬이니까 그냥 전치행렬임
	_11 = vRightVector.x;	_12 = vUpVector.x;	_13 = vDirection.x;
	_21 = vRightVector.y;	_22 = vUpVector.y;	_23 = vDirection.y;
	_31 = vRightVector.z;	_32 = vUpVector.z;	_33 = vDirection.z;

	// 이쪽은 이동파트, 위에서 통으로 전치행렬 해버리면 4행에 있어야 할 놈들이 4열로 가버리니까 그냥 이렇게 해준다
	_41 = -(vPosition.x * _11 + vPosition.y * _21 + vPosition.z * _31);
	_42 = -(vPosition.x * _12 + vPosition.y * _22 + vPosition.z * _32);
	_43 = -(vPosition.x * _13 + vPosition.y * _23 + vPosition.z * _33);

	memcpy(&matrix, this, 16 * sizeof(float)); // 그래서 얜 뭐지? 아 위에서 matrix에 넣어준게 아니라서 이걸로 this걸 넣어준건가

	return matrix;
}

// 스스로를 오브젝트 변환 행렬로 만드는 함수, 얘는 내적 사용 
// (사실 ViewLookAt도 내적 써도 되고 얘도 외적 써도 됨, 그냥 다양한 방법 써볼라고 일케만드신듯)
void K_Matrix::ObjectLookAt(k_Vector& vPosition, k_Vector& vTarget, k_Vector& vUp)
{
	k_Vector vDirection = vTarget - vPosition;
	vDirection = vDirection.Normal(); // z축 벡터, 카메라의 방향이 되겄다

	// 그랑슈메트 알고리즘? 암튼 뭐시기를 이용해서 진짜 y축벡터를 구해줌
	float fDot = vUp | vDirection;
	k_Vector vC = vDirection * fDot;
	k_Vector vD = (vDirection * fDot);
	k_Vector vUpVector = vUp - vD; 
	vUpVector = vUpVector.Normal(); // 진짜 y축 벡터, 정규화도 해줌

	k_Vector vRightVector = vUpVector ^ vDirection;

	// 회전 파트, 이건 뷰행렬이 아니니까 그냥 고스란히 넣어줌
	_11 = vRightVector.x;	_12 = vRightVector.y;	_13 = vRightVector.z;
	_21 = vUpVector.x;		_22 = vUpVector.y;		_23 = vUpVector.z;
	_31 = vDirection.x;		_32 = vDirection.y;		_33 = vDirection.z;

	// 이동 파트
	_41 = vPosition.x;
	_42 = vPosition.y;
	_43 = vPosition.z;
}

// 스스로를 투영 행렬로 만들어 반환하는 함수, 직교 투영
K_Matrix K_Matrix::OrthoLH(float w, float h, float n, float f)
{
	Identity();
	_11 = 2.0f / w;
	_22 = 2.0f / h;
	_33 = 1.0f / (f - n);
	_43 = -n / (f - n);
	return *this;
}

// 스스로를 투영 행렬로 만들어 반환하는 함수, 직교 투영 + 영역 지정
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

// 스스로를 투영 행렬로 만들어 반환하는 함수, 원근 투영
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

// 곱셈 연산자 재정의, 뒤에 오는 행렬과 곱연산한 결과행렬을 반환
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
