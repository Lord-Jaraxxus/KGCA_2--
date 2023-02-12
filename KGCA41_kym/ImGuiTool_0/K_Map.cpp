#include "K_Map.h"

bool K_Map::Build(UINT iWidth, UINT iHeight)
{
	// ���� �ڵ忡 �������, �ڵ� ������ �ϴ� �����...
	m_dwNumRows = iWidth;
	m_dwNumColumns = iHeight;

	// ��ü �� ���� = (iWidth-1)*(iHeigth-1)
	 // ��ü ���̽� ���� = (iWidth-1)*(iHeigth-1)*2
	 // ��ü ���� ���� = iWidth * iHeight
	m_iNumRows = iWidth;
	m_iNumCols = iHeight;
	m_iNumCellCols = m_iNumCols - 1;
	m_iNumCellRows = m_iNumRows - 1;

	m_VertexList.resize(iWidth * iHeight);
	int iHalfWidth = iWidth / 2;
	int iHalfHeight = iHeight / 2;

	float fCellDistance = 1.0f;

	if (m_fHeightList.size() == 0) 
	{
		m_fHeightList.resize(iWidth * iHeight);
		for (int i = 0; i < m_fHeightList.size(); i++) { m_fHeightList[i] = 0.0f; }
	}

	for (int iRow = 0; iRow < iHeight; iRow++)
	{
		for (int iCol = 0; iCol < iWidth; iCol++)
		{
			m_VertexList[iRow * iWidth + iCol].p =
			{ (float)(iCol - iHalfWidth) * fCellDistance,						// x
				m_fHeightList[iRow * m_iNumCols + iCol] * m_fScaleHeight,		// y, ���̸ʿ��� �����°ɷ� �ٲ���
				(float)(iHalfHeight - iRow) * fCellDistance };					// z

			m_VertexList[iRow * iWidth + iCol].c = { 1,1,1,1 };

			m_VertexList[iRow * iWidth + iCol].t =
			{ ((float)iCol / (float)(iWidth - 1)) * 10.0f,
			  ((float)iRow / (float)(iHeight - 1)) * 10.0f };
		}
	}

	m_IndexList.resize((iWidth - 1) * (iHeight - 1) * 2 * 3.0f);
	UINT iIndex = 0;
	for (int iRow = 0; iRow < iHeight - 1; iRow++)
	{
		for (int iCol = 0; iCol < iWidth - 1; iCol++)
		{
			UINT iNextRow = iRow + 1;
			UINT iNextCol = iCol + 1;
			m_IndexList[iIndex + 0] = iRow * iWidth + iCol;
			m_IndexList[iIndex + 1] = m_IndexList[iIndex + 0] + 1;
			m_IndexList[iIndex + 2] = iNextRow * iWidth + iCol;

			m_IndexList[iIndex + 3] = m_IndexList[iIndex + 2];
			m_IndexList[iIndex + 4] = m_IndexList[iIndex + 1];
			m_IndexList[iIndex + 5] = m_IndexList[iIndex + 3] + 1;

			iIndex += 6;
		}
	}

	indexlist.resize(m_IndexList.size());
	m_dwFace = m_IndexList.size() / 3;

	GenVertexNormal();

	return true;
}

bool K_Map::UpdateBuffer(K_DebugCamera* pMainCamera)
{
	m_dwFace = 0; // BaseObject����, �� ���ݱ��� �Ⱦ���?
	DWORD index = 0;
	k_Vector v[3];
	for (int iFace = 0; iFace < m_IndexList.size() / 3; iFace++)
	{
		UINT i0 = m_IndexList[iFace * 3 + 0];
		UINT i1 = m_IndexList[iFace * 3 + 1];
		UINT i2 = m_IndexList[iFace * 3 + 2];
		v[0] = m_VertexList[i0].p;
		v[1] = m_VertexList[i1].p;
		v[2] = m_VertexList[i2].p;
		for (int i = 0; i < 3; i++)
		{
			bool bRender = pMainCamera->m_vFrustum.ClassifyPoint(v[i]);
			if (bRender)
			{
				indexlist[index++] = i0;
				indexlist[index++] = i1;
				indexlist[index++] = i2;
				m_dwFace++;
				break;
			}
		}
	}
	m_pImmediateContext->UpdateSubresource(
		m_pIndexBuffer, 0, nullptr,
		&indexlist.at(0), 0, 0);
	return true;
}

