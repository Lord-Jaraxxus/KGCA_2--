#include "Sample.h"

bool Sample::Init() 
{
	K_Device::Init();
	return true;
}

bool Sample::Frame()
{
	K_Device::Frame();
	return true;
}

bool Sample::Render()
{
	K_Device::Render();
	return true;
}

bool Sample::Release()
{
	K_Device::Release();
	return true;
}

bool Sample::Run()
{
	Init();
	bool bGameRun = true;
	while (bGameRun)
	{
		if (K_Window::Run() == true)
		{
			Frame();
			Render();
		}
		else
		{
			bGameRun = false;
		}
	}

	Release();
	return true;
}


GAME_RUN(KymGameCoreSample1, 800, 600)