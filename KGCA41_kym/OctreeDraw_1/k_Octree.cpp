#include "k_Octree.h"

void k_Octree::BuildOctree(k_Node* parent, k_Vector Size)
{
	if (parent == nullptr)  return; 

	k_Vector vPos = parent->m_Box.vMin;
	k_Vector vOrgin = parent->m_Box.vMin;
	k_Vector vChildSize = Size / 2.0f;

	if (parent->m_iDepth < 2)
	{
		vPos.Set(vOrgin.x, vOrgin.y, vOrgin.z);
		parent->m_pChild[0] = new k_Node(parent, vPos, vChildSize); // x,y,z ��� �״��

		vPos.Set(vOrgin.x, vOrgin.y, vOrgin.z + vChildSize.z);
		parent->m_pChild[1] = new k_Node(parent, vPos, vChildSize); // z�� ����

		vPos.Set(vOrgin.x, vOrgin.y + vChildSize.y, vOrgin.z);
		parent->m_pChild[2] = new k_Node(parent, vPos, vChildSize); // y�� ����

		vPos.Set(vOrgin.x, vOrgin.y + vChildSize.y, vOrgin.z + vChildSize.z);
		parent->m_pChild[3] = new k_Node(parent, vPos, vChildSize); // z, y ����
		
		vPos.Set(vOrgin.x + vChildSize.x, vOrgin.y, vOrgin.z);
		parent->m_pChild[4] = new k_Node(parent, vPos, vChildSize); // x�� ����

		vPos.Set(vOrgin.x + vChildSize.x, vOrgin.y, vOrgin.z + vChildSize.z);
		parent->m_pChild[5] = new k_Node(parent, vPos, vChildSize); // x, z ����

		vPos.Set(vOrgin.x + vChildSize.x, vOrgin.y + vChildSize.y, vOrgin.z);
		parent->m_pChild[6] = new k_Node(parent, vPos, vChildSize); // x, y ����

		vPos.Set(vOrgin.x + vChildSize.x, vOrgin.y + vChildSize.y, vOrgin.z + vChildSize.z);
		parent->m_pChild[7] = new k_Node(parent, vPos, vChildSize); // x, y, z ��� ����

	}
	for (int iChild = 0; iChild < 8; iChild++) 
	{
		BuildOctree(parent->m_pChild[iChild], vChildSize);
	}
}

