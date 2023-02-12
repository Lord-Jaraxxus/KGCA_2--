#pragma once
#pragma comment(lib, "KymGameCore_d.lib")

#include "K_GameCore.h"
#include "K_DebugCamera.h"
#include "K_FBXLoader.h"

class Sample : public K_GameCore
{
public:
	std::vector<K_FBXLoader*>		m_FBXList;
	K_DebugCamera*	m_pDebugCamera = nullptr;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
}; 

