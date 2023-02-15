#include "K_UIObject.h"

bool K_UIObject::SetUV(ImVec2 uvTL, ImVec2 uvBR)
{
	m_VertexList[0].t.x = uvTL.x;
	m_VertexList[0].t.y = uvTL.y;

	m_VertexList[1].t.x = uvBR.x;
	m_VertexList[1].t.y = uvTL.y;

	m_VertexList[2].t.x = uvTL.x;
	m_VertexList[2].t.y = uvBR.y;

	m_VertexList[3].t.x = uvBR.x;
	m_VertexList[3].t.y = uvBR.y;

	return true;
}

bool K_UIObject::SetPosition(ImVec2 orginPos, ImVec2 widthHeight)
{
	m_VertexList[0].p = { orginPos.x - widthHeight.x / 2, orginPos.y + widthHeight.y / 2, 0.0f };
	m_VertexList[1].p = { orginPos.x + widthHeight.x / 2, orginPos.y + widthHeight.y / 2, 0.0f };
	m_VertexList[2].p = { orginPos.x - widthHeight.x / 2, orginPos.y - widthHeight.y / 2, 0.0f };
	m_VertexList[3].p = { orginPos.x + widthHeight.x / 2, orginPos.y - widthHeight.y / 2, 0.0f };

	return true;
}
