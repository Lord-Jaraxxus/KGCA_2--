#pragma once
#include "K_Window.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

class K_Device : public K_Window
{
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
public:
	bool PreRender();
	bool PostRender();

public:
	// �������̽� -> �ϵ���� ���� ����
	ID3D11Device* m_pd3dDevice = nullptr; // ����̽� ��ü
	ID3D11DeviceContext* m_pImmediateContext = nullptr; // �׷��� �갡 ����?
	// m_pD3dDevice : �ٸ� �������̽� ������ ����Ѵ�
	// m_pImmediateContext : ������ �������̽��� ���(����,����) �Ҷ� ����Ѵ�
	IDXGIFactory* m_pGIFactory = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRTV = nullptr;
	ID3D11DepthStencilView* m_pDSV = nullptr;
	D3D11_VIEWPORT			m_ViewPort;

public:
	HRESULT CreateDevice();		// 1) ����̽� ����
	HRESULT CreateDXGIDevice(); // 2) ���丮 ����
	HRESULT CreateSwapChain();	// 3) ����ü�� ����
	HRESULT CreateRenderTargetView();	// 4) ����Ÿ�ٺ� ����
	void CreateViewport();	// 5) ����Ʈ ����

	HRESULT CreateDepthStencilView(); // ���� ���ٽ� �� ����

public:
	virtual HRESULT ResizeDevice(UINT width, UINT height);
	virtual HRESULT CreateDXResource();
	virtual HRESULT DeleteDXResource();

};
