#include "K_FbxObjectSkinning.h"

HRESULT K_FbxObjectSkinning::CreateVertexLayout()
{
	HRESULT hr;
	if (m_pVSCode == nullptr)
	{
		return E_FAIL;
	}
	// 정점레이아웃은 정점쉐이더 밀접한 관련.
	// 정점레이아웃 생성시 사전에 정점쉐이더 생성이 필요하다.
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,   0,12,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,24,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT,    0,40,D3D11_INPUT_PER_VERTEX_DATA, 0},

		{ "INDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,0,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,16,D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	UINT NumElements = sizeof(ied) / sizeof(ied[0]);
	hr = m_pd3dDevice->CreateInputLayout(
		ied,
		NumElements,
		m_pVSCode->GetBufferPointer(),
		m_pVSCode->GetBufferSize(),
		&m_pVertexLayout);

	return hr;
}

HRESULT K_FbxObjectSkinning::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	K_FbxObject::CreateVertexBuffer();
	m_pVertexBufferList_IW.resize(m_vVertexDataListList_IW.size());

		for (int ivb = 0; ivb < m_vVertexDataListList_IW.size(); ivb++)
		{
			if (m_vVertexDataListList_IW[ivb].size() > 0)
			{
				m_pVertexBufferList_IW[ivb] =
					K_DX::CreateVertexBuffer(m_pd3dDevice,
						&m_vVertexDataListList_IW[ivb].at(0),
						m_vVertexDataListList_IW[ivb].size(),
						sizeof(IW_VERTEX));
			}
		}

	return hr;
}

bool K_FbxObjectSkinning::PostRender()
{
	if (m_bSkinned)
	{
		m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferBone);
	}

	if (m_pIndexBuffer == nullptr)
	{
			for (int iSubObj = 0; iSubObj < m_pVertexBufferList.size(); iSubObj++)
			{
				if (m_vVertexDataListList[iSubObj].size() <= 0) continue;
				UINT stride[2] = { sizeof(PNCT_VERTEX), sizeof(IW_VERTEX) }; // 정점1개의 바이트용량
				UINT offset[2] = { 0, 0 }; // 정점버퍼에서 출발지점(바이트)
				//SLOT(레지스터리)
				ID3D11Buffer* buffer[2] = { m_pVertexBufferList[iSubObj],m_pVertexBufferList_IW[iSubObj] };
				m_pImmediateContext->IASetVertexBuffers(0, 2, buffer, stride, offset);

				//if (m_pTextureList.size() > 0 && m_pTextureList[iSubObj] != nullptr)
				if ( m_pTextureList[iSubObj] != nullptr)
				{
					m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureList[iSubObj]->m_pTextureSRV);
				}
				m_pImmediateContext->Draw(m_vVertexDataListList[iSubObj].size(), 0);
			}
		}
	else
	{
		//m_pImmediateContext->DrawIndexed(m_dwFace * 3, 0, 0);
	}

	return true;
}

bool K_FbxObjectSkinning::Release()
{
	K_FbxObject::Release();
	return true;
}

HRESULT K_FbxObjectSkinning::CreateConstantBuffer()
{
	K_BaseObject::CreateConstantBuffer();

	HRESULT hr;
	for (int iBone = 0; iBone < 255; iBone++)
	{
		m_cbDataBone.matBone[iBone].Identity();
	}

	D3D11_BUFFER_DESC       bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(VS_CONSTANT_BONE_BUFFER) * 1; // 바이트 용량
	// GPU 메모리에 할당
	bd.Usage = D3D11_USAGE_DEFAULT; // 버퍼의 할당 장소 내지는 버퍼용도
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA  sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = &m_cbDataBone;
	hr = m_pd3dDevice->CreateBuffer(
		&bd, // 버퍼 할당
		&sd, // 초기 할당된 버퍼를 체우는 CPU메모리 주소
		&m_pConstantBufferBone);

	return hr;
}