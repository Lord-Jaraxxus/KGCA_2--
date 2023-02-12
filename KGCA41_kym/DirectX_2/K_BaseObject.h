#pragma once
#include "K_Std.h"

struct SimpleVertex
{
	k_Vector	p;
	k_Vector4D	c;
}; 

class K_BaseObject
{
public: 
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	ID3D11Device* m_pd3dDevice = nullptr; // ����̽� ��ü
	ID3D11DeviceContext* m_pImmediateContext = nullptr; // ���ؽ�Ʈ, ������ �� ��Ʈ���ϴ� �뵵
	void SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext); // ����̽��� ���ؽ�Ʈ�� �Ѱ��ִ� �Լ�
public:
	std::vector<SimpleVertex> m_VertexList;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;
	ID3DBlob* m_pVSCode = nullptr;
	ID3DBlob* m_pPSCode = nullptr;

public:
	HRESULT	CreateVertexBuffer();
	HRESULT CreateShader();
	HRESULT CreateVertexLayout();
};

