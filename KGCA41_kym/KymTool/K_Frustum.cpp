#include "pch.h"

#include "K_Frustum.h"

void K_Frustum::CreateFrustum(K_Matrix* matView, K_Matrix* matProj)
{
	// 받아온 뷰행렬과 투영행렬을 TMatrix로다가 바꿔삐고
	TBASIS_EX::TMatrix view = *(TBASIS_EX::TMatrix*)matView;
	TBASIS_EX::TMatrix proj = *(TBASIS_EX::TMatrix*)matProj;

	// 뷰*투영 곱해삐고 전치해서 역행렬로 바꿔줌.. 근데 이래도 되나? 되겠지 뭐 결합법칙은 성립하니까?
	TBASIS_EX::TMatrix matInvViewProj = view * proj;
	TBASIS_EX::D3DXMatrixInverse(&matInvViewProj, NULL, &matInvViewProj);

	// 프로스텀에 월드*뷰*투영행렬 다 계산된 결과 -> 밑에 요놈들 ㅇㅇ 
	// 즉, 저따가 투영행렬이랑 뷰행렬의 역행렬을 곱해서 월드상태로 만듬
	// m_vFrustum = v * world * (view * proj);
	// m_vFrustum = v * world * view*invView * proj * invproj;
	// m_vFrustum /= v.w     x,y= -1~+1,   z= 0~1;
	m_vFrustum[0] = TBASIS_EX::TVector3(-1.0f, -1.0f, 0.0f);
	m_vFrustum[1] = TBASIS_EX::TVector3(-1.0f, 1.0f, 0.0f);
	m_vFrustum[2] = TBASIS_EX::TVector3(1.0f, 1.0f, 0.0f);
	m_vFrustum[3] = TBASIS_EX::TVector3(1.0f, -1.0f, 0.0f);
	m_vFrustum[4] = TBASIS_EX::TVector3(-1.0f, -1.0f, 1.0f);
	m_vFrustum[5] = TBASIS_EX::TVector3(-1.0f, 1.0f, 1.0f);
	m_vFrustum[6] = TBASIS_EX::TVector3(1.0f, 1.0f, 1.0f);
	m_vFrustum[7] = TBASIS_EX::TVector3(1.0f, -1.0f, 1.0f);

	for (int iVer = 0; iVer < 8; iVer++)
	{
		//m_vFrustum[iVer] = m_vFrustum[iVer] * matInvViewProj;
		TBASIS_EX::D3DXVec3TransformCoord(&m_vFrustum[iVer], &m_vFrustum[iVer], &matInvViewProj);
		// 왜 밑에걸 썼는지는 잘 모르겠는데 위에걸로 하면 계산이 안되나봐? 아 연산자가 없네 ㅋㅋ
	}

	//   5    6  
	// 1    2
	// ->   <-
	//   4    7
	// 0    3 

	// 반시계로 돌려서 안쪽을 향하게 만듬, 이러면 이따 d값으로 비교할때 0보다.. 커야? 안쪽인건가?
	m_Plane[0].Create(*((k_Vector*)&m_vFrustum[1]),
		*((k_Vector*)&m_vFrustum[5]),
		*((k_Vector*)&m_vFrustum[0])); // left
	m_Plane[1].Create(*((k_Vector*)&m_vFrustum[3]), // right
		*((k_Vector*)&m_vFrustum[6]),
		*((k_Vector*)&m_vFrustum[2]));
	m_Plane[2].Create(*((k_Vector*)&m_vFrustum[5]), // top
		*((k_Vector*)&m_vFrustum[2]),
		*((k_Vector*)&m_vFrustum[6]));
	m_Plane[3].Create(*((k_Vector*)&m_vFrustum[0]), // bottom
		*((k_Vector*)&m_vFrustum[7]),
		*((k_Vector*)&m_vFrustum[3]));
	m_Plane[4].Create(*((k_Vector*)&m_vFrustum[0]), // near
		*((k_Vector*)&m_vFrustum[2]),
		*((k_Vector*)&m_vFrustum[1]));
	m_Plane[5].Create(*((k_Vector*)&m_vFrustum[5]), // far
		*((k_Vector*)&m_vFrustum[6]),
		*((k_Vector*)&m_vFrustum[4]));
}

bool K_Frustum::ClassifyPoint(k_Vector v)
{
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		float fDistance =
			m_Plane[iPlane].a * v.x +
			m_Plane[iPlane].b * v.y +
			m_Plane[iPlane].c * v.z +
			m_Plane[iPlane].d;
		if (fDistance < 0) return false; // 위에서 평면의 방향이 안쪽이니까 양수가 나와야 세이프, 음수가 나오면 아웃
	}
	return true;
}

K_POSITION K_Frustum::ClassifySphere(K_Sphere s)
{
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		float fDistance =
			m_Plane[iPlane].a * s.vCenter.x +
			m_Plane[iPlane].b * s.vCenter.y +
			m_Plane[iPlane].c * s.vCenter.z +
			m_Plane[iPlane].d;

		if (fDistance < 0) 
		{
			if (s.fRadius < fDistance * -1.0f) return K_SPANNING;
			else return K_BACK;
		}	
	}
	return K_FRONT;
}

K_POSITION K_Frustum::ClassifyBox(K_BOX box)
{
	float		fPlaneToCenter = 0.0;
	float		fDistance = 0.0f;
	k_Vector vDir;
	K_POSITION  Position;

	Position = K_FRONT;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = box.vAxis[0] * box.fExtent[0];
		fDistance = fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b * vDir.y + m_Plane[iPlane].c * vDir.z);
		vDir = box.vAxis[1] * box.fExtent[1];
		fDistance += fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b * vDir.y + m_Plane[iPlane].c * vDir.z);
		vDir = box.vAxis[2] * box.fExtent[2];
		fDistance += fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b * vDir.y + m_Plane[iPlane].c * vDir.z);

		fPlaneToCenter = m_Plane[iPlane].a * box.vCenter.x + m_Plane[iPlane].b * box.vCenter.y +
			m_Plane[iPlane].c * box.vCenter.z + m_Plane[iPlane].d;

		if (fPlaneToCenter > 0)
		{
			if (fPlaneToCenter < fDistance)
			{
				Position = K_SPANNING;
				break;
			}
		}
		else
			if (fPlaneToCenter < 0)
			{
				Position = K_BACK;
				if (fPlaneToCenter > -fDistance)
				{
					Position = K_SPANNING;
				}
				break;
			}
	}
	return Position;
}
