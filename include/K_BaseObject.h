#pragma once
#include "K_Texture.h"
#include "K_TextureManager.h"
#include "K_Shader.h"
#include "K_ShaderManager.h"

#include "k_Vector.h"
#include "k_Vector2D.h"
#include "K_Matrix.h"

struct SimpleVertex
{
	k_Vector	p;
	k_Vector4D	c;
	k_Vector2D	t;

	SimpleVertex() {}
	SimpleVertex(k_Vector vp, k_Vector4D vc, k_Vector2D vt)
	{
		p = vp;
		c = vc;
		t = vt;
	}
}; 

struct PNCT_VERTEX
{
	k_Vector	p;
	k_Vector	n;
	k_Vector4D	c;
	k_Vector2D	t;

	PNCT_VERTEX() {}
	PNCT_VERTEX(k_Vector vp, k_Vector vn, k_Vector4D vc, k_Vector2D vt)
	{
		p = vp;
		n = vn;
		c = vc;
		t = vt;
	}
};

struct VS_CONSTANT_BUFFER
{
	K_Matrix  matWorld;
	K_Matrix  matView;
	K_Matrix  matProj;
	float    x;
	float    y;
	float    z;
	float    fTimer;
};

struct IW_VERTEX 
{
	k_Vector4D i;
	k_Vector4D w;
	IW_VERTEX() {}
	IW_VERTEX(k_Vector4D vIndex, k_Vector4D vWeight)
	{
		i = vIndex;
		w = vWeight;
	}
};

struct VS_CONSTANT_BONE_BUFFER
{
	K_Matrix matBone[255];
};

namespace K_DX
{
	ID3D11Buffer* CreateVertexBuffer(ID3D11Device* pd3dDevice, void* pDataAddress, UINT iNumVertex, UINT iVertexSize);

	class K_BaseObject
	{
	public:
		virtual bool Init();
		virtual bool Frame();
		virtual bool PreRender();
		virtual bool Render();
		virtual bool PostRender();
		virtual bool Release();

		void SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext); // 디바이스와 콘텍스트를 넘겨주는 함수
		virtual bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, std::wstring textureName = 0, std::wstring shaderName = 0);

	public:
		virtual void		CreateVertexData();
		virtual void		CreateIndexData();
		virtual void		CreateConstantData();
		virtual HRESULT		CreateVertexBuffer();
		virtual HRESULT		CreateIndexBuffer();
		virtual HRESULT		CreateConstantBuffer();
		virtual bool		CreateShader(std::wstring filename);
		virtual HRESULT		CreateVertexLayout();
		virtual void		UpdateVertexBuffer();
		virtual void		UpdateConstantBuffer();
		virtual void		SetMatrix(K_Matrix* matWorld, K_Matrix* matView, K_Matrix* matProj);

		virtual bool		LoadTexture(std::wstring filename);

	public:
		ID3D11Device* m_pd3dDevice = nullptr; // 디바이스 객체
		ID3D11DeviceContext* m_pImmediateContext = nullptr; // 콘텍스트, 생성된 걸 컨트롤하는 용도

	public:
		std::vector<PNCT_VERTEX>	m_VertexList;
		std::vector<PNCT_VERTEX>	m_InitVertexList;
		std::vector<DWORD>			m_IndexList;
		ID3D11Buffer* m_pVertexBuffer;
		ID3D11Buffer* m_pIndexBuffer;
		VS_CONSTANT_BUFFER  m_cbData;
		ID3D11Buffer* m_pConstantBuffer;
		ID3D11InputLayout* m_pVertexLayout;
		ID3D11ShaderResourceView* m_pTextureSRV = nullptr; // 얜 또 언제 추가됬디야..

	public:
		ID3D11VertexShader* m_pVS;
		ID3D11PixelShader* m_pPS;
		ID3DBlob* m_pVSCode = nullptr;
		ID3DBlob* m_pPSCode = nullptr;

	public:
		K_Shader* m_pShader;
		K_Texture* m_pTexture;
		std::wstring m_szShaderName;
		std::wstring m_szTextureName;

	public:
		K_Matrix	m_matWorld;
		K_Matrix	m_matView;
		K_Matrix	m_matProj;
		DWORD		m_dwFace = 0;
	};
}

using namespace K_DX;

