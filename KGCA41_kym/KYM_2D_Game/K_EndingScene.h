#pragma once
#include "K_Scene.h"

class K_EndingScene : public K_Scene
{
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;

public:
	K_2dObject* m_pBackGround;
	K_Sound* m_pBGM;
};


