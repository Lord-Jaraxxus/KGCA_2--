#pragma once
#include "K_Device.h"
#include "K_BaseObject.h"
#include"k_GameCore.h"

class Sample : public K_Device
{
	K_BaseObject m_object;
	k_GameCore2D m_2Dgamecore;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
}; 

