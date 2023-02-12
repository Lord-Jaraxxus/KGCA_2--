#pragma once
#include "K_Device.h"
#include "K_BaseObject.h"

class Sample : public K_Device
{
	K_BaseObject m_object;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
}; 