bool K_Map::LoadHeightMap(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, W_STR loadTexture)
{
	ID3D11Resource* pTexture;
	HRESULT hr = DirectX::CreateWICTextureFromFileEx(pd3dDevice, loadTexture.c_str(),
		0, D3D11_USAGE_STAGING, 0,
		D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE, 0,
		WIC_LOADER_DEFAULT,
		&pTexture, nullptr);

	ID3D11Texture2D* pTexture2D = NULL;
	if (FAILED(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D)))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC td;
	pTexture2D->GetDesc(&td);

	m_fHeightList.resize(td.Height * td.Width);

	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(pImmediateContext->Map(pTexture2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0,
		&MappedFaceDest)))
	{
		UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
		for (UINT row = 0; row < td.Height; row++)
		{
			UINT rowStart = row * MappedFaceDest.RowPitch;
			for (UINT col = 0; col < td.Width; col++)
			{
				UINT colStart = col * 4;
				UINT uRed = pTexels[rowStart + colStart + 0];
				m_fHeightList[row * td.Width + col] = (float)uRed / 255.0f;
			}
		}
		pImmediateContext->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1));
	}

	m_iNumRows = td.Height;
	m_iNumCols = td.Width;
	pTexture2D->Release();

	return true;
}

void K_Map::GenVertexNormal()
{
	std::vector<k_Vector> m_FaceNormals;
	m_FaceNormals.resize(m_dwFace); //��...... ���̽�������Ʈ�� �߰�.. �ؾ߰���..?
	UINT iFace = 0;
	for (UINT i = 0; i < m_IndexList.size(); i += 3)
	{
		UINT i0 = m_IndexList[i + 0];
		UINT i1 = m_IndexList[i + 1];
		UINT i2 = m_IndexList[i + 2];
		m_FaceNormals[iFace++] = ComputeFaceNormal(i0, i1, i2);
	}
	struct tVertexInfo
	{
		std::vector<UINT >  faceIndexArray;
		k_Vector vNormal;
	};
	std::vector<tVertexInfo> m_VertexInfo;
	m_VertexInfo.resize(m_iNumCols * m_iNumRows);
	for (UINT iFace = 0; iFace < m_dwFace; iFace++)
	{
		for (UINT i = 0; i < 3; i++)
		{
			UINT i0 = m_IndexList[iFace * 3 + i];
			m_VertexInfo[i0].faceIndexArray.push_back(iFace);
		}
	}
	for (UINT iVertex = 0; iVertex < m_VertexInfo.size(); iVertex++)
	{
		for (UINT i = 0; i < m_VertexInfo[iVertex].faceIndexArray.size(); i++)
		{
			m_VertexInfo[iVertex].vNormal +=
				m_FaceNormals[m_VertexInfo[iVertex].faceIndexArray[i]];
		}
		m_VertexInfo[iVertex].vNormal.Normalize();

		m_VertexList[iVertex].n = m_VertexInfo[iVertex].vNormal;

#ifdef _DEBUG
		k_Vector vLight = { 0, 1, 0 };
		float fDot = vLight | m_VertexList[iVertex].n;
		m_VertexList[iVertex].c = { fDot ,fDot ,fDot , 1 };
#endif
	}
}

