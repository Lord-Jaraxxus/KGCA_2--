#pragma once
#include "K_Std.h"

class K_DxState
{
public:
	static ID3D11SamplerState* g_pCurrentSS;
	static ID3D11SamplerState* g_pDefaultSS;
	static ID3D11SamplerState* g_pDefaultSSWrap;
	static ID3D11SamplerState* g_pDefaultSSMirror;

	static ID3D11RasterizerState* g_pCurrentRS;
	static ID3D11RasterizerState* g_pDefaultRSWireFrame;
	static ID3D11RasterizerState* g_pDefaultRSSolid;

	static ID3D11BlendState* g_pCurrentBS;
	static ID3D11BlendState* g_pAlphaBlend;
	static ID3D11BlendState* g_pAlphaBlendDisable;

	static ID3D11DepthStencilState* g_pCurrentDSS;
	static ID3D11DepthStencilState* g_pDefaultDepthStencil;
	static ID3D11DepthStencilState* g_pGreaterDepthStencil;
	static ID3D11DepthStencilState* g_pDisableDepthStencil;

public:
	static bool SetState(ID3D11Device* pd3dDevice);
	static bool Release();
};

