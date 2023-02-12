#pragma once
#include "K_BaseObject.h"
#include "K_DebugCamera.h"

class K_Map : public K_BaseObject
{
public:
	DWORD        m_dwNumRows;
	DWORD        m_dwNumColumns;
	DWORD        m_iNumRows;
	DWORD        m_iNumCols;
	UINT         m_iNumCellCols;
	UINT         m_iNumCellRows;
	float		 m_fCellDistance = 1.0f;
	float        m_fScaleHeight = 100.0f;
	DWORD_VECTOR indexlist;
	std::vector<float>   m_fHeightList;
public:
	bool  Build(UINT iWidth, UINT iHeight);
	bool  UpdateBuffer(K_DebugCamera* pMainCamera);
	bool  LoadHeightMap(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, W_STR loadTexture);
	void  GenVertexNormal();
	k_Vector ComputeFaceNormal(UINT i0, UINT i1, UINT i2);
	float GetHeight(float fPosX, float fPosZ);
	float GetHeightmap(int row, int col);
	float Lerp(float fStart, float fEnd, float fTangent);
};
