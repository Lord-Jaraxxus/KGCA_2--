#pragma once
#include "K_Device.h"

struct SimpleVertex
{
	float x;
	float y;
	float z;
	//float r;
	//float g;
	//float b;
	//float a;
};

class Sample : public K_Device
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;
	ID3DBlob* m_pVSCode = nullptr;
	ID3DBlob* m_pPSCode = nullptr;

public:
	ID3D11ShaderResourceView** textureView; // 얘가 인터페이스, 이게 지금 우리한텐 하나의 이미지다

public:
	HRESULT	CreateVertexBuffer();
	HRESULT CreateShader();
	HRESULT CreateVertexLayout();
}; 

