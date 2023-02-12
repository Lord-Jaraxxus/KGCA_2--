#pragma once
#include "K_Std.h"

#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#pragma comment(lib, "DirectXTK.lib")

class K_Texture
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	ID3D11Device* m_pd3dDevice = nullptr; // 디바이스 객체
	ID3D11DeviceContext* m_pImmediateContext = nullptr; // 콘텍스트, 생성된 걸 컨트롤하는 용도
	ID3D11ShaderResourceView* m_pTextureSRV = nullptr; // 쉐이더 리소스 뷰, 진짜 텍스처는 얘가 텍스처
	ID3D11Texture2D* m_pTexture = nullptr; 
	//ID3D11Resource* m_pResource = nullptr; // 원래 쓰던거, ID3D11Texture2D의 부모임

public:
	D3D11_TEXTURE2D_DESC m_Desc;
	ID3D11Texture2D* GetTexture() { return m_pTexture; }
	ID3D11ShaderResourceView* GetSRV() { return m_pTextureSRV; }

public:
	HRESULT Load(	ID3D11Device* d3dDevice,
					ID3D11DeviceContext* pContext,
					std::wstring filename);
};

