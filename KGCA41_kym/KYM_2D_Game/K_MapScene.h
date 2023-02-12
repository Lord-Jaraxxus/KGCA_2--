#pragma once
#include "K_Scene.h"

class K_MapScene : public K_Scene
{
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;

public:
	std::vector<K_2dObject*>	m_pBackGroundList;
	std::vector<K_MapObject*>	m_pMapObjectList;
	std::vector<K_MapObject*>	m_pCircleList;
	std::vector<K_Texture*>		m_pCircleTextureList;
	K_Button*	m_pDeckViewButton;
	k_Vector2D	m_vCameraPos;
	k_Vector2D  m_vMousePos;
	bool		m_bLButtonState = false;
	bool		m_bSceneStart = true;
	int			m_iPlayerMaxHP;
	int			m_iPlayerCurrentHP;
	int			m_iStage = 0;

public:
	K_Sound*	m_pBGM;
	K_Sound*	m_pSelectSound;
	K_Sound*	m_pViewDeckSound;
};

