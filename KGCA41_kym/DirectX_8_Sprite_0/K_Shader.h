#pragma once
#include "K_Std.h"

#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h> // D3DCompileFromFile()

class K_Shader
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	ID3D11Device* m_pd3dDevice = nullptr; // ����̽� ��ü
	ID3D11DeviceContext* m_pImmediateContext = nullptr; // ���ؽ�Ʈ, ������ �� ��Ʈ���ϴ� �뵵

public:
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;
	ID3DBlob* m_pVSCode = nullptr;
	ID3DBlob* m_pPSCode = nullptr;

public:
	HRESULT Load(ID3D11Device* d3dDevice,
			 	 ID3D11DeviceContext* pContext,
			 	 std::wstring filename);
}; 

