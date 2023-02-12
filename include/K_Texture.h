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
	ID3D11Device* m_pd3dDevice = nullptr; // ����̽� ��ü
	ID3D11DeviceContext* m_pImmediateContext = nullptr; // ���ؽ�Ʈ, ������ �� ��Ʈ���ϴ� �뵵
	ID3D11ShaderResourceView* m_pTextureSRV = nullptr; // ���̴� ���ҽ� ��, ��¥ �ؽ�ó�� �갡 �ؽ�ó
	ID3D11Texture2D* m_pTexture = nullptr; 
	//ID3D11Resource* m_pResource = nullptr; // ���� ������, ID3D11Texture2D�� �θ���

public:
	D3D11_TEXTURE2D_DESC m_Desc;
	ID3D11Texture2D* GetTexture() { return m_pTexture; }
	ID3D11ShaderResourceView* GetSRV() { return m_pTextureSRV; }

public:
	HRESULT Load(	ID3D11Device* d3dDevice,
					ID3D11DeviceContext* pContext,
					std::wstring filename);
};

