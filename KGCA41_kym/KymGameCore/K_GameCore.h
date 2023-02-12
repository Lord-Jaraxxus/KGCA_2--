#pragma once
#include "K_Device.h"
#include "K_Timer.h"
#include "K_Input.h"
#include "K_Write.h"
#include "K_BaseObject.h"
#include "K_DxState.h"
#include "K_RenderTarget.h"

class K_GameCore : public K_Device
{
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

	bool Run();
	bool ToolRun();

//private: -> MFC들어가면서 선생님이 귀찮아서 그냥 퍼블릭으로 풀어버리심, 그냥 ToolInit이카면서 새로만들어도 됨
public:
	 bool K_GameCoreInit();
	 bool K_GameCoreFrame();
	 bool K_GameCorePreRender();
	 bool K_GameCoreRender();
	 bool K_GameCorePostRender();
	 bool K_GameCoreRelease();

public:
	virtual HRESULT CreateDXResource();
	virtual HRESULT DeleteDXResource();

public:
	bool m_bGameRun = true;

public:
	K_BaseObject	m_BG;
	K_RenderTarget	m_RT;
};

