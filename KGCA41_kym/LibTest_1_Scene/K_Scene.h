#pragma once
#include "K_GameCore.h"
#include "K_2dObject.h"

class K_Map : public K_2dObject
{
public:
	virtual bool	Frame() override;
	virtual void	SetPosition(k_Vector2D pos) override;
	int				m_iStage = 0;
};

class K_Npc : public K_2dObject
{
	virtual bool	Frame() override;
};

class K_Player : public K_2dObject
{
	virtual bool	Frame() override;
};


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
	K_Map* m_pMap;
	K_Player* m_pUser;


public:
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pImmediateContext;

public:
	virtual bool SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext);

};

