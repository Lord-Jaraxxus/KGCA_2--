#pragma once
#include "K_Std.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

class K_Write : public K_Singleton<K_Write>
{
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	ID2D1Factory* m_d2dFactory;
	ID2D1RenderTarget* m_d2dRT;
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat; // 폰트 형태들 들어가는 곳
	ID2D1SolidColorBrush* m_pTextColor; // 글자 색상

public:
	std::wstring m_szDefaultText = L"AAAAAAAA";

public:
	IDXGISurface1* m_pBackBuffer;
	bool Set(IDXGISwapChain* pSwapChain); // 백버퍼를 연동
	bool Draw(float x, float y, std::wstring text, D2D1_COLOR_F color = {0,0,0,1});
};

#define I_Write K_Write::GetInstance()