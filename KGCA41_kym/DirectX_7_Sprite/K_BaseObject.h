#pragma once
#include "K_Texture.h"
#include "K_TextureManager.h"
#include "K_Shader.h"
#include "K_ShaderManager.h"

#include"k_Vector.h"
#include"k_Vector2D.h"

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

public:
	K_Shader* m_pShader;
	K_Texture* m_pTexture;

public:
	HRESULT	CreateVertexBuffer();
	bool CreateShader(std::wstring filename);
	HRESULT CreateVertexLayout();
	
	bool LoadTexture(std::wstring filename);
};

