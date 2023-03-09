#include "K_UIObject.h"
#include <d3d11.h>

bool K_UIObject::Frame()
{
	if (m_pCutInfoList.size() == 0) return false;
	m_pTextureSRV = m_pCutInfoList[0]->tc->GetSRV();

	SetPosition(m_OrginPos, PtoN(m_pCutInfoList[0]->pxWH), m_fDepth);
	SetUV(m_pCutInfoList[0]->uvTL, m_pCutInfoList[0]->uvBR);
	UpdateVertexBuffer();
	return true;
}

bool K_UIObject::Release()
{
	for (auto CI : m_pCutInfoList) { delete CI; }
	K_BaseObject::Release();

	return true;
}

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

	m_fAlpha = alpha;

	return true;
}

// Pixel to NDC
ImVec2 K_UIObject::PtoN(ImVec2 pxWH)
{
	ImVec2 result;

	result.x = pxWH.x / g_rtClient.right * 2.0f;
	result.y = pxWH.y / g_rtClient.bottom * 2.0f;

	return result;
}

bool K_UIObject::Save(std::ofstream& outfile)
{
	outfile << "Type\t";
	outfile <<  std::to_string(m_Type);
	outfile << "\n";

	outfile << "ID\t";
	outfile << std::to_string(m_ID);
	outfile << "\n";

	outfile << "Name\t";
	outfile << to_wm(m_szObjName);
	outfile << "\n";

	outfile << "OrginPos\t";
	outfile << std::to_string(m_OrginPos[0]);
	outfile << " ";
	outfile << std::to_string(m_OrginPos[1]);
	outfile << "\n";

	outfile << "Depth\t";
	outfile << std::to_string(m_fDepth);
	outfile << "\n";

	outfile << "Alpha\t";
	outfile << std::to_string(m_fAlpha);
	outfile << "\n";
	
	outfile << "CutNum\t";
	outfile << std::to_string(m_pCutInfoList.size());
	outfile << "\n";

	for (auto cutInfo : m_pCutInfoList) 
	{
		outfile << "CutInfo\t";
		outfile << std::to_string(cutInfo->cn);
		outfile << " | ";

		outfile << std::to_string(cutInfo->pxWH[0]);
		outfile << " ";
		outfile << std::to_string(cutInfo->pxWH[1]);
		outfile << " | ";

		outfile << std::to_string(cutInfo->uvTL[0]);
		outfile << " ";
		outfile << std::to_string(cutInfo->uvTL[1]);
		outfile << " ";
		outfile << std::to_string(cutInfo->uvBR[0]);
		outfile << " ";
		outfile << std::to_string(cutInfo->uvBR[1]);
		outfile << " | ";

		outfile << to_wm(cutInfo->tn);
		outfile << " | ";

		outfile << to_wm(cutInfo->sn);
		outfile << "\n";
	}

	outfile << "=\n";

	return true;
}

bool K_UIObject::Load()
{
	return true;
}

bool K_UIObject::AddCut(ImVec2 pxWH, ImVec2 uvTL, ImVec2 uvBR, std::wstring tn, std::wstring sn)
{
	CI* newCI = new CI;
	newCI->pxWH = pxWH;
	newCI->uvTL = uvTL;
	newCI->uvBR = uvBR;
	newCI->tn = tn;
	newCI->sn = sn;

	K_Texture* newTexture = I_Tex.Load(tn);
	if (newTexture == nullptr) { return false; }

	newCI->tc = newTexture;
	m_pCutInfoList.push_back(newCI);

	newCI->cn = m_iCurrentCutNum;
	m_iCurrentCutNum++;

	return true;
}

bool K_UIObject::AddCut(CI newCut)
{
	CI* newCI = new CI;
	newCI->pxWH = newCut.pxWH;
	newCI->uvTL = newCut.uvTL;
	newCI->uvBR = newCut.uvBR;
	newCI->tn = newCut.tn;
	newCI->sn = newCut.sn;

	K_Texture* newTexture = I_Tex.Load(newCut.tn);
	if (newTexture == nullptr) { return false; }

	newCI->tc = newTexture;
	m_pCutInfoList.push_back(newCI);

	newCI->cn = m_iCurrentCutNum;
	m_iCurrentCutNum++;

	return true;
}
