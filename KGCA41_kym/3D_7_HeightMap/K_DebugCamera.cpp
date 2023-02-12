#include "K_DebugCamera.h"
#include "K_Input.h"
#include "TMath.h"

void K_DebugCamera::CreateViewMatrix(k_Vector vEye, k_Vector vAt, k_Vector vUp)
{
	m_vCameraPos = vEye;
	m_vTarget = vAt;
	m_vUp = vUp;
	m_matView.ViewLookAt(vEye, vAt, vUp);
}

void K_DebugCamera::CreateProjMatrix(float fNear, float fFar, float fFovY, float fAspectRatio)
{
	m_fNear = fNear;
	m_fFar = fFar;
	m_fFovY = fFovY;
	m_fAspectRatio = fAspectRatio;
	m_matProj.PerspectiveFovLH(m_fNear, m_fFar, m_fFovY, m_fAspectRatio);
}

bool K_DebugCamera::Frame()
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
	}

	m_matWorld._41 = m_vCameraPos.x;
	m_matWorld._42 = m_vCameraPos.y;
	m_matWorld._43 = m_vCameraPos.z;

	TBASIS_EX::TVector3 vPos;
	vPos.x = m_vCameraPos.x;
	vPos.y = m_vCameraPos.y;
	vPos.z = m_vCameraPos.z;

	//////////////////////////// DebugCamera ////////////////////////////
	TBASIS_EX::TMatrix matWorld;
	TBASIS_EX::TMatrix matView;
	TBASIS_EX::TMatrix matRotation;
	TBASIS_EX::TQuaternion m_qRotation;
	TBASIS_EX::D3DXQuaternionRotationYawPitchRoll(&m_qRotation, m_fYaw, m_fPitch, m_fRoll);
	TBASIS_EX::D3DXMatrixAffineTransformation(&matWorld, 1.0f, NULL, &m_qRotation, &vPos);
	TBASIS_EX::D3DXMatrixInverse(&matView, NULL, &matWorld);
	m_matView = *((K_Matrix*)&matView);

	//////////////////////////// Model View////////////////////////////
	/*TVector vLocalUp = { 0.0f, 1.0f, 0.0f };
	TVector vLocalLook = { 0.0f, 0.0f, 1.0f };

	TBASIS_EX::TMatrix matRotation;
	TBASIS_EX::D3DXMatrixRotationYawPitchRoll(
		&matRotation, m_fYaw, 0, m_fRoll);
	TMatrix* matR = (TMatrix*)&matRotation;

	TVector vWorldLook = vLocalLook * (*matR);
	TVector vWorldUp = vLocalUp * (*matR);
	TVector vWorld = vWorldLook * 10.0f;
	m_vPos = m_vTarget - vWorld;
	m_matView.ViewLookAt(m_vPos, m_vTarget, m_vUp); */

	Update();

	return true;
}