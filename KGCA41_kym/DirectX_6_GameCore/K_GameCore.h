#pragma once
#include "K_Device.h"
#include "K_Timer.h"
#include "K_Input.h"
#include "K_Write.h"

class K_GameCore : public K_Device
{
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

	bool Run();

private:
	 bool K_GameCoreInit();
	 bool K_GameCoreFrame();
	 bool K_GameCorePreRender();
	 bool K_GameCoreRender();
	 bool K_GameCorePostRender();
	 bool K_GameCoreRelease();

public:
	bool m_bGameRun = true;
	K_Write m_Write;
	IDXGISurface1* m_pBackBuffer;

};

