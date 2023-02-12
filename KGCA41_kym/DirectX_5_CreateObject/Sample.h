#pragma once
#include "K_Device.h"
#include "K_BaseObject.h"

class Sample : public K_Device
{
	std::vector<K_BaseObject*> m_pObjectList;
	int m_iTexIndex = 1;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
}; 

