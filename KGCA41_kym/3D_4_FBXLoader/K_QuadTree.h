#pragma once
#include "K_Node.h"

class K_QuadTree
{
	K_Node* m_pRootNode = nullptr;
	int m_iMaxDepth;
	K_Map* m_pMap = nullptr;
	K_Camera* m_pCamera = nullptr;
	std::vector<K_Node*> m_pLeafNodeList;
	std::vector<K_Node*> m_pDrawNodeList;

public:
	bool	Create(K_Camera* pMainCamera, K_Map* pMap, int iMaxDepth = 100);
	bool	BuildTree(K_Node* pNode);
	bool	IsSubDivide(K_Node* pNode);
	void	NodeVisibleCheckPoint(K_Node* pNode);
	void	NodeVisibleCheckSphere(K_Node* pNode);
	void	NodeVisibleCheckOBB(K_Node* pNode);
	virtual ~K_QuadTree();

public:
	bool	Frame();
	bool	Render();
};

