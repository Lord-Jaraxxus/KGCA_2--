#pragma once
#include "K_Window.h"

class Sample : public K_Window
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

