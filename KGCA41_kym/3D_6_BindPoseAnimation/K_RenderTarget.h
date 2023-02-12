#pragma once
#include "K_BaseObject.h"

class K_RenderTarget
{
public:
	ID3D11Device*				m_pd3dDevice;
	ID3D11DeviceContext*		m_pImmediateContext;

	ID3D11RenderTargetView*		m_pRenderTargetView;
	ID3D11DepthStencilView*		m_pDepthStencilView;
	ID3D11ShaderResourceView*	m_pSRV;
	ID3D11ShaderResourceView*	m_pDSVSRV;

	ID3D11Texture2D*			m_pTexture;
	D3D11_DEPTH_STENCIL_DESC	m_DepthStencilDesc;
	D3D11_TEXTURE2D_DESC		m_TexDesc;
	D3D11_VIEWPORT				m_Viewport;

	ID3D11RenderTargetView*		m_pOldRTV;
	ID3D11DepthStencilView*		m_pOldDSV;
	D3D11_VIEWPORT				m_OldViewport; // 쌤은 배열로 하셨는데, 굳이? 나중에 필요해지면 바꾸지 뭐

public:
	HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, FLOAT fWidth = 1024.0f, FLOAT fHeight = 1024.0f);
	//bool Begin(ID3D11DeviceContext* pContext);
	//void End(ID3D11DeviceContext* pContext);
	bool Begin();
	void End();
	bool Release();
};

