#include "k_MapObject2D.h"

void k_MapObject2D::Frame(float fDeltaTime, float fGameTime)
{
	SetPosition(m_sRect.x1, m_sRect.y1, m_sRect.w, m_sRect.h);

	for (int iVertex = 0; iVertex < m_VertexList.size(); iVertex++)
	{
		m_VertexList[iVertex] = { m_VertexList[iVertex].p, {0.1f, 0.3f, 0.2f, 0.0f} };
	}

	K_BaseObject::Frame();
}
