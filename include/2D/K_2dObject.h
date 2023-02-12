#pragma once
#include "K_BaseObject.h"

class K_2dObject : public K_BaseObject
{
public:
	K_Texture*	m_pMaskTex = nullptr;
	POINT		m_ptImageSize;
	k_Vector4D	m_rtUV;
	k_Vector4D	m_rtInit;
	k_Vector2D	m_vPos;
	k_Vector2D	m_vSize;
	k_Vector4D  m_rtCollision;
	k_Vector2D	m_vCameraPos = {0,0};

//private:
public:
	k_Vector2D	m_vDrawPos;
	k_Vector2D	m_vDrawSize;

public:
	virtual void	SetRect(k_Vector4D rt);
	virtual void	SetPosition(k_Vector2D pos);
	virtual void	SetDrawSize(k_Vector2D size);
	virtual void	UpdateVertexBuffer();
	virtual void	SetMask(K_Texture* pMaskTex);
	virtual bool	Frame() override;
	virtual bool	Render() override;
	virtual void	SetCamera(k_Vector2D vCameraPos);
};
