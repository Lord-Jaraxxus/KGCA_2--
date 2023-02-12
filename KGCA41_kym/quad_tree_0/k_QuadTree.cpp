#include "k_QuadTree.h"

void k_QuadTree::BuildQuadTree(k_Node* parent, float width, float heigt) 
{
	if (parent == nullptr)  return; 

	float x1 = parent->m_sRect.x1;
	float y1 = parent->m_sRect.y1;
	float w = parent->m_sRect.w;
	float h = parent->m_sRect.h;

	if (parent->m_iDepth < 2)
	{
		parent->m_pChild[0] = new k_Node(parent, x1, y1, w/2.0f, h/2.0f);
		parent->m_pChild[1] = new k_Node(parent, (x1 + w / 2.0f), y1, w / 2.0f, h / 2.0f);
		parent->m_pChild[2] = new k_Node(parent, x1, (y1 + h / 2.0f), w / 2.0f, h / 2.0f);
		parent->m_pChild[3] = new k_Node(parent, (x1 + w / 2.0f), (y1 + h/2.0f), w / 2.0f, h / 2.0f);
	}
	for (int iChild = 0; iChild < 4; iChild++) 
	{
		BuildQuadTree(parent->m_pChild[iChild], w / 2.0f, h / 2.0f);
	}
}

