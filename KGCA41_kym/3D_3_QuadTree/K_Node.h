#pragma once
#include "K_Map.h"
#include "K_Collision.h"

class K_Node
{
public:
	int						m_iDepth = 0;
	bool					m_bLeaf = false;
	DWORD					m_Corner[4];
	K_BOX					m_BB; // Bounding Box, Ben Brode... ±×¸³À¾´Ï´Ù 
	K_Sphere				m_BS; // Bounding Sphere
	DWORD					m_dwFace; // ¾êµµ ÇÊ¿äÇÑ°¡?
	std::vector<DWORD>		m_IndexList;
	ID3D11Buffer*			m_pIndexBuffer;
	std::vector<K_Node*>	m_pChild;

	// RECT m_rt; // ¾ê ÇÊ¿äÇÔ?

public:
	void	CreateChildNode(K_Node* pParent, K_Map* pMap);
	void	CreateIndexData(K_Map* pMap);
	HRESULT CreateIndexBuffer(K_Map* pMap);
	void	CreateBoundingBox(K_Map* pMap);

public:
	K_Node(K_Node* pParent, K_Map* pMap, DWORD dwLT, DWORD dwRT, DWORD dwRB, DWORD dwLB);
	virtual ~K_Node();
};

