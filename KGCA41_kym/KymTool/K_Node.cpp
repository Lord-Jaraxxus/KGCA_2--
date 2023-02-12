#include "pch.h"

#include "K_Node.h"

void K_Node::CreateChildNode(K_Node* pParent, K_Map* pMap)
{
	// 0	1	2	3	4
	// 5	6	7	8	9
	// 10	11	12	13	14
	// 15	16	17	18	19
	// 20	21	22	23	24

	DWORD dwTopLeft = m_Corner[0];
	DWORD dwTopRight = m_Corner[1];
	DWORD dwBottomLeft = m_Corner[2];
	DWORD dwBottomRight = m_Corner[3];

	DWORD dwTopCenter = (dwTopLeft + dwTopRight) / 2;
	DWORD dwBottomCenter = (dwBottomLeft + dwBottomRight) / 2;
	DWORD dwLeftCenter = (dwTopLeft + dwBottomLeft) / 2;
	DWORD dwRightCenter = (dwTopRight + dwBottomRight) / 2;
	DWORD dwCenter = (dwTopLeft + dwTopRight + dwBottomLeft + dwBottomRight) / 4;

	// 0 → 1
	//   ↙     
	// 2 → 3 
	m_pChild[0] = new K_Node(pParent, pMap, dwTopLeft, dwTopCenter, dwLeftCenter, dwCenter);
	m_pChild[1] = new K_Node(pParent, pMap, dwTopCenter, dwTopRight, dwCenter, dwRightCenter);
	m_pChild[2] = new K_Node(pParent, pMap, dwLeftCenter, dwCenter, dwBottomLeft, dwBottomCenter);
	m_pChild[3] = new K_Node(pParent, pMap, dwCenter, dwRightCenter, dwBottomCenter, dwBottomRight);
}

void K_Node::CreateIndexData(K_Map* pMap)
{
	// 0	1	2	3	4
	// 5	6	7	8	9
	// 10	11	12	13	14
	// 15	16	17	18	19
	// 20	21	22	23	24

	DWORD dwTopLeft = m_Corner[0];
	DWORD dwTopRight = m_Corner[1];
	DWORD dwBottomLeft = m_Corner[2];
	DWORD dwBottomRight = m_Corner[3];

	DWORD dwNumRowCell = (dwBottomLeft - dwTopLeft) / pMap->m_dwNumColumns; // 셀의 행 갯수
	DWORD dwNumColCell = dwTopRight - dwTopLeft; // 셀의 열 갯수
	DWORD dwNumCells = dwNumRowCell * dwNumColCell; // 셀의 총 갯수

	m_IndexList.resize(dwNumCells * 2 * 3); // 셀의 갯수 * 2 -> 페이스 갯수 * 3 -> 인덱스리스트 갯수 
	int iIndex = 0;

	for (int hang = 0; hang < dwNumRowCell; hang++)
	{
		for (int yeol = 0; yeol < dwNumColCell; yeol++) 
		{
			// 0  1			 4
			// 2		  3  5
			m_IndexList[iIndex + 0] = dwTopLeft + (hang * pMap->m_dwNumColumns) + yeol;
			m_IndexList[iIndex + 1] = m_IndexList[iIndex + 0] + 1;
			m_IndexList[iIndex + 2] = m_IndexList[iIndex + 0] + pMap->m_dwNumColumns;

			m_IndexList[iIndex + 3] = m_IndexList[iIndex + 2];
			m_IndexList[iIndex + 4] = m_IndexList[iIndex + 1];
			m_IndexList[iIndex + 5] = m_IndexList[iIndex + 3] + 1;

			for (DWORD dwVertex = 0; dwVertex < 6; dwVertex++)
			{
				if (m_BB.vMin.y > pMap->m_VertexList[m_IndexList[iIndex + dwVertex]].p.y)
				{
					m_BB.vMin.y = pMap->m_VertexList[m_IndexList[iIndex + dwVertex]].p.y;
				}
				if (m_BB.vMax.y < pMap->m_VertexList[m_IndexList[iIndex + dwVertex]].p.y)
				{
					m_BB.vMax.y = pMap->m_VertexList[m_IndexList[iIndex + dwVertex]].p.y;
				}
			}
			iIndex += 6;
		}
	}

	m_dwFace = m_IndexList.size() / 3;
	m_BB.vCenter = (m_BB.vMax + m_BB.vMin) * 0.5f;
	m_BB.vAxis[0] = { 1,0,0 };
	m_BB.vAxis[1] = { 0,1,0 };
	m_BB.vAxis[2] = { 0,0,1 };
	m_BB.fExtent[0] = m_BB.vMax.x - m_BB.vCenter.x;
	m_BB.fExtent[1] = m_BB.vMax.y - m_BB.vCenter.y;
	m_BB.fExtent[2] = m_BB.vMax.z - m_BB.vCenter.z;
	//   5    6  
	// 1    2
	// ->   <-
	//   4    7
	// 0    3 
	m_BB.vPos[0] = m_BB.vMin;
	m_BB.vPos[1] = { m_BB.vMin.x, m_BB.vMax.y, m_BB.vMin.z};
	m_BB.vPos[2] = { m_BB.vMax.x, m_BB.vMax.y, m_BB.vMin.z };
	m_BB.vPos[3] = { m_BB.vMax.x, m_BB.vMin.y, m_BB.vMin.z };
	m_BB.vPos[4] = { m_BB.vMin.x, m_BB.vMin.y, m_BB.vMax.z };
	m_BB.vPos[5] = { m_BB.vMin.x, m_BB.vMax.y, m_BB.vMax.z };
	m_BB.vPos[6] = m_BB.vMax;
	m_BB.vPos[7] = { m_BB.vMax.x, m_BB.vMin.y, m_BB.vMax.z };

	m_BS.vCenter = m_BB.vCenter;
	k_Vector aa = m_BB.vMax - m_BB.vCenter;
	m_BS.fRadius = aa.Length();	
}

