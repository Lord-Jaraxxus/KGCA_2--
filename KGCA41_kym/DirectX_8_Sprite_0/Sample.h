#pragma once
#include "K_GameCore.h"

// ��� �� Ÿ�̸� Ŭ���� ����� ������ �߶���? ������ ����� �־������� �� ���ư��µ� 
// ����! �������� Ÿ�̸� Ŭ���� ����� �ƴ϶� Ÿ�̸� Ŭ���� cpp�� ��������! �� ���밡��!
//float g_fGameTimer = 0.0f;
//float g_fSecondPerFrame = 0.0f;

class K_2dObject : public K_BaseObject 
{
public:
	K_Texture*	m_pMaskTex = nullptr;
	POINT		m_ptImageSize;
	k_Vector4D	m_rtUV;
	k_Vector4D	m_rtInit;
	k_Vector2D	m_vPos;
	k_Vector2D	m_vSize;
private:
	k_Vector2D	m_vDrawPos;
	k_Vector2D	m_vDrawSize;

public:
	virtual void	SetPosition(k_Vector2D pos);
	virtual void	SetRect(k_Vector4D rt);
	virtual void	UpdateVertexBuffer();
	virtual void	SetMask(K_Texture* pMaskTex);
	virtual bool	Frame() override;
	virtual bool	Render() override;
};

class K_Npc : public K_2dObject 
{
	virtual bool	Frame() override;
};

class Sample : public K_GameCore
{
public:
	std::vector<K_BaseObject*> m_pObjectList;
	std::vector<K_BaseObject*> m_pNPCList;
	K_BaseObject*	m_pMap;
	K_2dObject*		m_pUser;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
}; 

