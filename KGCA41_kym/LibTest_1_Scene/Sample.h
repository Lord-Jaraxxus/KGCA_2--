#pragma once
#pragma comment(lib, "KymGameCore_d.lib")

#include "K_Scene.h"

class Sample : public K_GameCore
{
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	K_Scene*	m_pScene;
}; 

