#pragma once
#include "K_Std.h"

class K_DxState
{
public:
	static ID3D11SamplerState* g_pDefaultSS;

public:
	static bool SetState(ID3D11Device* pd3dDevice);
	static bool Release();
};

