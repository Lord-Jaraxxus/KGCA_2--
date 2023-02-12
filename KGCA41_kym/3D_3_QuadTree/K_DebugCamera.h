#pragma once
#include "K_Camera.h"

class K_DebugCamera : public K_Camera
{
public:
	virtual void  CreateViewMatrix(k_Vector vEye, k_Vector vAt, k_Vector vUp);
	virtual void  CreateProjMatrix(float fNear, float fFar, float fFovY, float fAspectRatio);
	virtual bool  Frame();
};

