#pragma once
#include "K_FbxObject.h"

struct IndexWeight 
{
	std::vector<int>  Index;
	std::vector<float>  weight;
	void insert(int iBone, float fWeight)
	{
		for (int i = 0; i < Index.size(); i++)
		{
			if (fWeight > weight[i])
			{
				for (int j = Index.size() - 1; j > i; --j)
				{
					Index[j] = Index[j - 1];
					weight[j] = weight[j - 1];
				}
				Index[i] = iBone;
				weight[i] = fWeight;
				break;
			}
		}
	}
	IndexWeight()
	{
		Index.resize(8);
		weight.resize(8);
	}
};

class K_FbxObjectSkinning : public K_FbxObject
{
public:
	bool m_bSkinned = false;
	std::vector<ID3D11Buffer*> m_pVertexBufferList_IW;
	std::vector< std::vector<IW_VERTEX>>   m_vVertexDataListList_IW;
	std::vector<IndexWeight> m_IndexWeightList;
	std::map<UINT, K_Matrix> m_dxMatrixBindPseMap;

public:
	VS_CONSTANT_BONE_BUFFER  m_cbDataBone;
	ID3D11Buffer* m_pConstantBufferBone;

public:
	HRESULT CreateVertexLayout();
	HRESULT CreateVertexBuffer();
	HRESULT	CreateConstantBuffer();
	bool	PostRender();
	bool	Release();
};