k_Vector K_Map::ComputeFaceNormal(UINT i0, UINT i1, UINT i2)
{
	TBASIS_EX::TVector3 vNormal;
	TBASIS_EX::TVector3 e0 = ConvertVector3KtoT(m_VertexList[i1].p - m_VertexList[i0].p);
	TBASIS_EX::TVector3 e1 = ConvertVector3KtoT(m_VertexList[i2].p - m_VertexList[i0].p);
	TBASIS_EX::D3DXVec3Cross(&vNormal, &e0, &e1);
	TBASIS_EX::D3DXVec3Normalize(&vNormal, &vNormal);

	k_Vector vResult = ConvertVector3TtoK(vNormal);
	return vResult;
}

float K_Map::GetHeight(float fPosX, float fPosZ)
{
	// fPosX/fPosZ�� ��ġ�� �ش��ϴ� ���̸ʼ��� ã�´�.
	// m_iNumCols��m_iNumRows�� ����/������ ���� ũ�Ⱚ��.
	float fCellX = (float)(m_iNumCellCols * m_fCellDistance / 2.0f + fPosX);
	float fCellZ = (float)(m_iNumCellRows * m_fCellDistance / 2.0f - fPosZ);

	// ���� ũ��� ������ 0~1 ������ ������ �ٲپ� ���̸� �迭�� �����Ѵ�.
	fCellX /= (float)m_fCellDistance;
	fCellZ /= (float)m_fCellDistance;

	// fCellX, fCellZ ������ �۰ų� ���� �ִ� ����( �Ҽ��κ��� �߶󳽴�.)
	float fVertexCol = ::floorf(fCellX);
	float fVertexRow = ::floorf(fCellZ);

	// ���̸� ������ ����� ������ �ʱ�ȭ �Ѵ�.
	if (fVertexCol < 0.0f)  fVertexCol = 0.0f;
	if (fVertexRow < 0.0f)  fVertexRow = 0.0f;
	if ((float)(m_iNumCols - 2) < fVertexCol)	fVertexCol = (float)(m_iNumCols - 2);
	if ((float)(m_iNumRows - 2) < fVertexRow)	fVertexRow = (float)(m_iNumRows - 2);

	// ���� ���� �÷��� �����ϴ� 4�� ������ ���̰��� ã�´�. 
	//  A   B
	//  *---*
	//  | / |
	//  *---*  
	//  C   D
	float A = GetHeightmap((int)fVertexRow, (int)fVertexCol);
	float B = GetHeightmap((int)fVertexRow, (int)fVertexCol + 1);
	float C = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol);
	float D = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol + 1);

	// A������ ��ġ���� ������ ��(������)�� ����Ѵ�. 0 ~ 1.0f
	float fDeltaX = fCellX - fVertexCol;
	float fDeltaZ = fCellZ - fVertexRow;
	// �����۾��� ���� ���� ���ս��� ã�´�. 
	float fHeight = 0.0f;
	// �����̽��� �������� �����Ѵ�.
	// fDeltaZ + fDeltaX < 1.0f
	if (fDeltaZ < (1.0f - fDeltaX))  //ABC
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C	
						  // �� ������ ���̰��� ���̸� ���Ͽ� ��ŸX�� ���� ���� �������� ã�´�.		
		fHeight = A + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDeltaZ);
	}
	// �Ʒ����̽��� �������� �����Ѵ�.
	else // DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B
						  // �� ������ ���̰��� ���̸� ���Ͽ� ��ŸZ�� ���� ���� �������� ã�´�.		
		fHeight = D + Lerp(0.0f, uy, 1.0f - fDeltaX) + Lerp(0.0f, vy, 1.0f - fDeltaZ);
	}
	return fHeight;
}

float K_Map::GetHeightmap(int row, int col)
{
	return m_VertexList[row * m_iNumRows + col].p.y;
}

float K_Map::Lerp(float fStart, float fEnd, float fTangent)
{
	return fStart - (fStart * fTangent) + (fEnd * fTangent);
}
