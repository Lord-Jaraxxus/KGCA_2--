#pragma once
#include "K_FBXLoader.h"

struct K_ActionTable 
{
	std::wstring wTableName;

	UINT iStartFrame;
	UINT iEndFrame;
	float fTickPerFrame; // 160
	float fFrameSpeed; // 30
	bool  bLoop;
	K_ActionTable()
	{
		bLoop = true;
	}
};

class K_Character
{
public:
	K_Matrix	m_matWorld;
	K_Matrix	m_matView;
	K_Matrix	m_matProj;

public:
	K_FBXLoader* m_pFbxFile;
	K_FBXLoader* m_pCurrentActionFile = nullptr; 
	std::map<std::wstring, K_FBXLoader*> m_ActionFileList;

public:
	K_ActionTable m_ActionCurrent;
	std::map<std::wstring, K_ActionTable> m_ActionTableList;
	K_AnimeScene m_AnimeScene;
	K_AnimeCurrentInfo m_AnimeCurrentInfo;
	float	m_fAnimeCurrentFrame;
	float	m_fAnimeSpeed = 1.0f;
	float	m_fAnimeInverse = 1.0f;

public:
	bool UpdateFrame(ID3D11DeviceContext* pContext);
	void SetMatrix(K_Matrix* matWorld, K_Matrix* matView, K_Matrix* matProj);
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
};

