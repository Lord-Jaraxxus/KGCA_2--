#include "K_Camera.h"
#include "K_Input.h"

void K_Camera::CreateViewMatrix(k_Vector vEye, k_Vector vAt, k_Vector vUp)
{
	m_vCameraPos = vEye;
	m_vTarget = vAt;
	m_vUp = vUp;
	m_matView.ViewLookAt(vEye, vAt, vUp);

	Update();
}

void K_Camera::CreateProjMatrix(float fNear, float fFar, float fFovY, float fAspectRatio)
{
	m_fNear = fNear;
	m_fFar = fFar;
	m_fFovY = fFovY;
	m_fAspectRatio = fAspectRatio;
	m_matProj.PerspectiveFovLH(m_fNear, m_fFar, m_fFovY, m_fAspectRatio);
	//m_matProj = m_matProj.OrthoLH(800, 600, 0.0f, 100.0f);
	//OrthoOffCenterLH(m_matProj ,-400, 400, -300, 300, 0.0f, 100.0f);

}

bool K_Camera::Frame()
{
	if (I_Input.GetKey('W') == KEY_HOLD)
	{
		k_Vector v = m_vLook * 10.0f * g_fSecondPerFrame;
		m_vCameraPos += v;
	}
	if (I_Input.GetKey('S') == KEY_HOLD)
	{
		k_Vector v = m_vLook * 10.0f * g_fSecondPerFrame;
		m_vCameraPos = m_vCameraPos - v;
	}
	if (I_Input.GetKey('A') == KEY_HOLD)
	{
		k_Vector v = m_vRight * 10.0f * g_fSecondPerFrame;
		m_vCameraPos = m_vCameraPos - v;
	}
	if (I_Input.GetKey('D') == KEY_HOLD)
	{
		k_Vector v = m_vRight * 10.0f * g_fSecondPerFrame;
		m_vCameraPos += v;
	}
	if (I_Input.GetKey('Q') == KEY_HOLD)
	{

		k_Vector v = m_vUp * 10.0f * g_fSecondPerFrame;
		m_vCameraPos += v;
	}
	if (I_Input.GetKey('E') == KEY_HOLD)
	{
		k_Vector v = m_vUp * 10.0f * g_fSecondPerFrame;
		m_vCameraPos = m_vCameraPos - v;
	}

	if (I_Input.GetKey(VK_LBUTTON) == KEY_HOLD)
	{
		m_fYaw += I_Input.m_ptOffset.x * 0.002f;
		m_fPitch += I_Input.m_ptOffset.y * 0.002f;

		K_Matrix rx;
		rx = rx * rx.RotationY(0.001f * I_Input.m_ptOffset.x);
		rx = rx * rx.RotationX(0.001f * I_Input.m_ptOffset.y);
		m_matView = m_matView * rx;
	}

	m_matView._41 = -(m_vCameraPos | m_vRight);
	m_matView._42 = -(m_vCameraPos | m_vUp);
	m_matView._43 = -(m_vCameraPos | m_vLook);
	//m_matView._41 = -(m_vCameraPos.x * m_matView._11 + m_vCameraPos.y * m_matView._21 + m_vCameraPos.z * m_matView._31);
	//m_matView._42 = -(m_vCameraPos.x * m_matView._12 + m_vCameraPos.y * m_matView._22 + m_vCameraPos.z * m_matView._32);
	//m_matView._43 = -(m_vCameraPos.x * m_matView._13 + m_vCameraPos.y * m_matView._23 + m_vCameraPos.z * m_matView._33);

	//k_Vector vUp = { 0,1,0 };

	//TMatrix mCamera = TMath::RotationY(g_fGameTimer);
	//vPos = (vPos + vPosMovement) * mCamera;

	//m_matView.ViewLookAt(m_vCameraPos, m_vTarget, vUp); // �̰� �ȵƴ� ���� : ViewLookAt �ȿ��� ���̶� ����Ʈ���͸� ����ȭ ���س��� ���� �ϴϱ� �ߵǳ�
	//m_matView.ViewLookAt(m_vCameraPos, m_vTarget, m_vUp); // ���� �̰� ���� ����? -> ������ ���� 90������ �����ȵǼ� �߶���, �ٵ� �� 010�̳� �Ʊ��������ͳ� ���� �� ���� �ִ°� �Ȱ����ϱ� �ӽ� �����ͷ� �ᵵ �������� -> ���Ƴ�~ �������� z��ȸ������~

	Update();

	return true;
}

void K_Camera::Update()
{
	m_vRight.x = m_matView._11;
	m_vRight.y = m_matView._21;
	m_vRight.z = m_matView._31;

	m_vUp.x = m_matView._12;
	m_vUp.y = m_matView._22;
	m_vUp.z = m_matView._32;

	m_vLook.x = m_matView._13;
	m_vLook.y = m_matView._23;
	m_vLook.z = m_matView._33;

	m_vRight.Normalize();
	m_vUp.Normalize();
	m_vLook.Normalize();
}
