#include "pch.h"

#include "K_QuadTree.h"

bool K_QuadTree::Create(K_Camera* pMainCamera, K_Map* pMap, int iMaxDepth)
{
	m_pCamera = pMainCamera;
	m_pMap = pMap;
	m_iMaxDepth = iMaxDepth;

	m_pRootNode = new K_Node(nullptr, m_pMap, 
		0, 
		m_pMap->m_dwNumColumns - 1, 
		(m_pMap->m_dwNumColumns * m_pMap->m_dwNumRows) - m_pMap->m_dwNumColumns,
		(m_pMap->m_dwNumColumns * m_pMap->m_dwNumRows) - 1);
	BuildTree(m_pRootNode);

	return true;
}

bool K_QuadTree::BuildTree(K_Node* pNode)
{
	if (pNode == nullptr) return false;

	if (IsSubDivide(pNode) == false) 
	{
		pNode->m_bLeaf = true;
		m_pLeafNodeList.push_back(pNode);
		return false;
	}

	pNode->CreateChildNode(pNode, m_pMap);

	BuildTree(pNode->m_pChild[0]);
	BuildTree(pNode->m_pChild[1]);
	BuildTree(pNode->m_pChild[2]);
	BuildTree(pNode->m_pChild[3]);

	return true;
}

bool K_QuadTree::IsSubDivide(K_Node* pNode)
{
	if (pNode->m_iDepth >= m_iMaxDepth) return false;
	if ((pNode->m_Corner[1] - pNode->m_Corner[0]) == 1) return false;

	return true;
}

void K_QuadTree::NodeVisibleCheckPoint(K_Node* pNode)
{
	bool Ret[8] = { true, true, true, true, true, true, true, true };
	for (int i = 0; i < 8; i++)
	{
		Ret[i] = m_pCamera->m_vFrustum.ClassifyPoint(pNode->m_BB.vPos[i]);
	}

	int check = 0; // 0이면 완전 포함, 1이면 몇개만 포함, 2면 완전 아웃

	// 하나라도 아웃이면 일단 check를 1로
	for (int i = 0; i < 8; i++)
	{
		if (Ret[i] == false) check = 1;
	}
	// 체크가 1일때
	if (check == 1)
	{
		check = 2;
		for (int i = 0; i < 8; i++)
		{
			if (Ret[i] == true) check = 1; // 하나라도 true면 check는 1, 아니면 죄다 false이므로 check는 2
		}
	}


	if (check == 0) // 완전 포함
	{
		m_pDrawNodeList.push_back(pNode);
	}
	if (check == 1) // 일부 포함
	{
		if (pNode->m_bLeaf)
		{
			m_pDrawNodeList.push_back(pNode);
		}
		else
		{
			for (int iNode = 0; iNode < 4; iNode++)
			{
				NodeVisibleCheckPoint(pNode->m_pChild[iNode]);
			}
		}
	}
}

void K_QuadTree::NodeVisibleCheckSphere(K_Node* pNode)
{
	K_POSITION Ret = m_pCamera->m_vFrustum.ClassifySphere(pNode->m_BS);

	if (Ret == K_FRONT) m_pDrawNodeList.push_back(pNode); // 완전 포함

	if (Ret == K_SPANNING) 
	{
		if (pNode->m_bLeaf) m_pDrawNodeList.push_back(pNode);
		
		else
		{
			for (int iNode = 0; iNode < 4; iNode++)
			{
				NodeVisibleCheckSphere(pNode->m_pChild[iNode]);
			}
		}
	}

}

void K_QuadTree::NodeVisibleCheckOBB(K_Node* pNode)
{
	K_POSITION dwRet = m_pCamera->m_vFrustum.ClassifyBox(pNode->m_BB);

	if (K_FRONT == dwRet)// 완전포함.
	{
		m_pDrawNodeList.push_back(pNode);
	}

	if (K_SPANNING == dwRet) // 걸쳐있다.
	{
		if (pNode->m_bLeaf)
		{
			m_pDrawNodeList.push_back(pNode);
		}
		else
		{
			for (int iNode = 0; iNode < 4; iNode++)
			{
				NodeVisibleCheckOBB(pNode->m_pChild[iNode]);
			}
		}
	}
}

K_QuadTree::~K_QuadTree()
{
	if (m_pRootNode)
	{
		delete m_pRootNode;
		m_pRootNode = nullptr;
	}
}

bool K_QuadTree::Frame()
{
	m_pDrawNodeList.clear();
	NodeVisibleCheckOBB(m_pRootNode);
	return true;
}

bool K_QuadTree::Render()
{
	for (auto node : m_pDrawNodeList)
	{
		m_pMap->PreRender();
		m_pMap->m_pImmediateContext->IASetIndexBuffer(node->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		m_pMap->m_pImmediateContext->DrawIndexed(node->m_dwFace * 3, 0, 0);
	}
	return true;
}
