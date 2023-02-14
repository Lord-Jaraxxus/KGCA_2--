#include "K_Sprite.h"

bool K_Sprite::Frame()
{
	UpdateTexture();

	return true;
}

bool K_Sprite::Release()
{
	K_BaseObject::Release();

	for (auto CI : m_pCutInfoList)
	{
		CI->tc->Release();
		delete CI;
	}

	return true;
}

bool K_Sprite::UpdateTexture()
{
	m_pTextureSRV = m_pCutInfoList[m_iCurrentFrame]->tc->m_pTextureSRV;

	m_VertexList[0].t.x = m_pCutInfoList[m_iCurrentFrame]->uvBR.x;
	m_VertexList[0].t.y = m_pCutInfoList[m_iCurrentFrame]->uvBR.y;
	m_VertexList[0].t.x = m_pCutInfoList[m_iCurrentFrame]->uvBR.x;
	m_VertexList[0].t.x = m_pCutInfoList[m_iCurrentFrame]->uvBR.x;

	m_iCurrentFrame++;
	if (m_iCurrentFrame >= m_iMaxFrame) { m_iCurrentFrame = 0; }

	return true;
}

bool K_Sprite::AddCut(ImVec2 uvTL, ImVec2 uvBR, std::wstring tn, std::wstring sn)
{
	CI* newCI = new CI;
	newCI->uvTL = uvTL;
	newCI->uvBR = uvBR;
	newCI->tn = tn;
	newCI->sn = sn;

	K_Texture* newTexture = I_Tex.Load(tn);
	if (newTexture == nullptr) { return false; }
	
	newCI->tc = newTexture;
	m_pCutInfoList.push_back(newCI);

	return true;
}
