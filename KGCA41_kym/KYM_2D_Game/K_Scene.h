#pragma once
#include "K_GameCore.h"
#include "2dObjects.h"
#include "K_Deck.h"
#include "K_SoundManager.h"

class K_Scene 
{
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	std::vector<K_BaseObject*> m_pObjectList;
	std::vector<K_BaseObject*> m_pNPCList;
	K_BackGround* m_pMap;
	K_Player* m_pUser;

public:
	int	m_iSceneState = 0;

public:
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pImmediateContext;

public:
	virtual bool SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext);

};

