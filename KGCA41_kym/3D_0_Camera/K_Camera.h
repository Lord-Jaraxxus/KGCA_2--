#pragma once
#include "K_BaseObject.h"

class K_Camera : public K_BaseObject
{
public:
	k_Vector		m_vCameraPos = { 0,0,0 };
	k_Vector		m_vLook; // z axis, axis°¡ ¹¹¿©?
	k_Vector		m_vUp = { 0,1,0 }; // y
	k_Vector		m_vRight; // x

	float     m_fYaw = 0.0f;
	float     m_fPitch = 0.0f;
	float     m_fRoll = 0.0f;
	float     m_fDistance = 0.0f;
	float     m_fSpeed = 10.0f;

	k_Vector		m_vTarget = { 0,0,0 };
	float			m_fNear;
	float			m_fFar;
	float			m_fFovY;
	float			m_fAspectRatio;

public:
	virtual void  CreateViewMatrix(k_Vector vEye, k_Vector vAt, k_Vector vUp);
	virtual void  CreateProjMatrix(float fNear, float fFar, float fFovY, float fAspectRatio);
	virtual bool  Frame();
	void		  Update();
};

