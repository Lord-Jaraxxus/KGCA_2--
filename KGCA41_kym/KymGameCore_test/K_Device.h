#pragma once
#include "K_Window.h"
#include <d3d11.h>
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
	// 인터페이스 -> 하드웨어 직접 제어
	ID3D11Device* m_pd3dDevice = nullptr; // 디바이스 객체
	ID3D11DeviceContext* m_pImmediateContext = nullptr; // 그래서 얘가 뭐임?
	// m_pD3dDevice : 다른 인터페이스 생성시 사용한다
	// m_pImmediateContext : 생성된 인터페이스를 사용(관리,제어) 할때 사용한다
	IDXGIFactory* m_pGIFactory = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRTV = nullptr;

public:
	HRESULT CreateDevice();		// 1) 디바이스 생성
	HRESULT CreateDXGIDevice(); // 2) 팩토리 생성
	HRESULT CreateSwapChain();	// 3) 스왑체인 생성
	HRESULT CreateRenderTargetView();	// 4) 렌더타겟뷰 생성
	void CreateViewport();	// 5) 뷰포트 설정
};

