#pragma once
#include "K_Scene.h"

class K_TitleScene : public K_Scene
{
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;

public:
	std::vector<K_2dObject*> m_pBackGroundList;
	K_Button*	m_pStartButton;
	K_Button*	m_pSettingButton;
	K_Button*	m_pEndButton;

	K_Sound*	m_pBGM;
};

