#include "K_FbxObject.h"

HRESULT K_FbxObject::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	m_pVertexBufferList.resize(m_vVertexDataListList.size());

	for (int i = 0; i < m_vVertexDataListList.size(); i++)
	{
		if (m_vVertexDataListList[i].size() > 0) 
		{
			ID3D11Buffer* VB = K_DX::CreateVertexBuffer(m_pd3dDevice, &m_vVertexDataListList[i].at(0), m_vVertexDataListList[i].size(), sizeof(PNCT_VERTEX));
			m_pVertexBufferList[i] = VB;
		}
	}
	return hr;
}

bool K_FbxObject::LoadTexture(std::wstring filename)
{
	W_STR szDefaultDir = L"../../data/fbx/";
	m_pTextureList.resize(m_vTextureNameList.size());
	for (int i = 0; i < m_vTextureNameList.size(); i++)
	{
		if (m_vTextureNameList[i] != L"") 
		{
			W_STR szLoadFile = szDefaultDir + m_vTextureNameList[i];
			m_pTextureList[i] = I_Tex.Load(szLoadFile);
		}
	}
	return true;
}

bool K_FbxObject::PostRender()
{
	if (m_pIndexBuffer == nullptr)
	{
		for (int iSubObj = 0; iSubObj < m_pVertexBufferList.size(); iSubObj++)
		{
			if (m_vVertexDataListList[iSubObj].size() <= 0) continue;
			//if (m_pTextureList[iSubObj] == nullptr) continue;
			UINT stride = sizeof(PNCT_VERTEX); // 정점1개의 바이트용량
			UINT offset = 0; // 정점버퍼에서 출발지점(바이트)
			m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBufferList[iSubObj], &stride, &offset);
			if (m_pTextureList[iSubObj] != nullptr) m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureList[iSubObj]->m_pTextureSRV);
			m_pImmediateContext->Draw(m_vVertexDataListList[iSubObj].size(), 0);
		}
	}

	else // 나중에 인덱스 추가해서 써먹을때 쓸듯? 지금은 m_dwFace없으니까 일단 냅둬
	{
		//m_pImmediateContext->DrawIndexed(m_dwFace * 3, 0, 0);
	}
	return true;
}

bool K_FbxObject::Release()
{
	K_BaseObject::Release();

	for (int iSubObj = 0; iSubObj < m_pVertexBufferList.size(); iSubObj++)
	{
		if (m_pVertexBufferList[iSubObj])
		{
			m_pVertexBufferList[iSubObj]->Release();
		}
	}
	return true;
}

K_Matrix K_FbxObject::InterPlate(float fTime)
{
//   10                20
//   A=0 ------------ B=20
//   t=0  ~  t=0.5f ~  t=1
	K_AnimeTrack A, B;
	A = m_AnimeTracks[max(0, fTime + 0)];
	B = m_AnimeTracks[min(50, fTime + 1)];
	float t = fTime - A.iFrame;

	TBASIS_EX::TVector3 pos;
	TBASIS_EX::D3DXVec3Lerp(&pos, &A.t, &B.t, t);
	TBASIS_EX::TVector3 scale;
	TBASIS_EX::D3DXVec3Lerp(&scale, &A.s, &B.s, t);
	TBASIS_EX::TQuaternion qRotation;
	TBASIS_EX::D3DXQuaternionSlerp(&qRotation, &A.r, &B.r, t);

	TBASIS_EX::TMatrix matScale;
	//TBASIS_EX::D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);
	TBASIS_EX::TMatrix matRotation;
	TBASIS_EX::D3DXMatrixRotationQuaternion(&matRotation, &qRotation);

	TBASIS_EX::TMatrix  matCurrent = /*matScale **/ matRotation;
	matCurrent._41 = pos.x;
	matCurrent._42 = pos.y;
	matCurrent._43 = pos.z;

	K_Matrix ResultMat = ConvertMatrixTtoK(matCurrent);

	return ResultMat;
}
