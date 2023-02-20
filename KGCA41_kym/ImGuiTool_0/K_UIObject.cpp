#include "K_UIObject.h"
#include <d3d11.h>

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

bool K_UIObject::SetPosition(ImVec2 orginPos, ImVec2 widthHeight, float depth) // NDC 좌표계 기준 원점(중앙)과 너비, 높이를 받아와서 세팅
{
	m_VertexList[0].p = { orginPos.x - widthHeight.x / 2, orginPos.y + widthHeight.y / 2, depth };
	m_VertexList[1].p = { orginPos.x + widthHeight.x / 2, orginPos.y + widthHeight.y / 2, depth };
	m_VertexList[2].p = { orginPos.x - widthHeight.x / 2, orginPos.y - widthHeight.y / 2, depth };
	m_VertexList[3].p = { orginPos.x + widthHeight.x / 2, orginPos.y - widthHeight.y / 2, depth };

	m_OrginPos = orginPos;
	m_WidthHeight = widthHeight;
	m_fDepth = depth;

	return true;
}


bool K_UIObject::SetAlpha(float alpha)
{
	m_VertexList[0].c.w = alpha;
	m_VertexList[1].c.w = alpha;
	m_VertexList[2].c.w = alpha;
	m_VertexList[3].c.w = alpha;

	return true;
}

ImVec2 K_UIObject::PtoN(ImVec2 pxWH)
{
	ImVec2 result;

	result.x = pxWH.x / g_rtClient.right;
	result.y = pxWH.y / g_rtClient.bottom;

	return result;
}