HRESULT K_Node::CreateIndexBuffer(K_Map* pMap)
{
	HRESULT hr;

	D3D11_BUFFER_DESC       bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size(); // 바이트 용량
	// GPU 메모리에 할당
	bd.Usage = D3D11_USAGE_DEFAULT; // 버퍼의 할당 장소 내지는 버퍼용도
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA  sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = &m_IndexList.at(0);
	hr = pMap->m_pd3dDevice->CreateBuffer(
		&bd, // 버퍼 할당
		&sd, // 초기 할당된 버퍼를 체우는 CPU메모리 주소
		&m_pIndexBuffer);

	return hr;
}

void K_Node::CreateBoundingBox(K_Map* pMap)
{
	DWORD dwTopLeft = m_Corner[0];
	DWORD dwTopRight = m_Corner[1];
	DWORD dwBottomLeft = m_Corner[2];
	DWORD dwBottomRight = m_Corner[3];

	m_BB.vMin.x = pMap->m_VertexList[dwBottomLeft].p.x;
	m_BB.vMin.y = 100000.0f;
	m_BB.vMin.z = pMap->m_VertexList[dwBottomLeft].p.z;

	m_BB.vMax.x = pMap->m_VertexList[dwTopRight].p.x;
	m_BB.vMax.y = -100000.0f;
	m_BB.vMax.z = pMap->m_VertexList[dwTopRight].p.z;
}

K_Node::K_Node(K_Node* pParent, K_Map* pMap, DWORD dwLT, DWORD dwRT, DWORD dwRB, DWORD dwLB)
{
	if (pParent != nullptr) m_iDepth = pParent->m_iDepth + 1;
	
	m_Corner[0] = dwLT;
	m_Corner[1] = dwRT;
	m_Corner[2] = dwRB;
	m_Corner[3] = dwLB;
	m_bLeaf = false;
	m_pChild.resize(4);

	// boundingBox( aabb, obb )
	// 0  1   2   3   4
	// 5  6   7   8   9
	// 10 11  12  13  14
	// 15 16  17  18  19
	// 20  21 22  23  24

	CreateBoundingBox(pMap);
	CreateIndexData(pMap);
	CreateIndexBuffer(pMap);
}

K_Node::~K_Node()
{
	if (m_pIndexBuffer)m_pIndexBuffer->Release();

	delete m_pChild[0];
	delete m_pChild[1];
	delete m_pChild[2];
	delete m_pChild[3];

	m_pChild[0] = nullptr;
	m_pChild[1] = nullptr;
	m_pChild[2] = nullptr;
	m_pChild[3] = nullptr;
}
