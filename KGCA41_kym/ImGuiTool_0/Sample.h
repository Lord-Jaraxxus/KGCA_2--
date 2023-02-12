#pragma once
#pragma comment(lib, "KymGameCore_d.lib")

#include "K_GameCore.h"
#include "K_2dObject.h"

class Sample : public K_GameCore
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	std::vector<K_BaseObject*> m_pRectList;

public:
	bool CreateNewRect();
}; 

