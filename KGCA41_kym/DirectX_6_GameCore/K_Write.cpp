#include "K_Write.h"

bool K_Write::Init()
{
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_d2dFactory);
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
							__uuidof(IDWriteFactory),
							(IUnknown**)&m_pDWriteFactory);

	// CreateTextFormat �Ű�������
	//WCHAR const* fontFamilyName;
	//IDWriteFontCollection* fontCollection;
	//DWRITE_FONT_WEIGHT fontWeight;
	//DWRITE_FONT_STYLE fontStyle;
	//DWRITE_FONT_STRETCH fontStretch;
	//FLOAT fontSize;
	//WCHAR const* localeName;
	//IDWriteTextFormat** textFormat;

	hr = m_pDWriteFactory->CreateTextFormat(
		L"���",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		100,
		L"ko-kr", // L"en-us"
		&m_pTextFormat);

	return true; 
}

bool K_Write::Frame()
{
	return true;
}

bool K_Write::Render()
{
	Draw(0, 0, m_szDefaultText, { 1,1,1,1 });
	return true;
}

bool K_Write::Release()
{
	if (m_pTextColor) m_pTextColor->Release();
	if (m_pDWriteFactory) m_pDWriteFactory->Release();
	if (m_d2dRT) m_d2dRT->Release();
	if (m_d2dFactory) m_d2dFactory->Release();

	return true;
}

bool K_Write::Set(IDXGISurface1* dxgiSurface)
{
	// CreateDxgiSurfaceRenderTarget �Ű�������
	//IDXGISurface* dxgiSurface;
	D2D1_RENDER_TARGET_PROPERTIES props;
	ZeroMemory(&props, sizeof(props));
	props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	props.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);
	props.dpiX = 96.0f; // ���� �� ��ġ, �� �ػ� 
	props.dpiY = 96.0f;
	props.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	props.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
	//ID2D1RenderTarget** renderTarget;

	// ** �߿� : ����̽� ���� �÷��� �����ؾ� �Ѵ� **
	// 3D->2D ����, 3D�� �ִ� ����۸� �����ͼ� 2D ����Ÿ������ �����Ҳ��� ��¶�
	HRESULT hr = m_d2dFactory->CreateDxgiSurfaceRenderTarget(
		dxgiSurface,
		&props,
		&m_d2dRT);

	hr = m_d2dRT->CreateSolidColorBrush({1,1,1,1}, &m_pTextColor);

	return true;
}

bool K_Write::Draw(float x, float y, std::wstring text, D2D1_COLOR_F color)
{
	m_d2dRT->BeginDraw();
	D2D1_RECT_F rt = { x,y, g_rtClient.right, g_rtClient.bottom };
	m_pTextColor->SetColor(color);
	m_d2dRT->DrawText(text.c_str(), text.size(), m_pTextFormat, rt, m_pTextColor);
	m_d2dRT->EndDraw();
	return true;
}
