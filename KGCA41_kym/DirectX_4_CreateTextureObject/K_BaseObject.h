#pragma once
#include "K_Std.h"

struct SimpleVertex
{
	k_Vector	p;
	k_Vector4D	c;
	k_Vector2D	t;
}; 

class K_BaseObject
{
public: 
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

	virtual bool Create(ID3D11Device* pd3dDevice,
						ID3D11DeviceContext* pContext,
						const wchar_t* textureName = 0,
						const wchar_t* shaderName = 0);

public:
	ID3D11Device* m_pd3dDevice = nullptr; // 디바이스 객체
	ID3D11DeviceContext* m_pImmediateContext = nullptr; // 콘텍스트, 생성된 걸 컨트롤하는 용도
	void SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext); // 디바이스와 콘텍스트를 넘겨주는 함수
public:
	std::vector<SimpleVertex> m_VertexList;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;
	ID3DBlob* m_pVSCode = nullptr;
	ID3DBlob* m_pPSCode = nullptr;

public:
	ID3D11ShaderResourceView* m_pTextureSRV = nullptr; // 쉐이더 리소스 뷰, 진짜 텍스처는 얘가 텍스처
	ID3D11Resource* m_pResource = nullptr;

public:
	HRESULT	CreateVertexBuffer();
	HRESULT CreateShader(std::wstring filename);
	HRESULT CreateVertexLayout();

	HRESULT LoadTexture(std::wstring filename);
};

